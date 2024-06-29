#include <iostream>

#include <glad/glad.h>
#include <glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/normalize_dot.hpp>

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



glm::vec3 screenToWorldRay(float mouseX, float mouseY, const glm::mat4& view, const glm::mat4& projection, int screenWidth, int screenHeight);
bool intersectRayWithCubeFace(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& cubePos, const glm::vec3& faceNormal, float& t);
void highlightIntersectedFace(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& cubePos, Shader shader);
void drawHighlightedFace(const glm::vec3& cubePos, const glm::vec3& faceNormal, Shader shader);


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
		Voxel(glm::ivec3(0, 0, 0), glm::vec3(0.25f, 0.57f, 0.98f)),
		Voxel(glm::ivec3(1, 1, 1), glm::vec3(0.25f, 0.30f, 0.98f)),
		// Add more vertices as needed
	};


	// std::vector<Mesh> meshes;
	// for (const auto& pair : world.getAllChunks()) {
	// 	  const Chunk& chunk = pair.second;
	// 	  Mesh mesh = generateMesh(chunk);
	// 	  meshes.push_back(mesh);
	// }

	unsigned int VBO, VAO;  //, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel::vertices), Voxel::vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	Box boundary(Point3D(0, 0, 0), Point3D(128, 128, 128));
	Octree octree(boundary, 4);
	for (const auto& voxel : voxels) {
		octree.insert(voxel);
	}

	std::cout << "Voxels inserted successfully." << std::endl;

	// Voxel voxel1(glm::ivec3(10, 10, 10), glm::vec3(1.0f, 0.0f, 0.0f));
	// Voxel voxel2(glm::ivec3(20, 20, 20), glm::vec3(0.0f, 1.0f, 0.0f));
	// Voxel voxel3(glm::ivec3(30, 30, 30), glm::vec3(0.0f, 0.0f, 1.0f));


	// main loop
	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setMat4("view", camera.getViewMatrix());
		shader.setMat4("projection", glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));

		glBindVertexArray(VAO);

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		glm::vec3 rayDir = screenToWorldRay(mouseX, mouseY, camera.getViewMatrix(), glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f), SCR_WIDTH, SCR_HEIGHT);

		for (const auto& voxel : octree.getVoxelsForRendering()) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(voxel.position));
			shader.setMat4("model", model);
			shader.setVec3("color", voxel.color);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			highlightIntersectedFace(camera.position, rayDir, glm::vec3(voxel.position), shader);
		}

		glBindVertexArray(0);

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
		camera.processKeyboard(FORWARD, shifting, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, shifting, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, shifting, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, shifting, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processKeyboard(UP, shifting, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.processKeyboard(DOWN, shifting, deltaTime);

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

	camera.processMouseMovement(xoffset, yoffset);
}

// whenever the mouse scroll wheel scrolls
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processMouseScroll(yoffset);
}

glm::vec3 screenToWorldRay(float mouseX, float mouseY, const glm::mat4& view, const glm::mat4& projection, int screenWidth, int screenHeight) {
	float x = (2.0f * mouseX) / screenWidth - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / screenHeight;
	float z = 1.0f;

	glm::vec3 ray_nds = glm::vec3(x, y, z);
	glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0);
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	glm::vec3 ray_wor = glm::vec3(glm::inverse(view) * ray_eye);
	ray_wor = glm::normalize(ray_wor);

	return ray_wor;
}

bool intersectRayWithCubeFace(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& cubePos, const glm::vec3& faceNormal, float& t) {
	float denom = glm::dot(faceNormal, rayDir);
	if (glm::abs(denom) > 1e-6) {
		glm::vec3 p0l0 = cubePos - rayOrigin;
		t = glm::dot(p0l0, faceNormal) / denom;
		return (t >= 0);
	}
	return false;
}

