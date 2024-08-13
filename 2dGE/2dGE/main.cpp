#include <iostream>

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <glfw3.h>

#include "shaders/shader.h"

GLFWwindow* window;
bool running = false;

void processInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		running = false;
}

int main() {
	std::cout << "Starting 2dVoxGE..." << std::endl;

	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 800, "2dVoxGE", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	//glfwSetWindowUserPointer(window, this); NOT SURE WHAT THIS DOES. THINK IT IS USED WHEN USING THE 'RENDERER' CLASS OR SIMILAR

	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); CHANGE WINDOW SIZE
	//glfwSetCursorPosCallback(window, mouse_callback);	MOUSE MOVEMENT
	//glfwSetScrollCallback(window, scroll_callback); SCROLL

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); ENABLE MOUSE CURSUR INPUT

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	//glEnable(GL_DEPTH_TEST);

	Shader shader = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
	shader.use();

	running = true;

	while (running) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
		processInput();
	}
}