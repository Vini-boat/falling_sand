#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "window/window.hpp"

using namespace std;

constexpr int WINDOW_WIDTH  = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr string WINDOW_NAME = "Hello, Window";

void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window,true);
}

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.0f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

int main() 
{
	GLFWwindow* window = initWindow(WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_NAME.c_str());

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Render
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
