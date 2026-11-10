#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using namespace std;

constexpr int WINDOW_WIDTH  = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr string WINDOW_NAME = "Hello, Window";


void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0,0,w,h);
}

void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window,true);
}


int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_NAME.c_str(),NULL,NULL);
	if (window == NULL)
	{
		cout << "Não deu pra criar a janela GLFW" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Não deu pra inicialidar o GLAD" << endl;
		return -1;
	}

	glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
	
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

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
