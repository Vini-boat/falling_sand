#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <math.h>

#include "window/window.hpp"
#include "types.hpp"
#include "shaders/shader.hpp"

using namespace std;

constexpr int    WINDOW_WIDTH  = 800;
constexpr int    WINDOW_HEIGHT = 600;
constexpr char   WINDOW_NAME[] = "Hello, Window";
constexpr int 	 LOG_BUFF_SIZE = 512;


void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window,true);

	if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);

	if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
}

float vertices[] = {
	 // positions         // colors
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
};
unsigned int indices[] = {
    0, 1, 2 
};

int main() 
{
	GLFWwindow* window = initWindow(WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_NAME);

	ShaderProgram shaderProgram1(
		ASSETS_DIR"/shaders/interpolation.vs",
		ASSETS_DIR"/shaders/interpolation.fs"
	);

	// O V Array Object armazena essas configurações 
	// para cada tipo de objeto que a gente pode ter
	// essas "configurações" são as chamadas para glBindBuffe

	unsigned int VAO;
	glGenVertexArrays(1,&VAO);

	// Criar o Vertex Buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int EBO;
	glGenBuffers(1,&EBO);

	glBindVertexArray(VAO);
	{
		
		// Carregar os dados para o Vertex Buffer CPU -> GPU
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Configura como interpretar os atributos do vertex shader
		// Que estão no VBO
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0 );
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)) );
		glEnableVertexAttribArray(1);

		
		
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
	}
	glBindVertexArray(0);

	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Render
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT ); // escolhe qual dos buffers que vai dar clear

		shaderProgram1.use();
		float timeValue = glfwGetTime();
		float offset = sin(timeValue);
		shaderProgram1.setUniform("uOffset", offset);
		glBindVertexArray( VAO );
		// glDrawArrays( GL_TRIANGLES, 0, 3 );
		glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		

		glfwSwapBuffers( window );
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
