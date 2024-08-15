#include <iostream>

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <glfw3.h>

#include "shaders/shader.h"

GLFWwindow* window;
bool running = false;

float vertices[] = {
	0.0f, -0.1f, 0.0f,
	0.1f, -0.1f, 0.0f,
	0.1f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f
};

const int numSquares = 5;

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	Shader shader = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
	shader.use();

	// random colors
	glm::vec3 colors[numSquares];
	for (unsigned int i = 0; i < numSquares; i++) {
		colors[i] = glm::vec3(
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
		);
	}

	// square positions
	glm::mat4 models[numSquares];
	for (unsigned int i = 0; i < numSquares; i++) {
		float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f;
		float y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f;
		models[i] = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
	}

	// generate buffers
	GLuint VAO, VBO, colorVBO, modelVBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &colorVBO);
	glGenBuffers(1, &modelVBO);

	glBindVertexArray(VAO);

	// vert buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	// model buffer
	glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(models), models, GL_STATIC_DRAW);

	// add squares to shader
	for (unsigned int i = 0; i < 4; i++) {
		glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
		glEnableVertexAttribArray(2 + i);
		glVertexAttribDivisor(2 + i, 1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	running = true;

	// main loop
	while (running) {
		glBindVertexArray(VAO);
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, numSquares);
		glBindVertexArray(0);

		if (glfwWindowShouldClose(window)) {
			running = false;
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glBindVertexArray(VAO);
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, numSquares);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		processInput();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &colorVBO);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}