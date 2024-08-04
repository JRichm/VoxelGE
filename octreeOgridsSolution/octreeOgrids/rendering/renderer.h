#pragma once

#include <vector>

#include <glad/glad.h>
#include <glfw3.h>

#include "camera.h"
#include "shader.h"

#include "../world/voxel.h"

class Renderer {
public:
	Renderer(int _width, int _height)
		: scr_width(_width),
		scr_height(_height),
		camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
		firstMouse(true),
		lastX(_width / 2.0),
		lastY(_height / 2.0),
		running(false),
		deltaTime(0.0f),
		lastFrame(0.0f) {}

	bool initglfw();
	void update();

	void createVoxelBuffers(const std::vector<glm::vec3>& positions, const std::vector<Voxel>& voxels);
	void terminate();
	bool running = false;	

private:
	GLFWwindow* window;
	Camera camera;
	Shader shader;
	GLuint voxelVAO, voxelVBO, voxelCBO;

	int scr_width;
	int scr_height;

	bool firstMouse = true; // wtf is this used for?
	float lastX;
	float lastY;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;


	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processInput();
};

