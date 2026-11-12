#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <string>
#include <math.h>

#include "window/window.hpp"
#include "types.hpp"
#include "shaders/shader.hpp"
#include "textures/texture2d.hpp"

using namespace std;

constexpr int    WINDOW_WIDTH  = 800;
constexpr int    WINDOW_HEIGHT = 600;
constexpr char   WINDOW_NAME[] = "Hello, Window";
constexpr int 	 LOG_BUFF_SIZE = 512;

float mix_factor = 0.2f;

void processInput(GLFWwindow* window)
{
	// Fechar a janela
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window,true);

	// Trocar o modo para só as linhas
	if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);

	if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
	
	// Trocar a proporção de mix das texturas
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		mix_factor = min(1.0f, mix_factor + 0.001f);
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		mix_factor = max(0.0f, mix_factor - 0.001f);
	
}

float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
};
unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 

int main() 
{
	GLFWwindow* window = initWindow(WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_NAME);

	ShaderProgram shaderProgram1(
		ASSETS_DIR"/shaders/texture/texture.vs",
		ASSETS_DIR"/shaders/texture/texture.fs"
	);

	// O V Array Object armazena essas configurações 
	// para cada tipo de objeto que a gente pode ter
	// essas "configurações" são as chamadas para glBindBuffe
    // TODO: criar a imagem mental dos objetos GL no excalidraw
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
		// Expecifica o tipo de buffer a ser bindado
		//   pode ser: GL_ARRAY_BUFFER ou GL_ELEMENT_ARRAY_BUFFER comummente (existem outros)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Configura como interpretar os atributos do vertex shader
		// Que estão no VBO
		
		// Cada atributo tem que ser configurado e ativado independentemente
		// QUESTION: Essas configurações de attribute pointers ficam salvas no VBO bindado?
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0 );
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)) );
		glEnableVertexAttribArray(1);
		
		glVertexAttribPointer( 
			2, 								// Indice do atributo a ser configurado
			2, 								// Número de componentes, tem que ser 1,2,3 ou 4 (pq vai virar um vec)
			GL_FLOAT,						// Data type de cada componenete
			GL_FALSE, 						// fala se o dado tem que ser normalizado, entre -1 e 1 para signed e 0 e 1 para unsigned
											//   só vale pra GL_BYTE E GL_UNSIGNED_BYTE
			8 * sizeof(float),				// stride, o offset em bytes entre o mesmo atributo em vértices consecutivos
			(void*)(6 * sizeof(float))      // offset inicial para o começo do atributo
		);
		glEnableVertexAttribArray(2);		// ativa a configuração 2 efetivamente. pode ser chamado em qqr momento com o VAO bindado

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
	}
	glBindVertexArray(0);

	
	Texture2D boxTexture(ASSETS_DIR"/textures/container.jpg",{
		.wrap_s = GL_CLAMP_TO_EDGE,
		.wrap_t = GL_CLAMP_TO_EDGE,
	});
	Texture2D faceTexture(ASSETS_DIR"/textures/face.png");

	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Render
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT ); // escolhe qual dos buffers que vai dar clear

		shaderProgram1.use();
		glBindVertexArray( VAO );

		// Tem essas texture units
		// elas são targets que podem ser ativos para o 
		//   shader poder usar várias texturas ao mesmo tempo
		glActiveTexture(GL_TEXTURE0);
		boxTexture.bind();
		// tem que passar o id do texture unit para o uniform sempler poder usar
		shaderProgram1.setUniform("texture1", 0);

		glActiveTexture(GL_TEXTURE1);
		faceTexture.bind();
		shaderProgram1.setUniform("texture2", 1);

		shaderProgram1.setUniform("mixFactor",mix_factor);
		
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		glfwSwapBuffers( window );
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
