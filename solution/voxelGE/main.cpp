#include <iostream>

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>

#include "shaders/shader.h"

#include "camera.h"
#include "octree.h"

//#include "voxel.h"
//#include "chunk.h"
//#include "mesh.h"
//#include "world.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


int main() {
	std::cout << "Starting VoxGE..." << std::endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "VoxGE", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

	shader.checkCompileErrors(shader.ID, "VERTEX");
	shader.checkCompileErrors(shader.ID, "FRAGMENT");
	shader.checkLinkErrors(shader.ID);

	// Define vertices and VAO
	std::vector<Voxel> voxels = {
		Voxel(glm::ivec3(10, 10, 10), glm::vec3(1.0f, 0.0f, 0.0f)),
		Voxel(glm::ivec3(20, 20, 20), glm::vec3(0.0f, 1.0f, 0.0f)),
		Voxel(glm::ivec3(30, 30, 30), glm::vec3(0.0f, 0.0f, 1.0f)),
		// Add more vertices as needed
	};


	// std::vector<Mesh> meshes;
	// for (const auto& pair : world.getAllChunks()) {
	// 	  const Chunk& chunk = pair.second;
	// 	  Mesh mesh = generateMesh(chunk);
	// 	  meshes.push_back(mesh);
	// }

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel) * voxels.size(), voxels.data(), GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, position));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	Box boundary(Point3D(0, 0, 0), Point3D(128, 128, 128));
	Octree octree(boundary, 4);

	// Voxel voxel1(glm::ivec3(10, 10, 10), glm::vec3(1.0f, 0.0f, 0.0f));
	// Voxel voxel2(glm::ivec3(20, 20, 20), glm::vec3(0.0f, 1.0f, 0.0f));
	// Voxel voxel3(glm::ivec3(30, 30, 30), glm::vec3(0.0f, 0.0f, 1.0f));

	for (const auto & voxel : voxels) {
		octree.insert(voxel);
	}

	std::cout << "Voxels inserted successfully." << std::endl;


	// main loop
	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setMat4("view", camera.GetViewMatrix());
		shader.setMat4("projection", glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));


		// Draw voxels from octree
		glBindVertexArray(VAO);
		for (const auto& voxel : octree.getVoxelsForRendering()) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(voxel.position));
			shader.setMat4("model", model);
			shader.setVec3("color", voxel.color);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;

}

// whenever the window size changed (by OS or user resize)
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// process input
void processInput(GLFWwindow* window) {
	bool shifting = false;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		shifting = true;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, shifting, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, shifting, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, shifting, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, shifting, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, shifting, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, shifting, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(RUN, shifting, deltaTime);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		std::cout << "Mouse Click" << std::endl;

}

// whenever the mouse moves
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// whenever the mouse scroll wheel scrolls
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}