void highlightIntersectedFace(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& cubePos, Shader shader) {
	std::vector<glm::vec3> faceNormals = {
		glm::vec3(0.0f, 0.0f, -1.0f), // -Z
		glm::vec3(0.0f, 0.0f, 1.0f),  // +Z
		glm::vec3(0.0f, -1.0f, 0.0f), // -Y
		glm::vec3(0.0f, 1.0f, 0.0f),  // +Y
		glm::vec3(-1.0f, 0.0f, 0.0f), // -X
		glm::vec3(1.0f, 0.0f, 0.0f)   // +X
	};

	float t;
	bool intersected = false;
	glm::vec3 intersectedNormal;
	for (const auto& normal : faceNormals) {
		if (intersectRayWithCubeFace(rayOrigin, rayDir, cubePos, normal, t)) {
			intersected = true;
			intersectedNormal = normal;
			break;
		}
	}

	if (intersected) {
		drawHighlightedFace(cubePos, intersectedNormal, shader);
	}
}

void drawHighlightedFace(const glm::vec3& cubePos, const glm::vec3& faceNormal, Shader shader) {
	// implement drawing code to highlight the face based on the normal direction
	// this can involve setting a different color for the face vartices or drawing and outline
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 v0, v1, v2, v3;

	// -Z
	if (faceNormal == glm::vec3(0.0f, 0.0f, -1.0f)) {
		v0 = cubePos + glm::vec3(-0.5f, -0.5f, -0.5f);
		v1 = cubePos + glm::vec3(0.5f, -0.5f, -0.5f);
		v2 = cubePos + glm::vec3(0.5f, 0.5f, -0.5f);
		v3 = cubePos + glm::vec3(-0.5f, 0.5f, -0.5f);
	}

	// +Z
	else if (faceNormal == glm::vec3(0.0f, 0.0f, 1.0f)) {
		v0 = cubePos + glm::vec3(-0.5f, -0.5f, 0.5f);
		v1 = cubePos + glm::vec3(0.5f, -0.5f, 0.5f);
		v2 = cubePos + glm::vec3(0.5f, 0.5f, 0.5f);
		v3 = cubePos + glm::vec3(-0.5f, 0.5f, 0.5f);
	}

	// -Y
	else if (faceNormal == glm::vec3(0.0f, -1.0f, 0.0f)) {
		v0 = cubePos + glm::vec3(-0.5f, -0.5f, -0.5f);
		v1 = cubePos + glm::vec3(0.5f, -0.5f, -0.5f);
		v2 = cubePos + glm::vec3(0.5f, -0.5f, 0.5f);
		v3 = cubePos + glm::vec3(-0.5f, -0.5f, 0.5f);
	}

	// +Y
	else if (faceNormal == glm::vec3(0.0f, 1.0f, 0.0f)) {
		v0 = cubePos + glm::vec3(-0.5f, 0.5f, -0.5f);
		v1 = cubePos + glm::vec3(0.5f, 0.5f, -0.5f);
		v2 = cubePos + glm::vec3(0.5f, 0.5f, 0.5f);
		v3 = cubePos + glm::vec3(-0.5f, 0.5f, 0.5f);
	}

	// -X
	else if (faceNormal == glm::vec3(-1.0f, 0.0f, 0.0f)) {
		v0 = cubePos + glm::vec3(-0.5f, -0.5f, -0.5f);
		v1 = cubePos + glm::vec3(-0.5f, 0.5f, -0.5f);
		v2 = cubePos + glm::vec3(-0.5f, 0.5f, 0.5f);
		v3 = cubePos + glm::vec3(-0.5f, -0.5f, 0.5f);
	}

	// +X
	else if (faceNormal == glm::vec3(1.0f, 0.0f, 0.0f)) {
		v0 = cubePos + glm::vec3(0.5f, -0.5f, -0.5f);
		v1 = cubePos + glm::vec3(0.5f, 0.5f, -0.5f);
		v2 = cubePos + glm::vec3(0.5f, 0.5f, 0.5f);
		v3 = cubePos + glm::vec3(0.5f, -0.5f, 0.5f);
	}

	glm::vec3 vertices[] = {
		 v0, v1, v2,
		 v2, v3, v0,
	};

	unsigned int hl_VAO, hl_VBO;
	glGenVertexArrays(1, &hl_VAO);
	glGenBuffers(1, &hl_VBO);

	glBindVertexArray(hl_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, hl_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	shader.setVec3("color", color);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &hl_VBO);
	glDeleteVertexArrays(1, &hl_VAO);
}