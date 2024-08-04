#include <iostream>
#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/normalize_dot.hpp>

#include "renderer.h"
#include "shader.h"


std::unordered_map<int, glm::vec3> voxelColors = {
	{ 1, glm::vec3(1.0f, 0.0f, 0.0f)},	// red
	{ 2, glm::vec3(0.0f, 1.0f, 0.0f)},	// green
	{ 3, glm::vec3(0.0f, 0.0f, 1.0f)},	// blue
};

// GLFW Initialization
bool Renderer::initglfw() {
	std::cout << "Opening window" << std::endl;

	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(scr_width, scr_height, "VoxGE", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	shader = Shader("rendering/shaders/vertex.glsl", "rendering/shaders/fragment.glsl");
	std::cout << "initglfw true" << std::endl;
	running = true;
	return true;
}


// Update Render (CALLED EVERY FRAME)
void Renderer::update() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	shader.use();
	shader.setMat4("view", camera.getViewMatrix());
	shader.setMat4("projection", glm::perspective(glm::radians(camera.zoom), (float)scr_width / (float)scr_height, 0.1f, 100.0f));

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(voxelVAO);
	glDrawArrays(GL_POINTS, 0, positions.size());

	glBindVertexArray(0);

	glfwSwapBuffers(window);
	glfwPollEvents();
	processInput();
}

void Renderer::terminate() {
	glfwSetWindowShouldClose(window, true);
	glDeleteVertexArrays(1, &voxelVAO);
	glDeleteBuffers(1, &voxelVBO);
	glfwTerminate();
}

void Renderer::createVoxelBuffers(const std::vector<glm::vec3>& positions, const std::vector<Voxel>& voxels) {
	std::cout << "Creating voxel buffers" << std::endl;

	std::vector<glm::vec3> colors;
	for (const auto& voxel : voxels) {
		auto it = voxelColors.find(voxel.type);
		glm::vec3 color;
		if (it != voxelColors.end()) {
			color = it->second;
		}
		else {
			color = glm::vec3(1.0f, 0.0f, 1.0f); // Default color if type not found
		}
		colors.push_back(color);
	}

	glGenVertexArrays(1, &voxelVAO);
	glGenBuffers(1, &voxelVBO);
	glGenBuffers(1, &voxelCBO);

	glBindVertexArray(voxelVAO);

	glBindBuffer(GL_ARRAY_BUFFER, voxelVBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, voxelCBO);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Store positions and colors in member variables
	this->positions = positions;
	this->colors = colors;
}

// process user input
void Renderer::processInput() {
	bool shifting = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		running = false;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.processKeyboard(FORWARD, shifting, deltaTime);
		std::cout << "\tpressing W" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.processKeyboard(BACKWARD, shifting, deltaTime);
		std::cout << "\tpressing S" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.processKeyboard(LEFT, shifting, deltaTime);
		std::cout << "\tpressing A" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.processKeyboard(RIGHT, shifting, deltaTime);
		std::cout << "\tpressing D" << std::endl;

	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.processKeyboard(UP, shifting, deltaTime);
		std::cout << "\tpressing SPACE" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		camera.processKeyboard(DOWN, shifting, deltaTime);
		std::cout << "\tpressing CTRL" << std::endl;
	}
}


void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Renderer::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	if (renderer->firstMouse) {
		renderer->lastX = xpos;
		renderer->lastY = ypos;
		renderer->firstMouse = false;
	}

	float xoffset = xpos - renderer->lastX;
	float yoffset = renderer->lastY - ypos; // reversed since y-coordinates range from bottom to top
	renderer->lastX = xpos;
	renderer->lastY = ypos;

	renderer->camera.processMouseMovement(xoffset, yoffset);
}

void Renderer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	renderer->camera.processMouseScroll(yoffset);
}