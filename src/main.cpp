#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <math.h>

#include "window/window.hpp"
#include "models/vertex_layout.hpp"
#include "shaders/shader.hpp"
#include "textures/texture2d.hpp"
#include "camera/camera.hpp"

using namespace std;

constexpr int    WINDOW_WIDTH  = 800;
constexpr int    WINDOW_HEIGHT = 600;
constexpr char   WINDOW_NAME[] = "Hello, Window";
constexpr int 	 LOG_BUFF_SIZE = 512;

const auto x_axis = glm::vec3(1.0f,0.0f,0.0f);
const auto y_axis = glm::vec3(0.0f,1.0f,0.0f);
const auto z_axis = glm::vec3(0.0f,0.0f,1.0f);

auto dt = 0.0f;
auto time_last_frame = 0.0f;

void processInput(GLFWwindow* window)
{
	// Fechar a janela
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window,true);
}

float quad[] = {
	// pos         // uv
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,
};
int indices[] = {
	0,1,2,
	2,3,0
};

int main() 
{
	GLFWwindow* window = initWindow(WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_NAME);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int EBO;
	glGenBuffers(1,&EBO);
	
	VertexLayout VAO;
	VAO.bind();
	{

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

		VAO.setLayout(
			{
				{.qtdComponents = 2},
				{.qtdComponents = 2}
			}	
		);

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
	}
	glBindVertexArray(0);

	
	ShaderProgram shaderProgram1(
		ASSETS_DIR"/shaders/simulation/sim.vs",
		ASSETS_DIR"/shaders/simulation/sim.fs"
	);
	

	shaderProgram1.setUniform("paleta", 1);

	while(!glfwWindowShouldClose(window))
	{
		auto time = (float)glfwGetTime();
		dt = time - time_last_frame;
		time_last_frame = time;
		
		processInput(window);
		
		// Render
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT);
		
		shaderProgram1.use();
		VAO.bind();

		shaderProgram1.setUniform("tela", 0);

		glfwSwapBuffers( window );
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
