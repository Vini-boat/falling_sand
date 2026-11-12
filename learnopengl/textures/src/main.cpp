#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

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
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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
			8 * sizeof(float),				// stride, o offset entre atributos do mesmo tipo
			(void*)(6 * sizeof(float))      // offset inicial para o começo do atributo
		);
		glEnableVertexAttribArray(2);		// ativa essa configuração efetivamente

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
	}
	glBindVertexArray(0);

	// por conta das coordenadas de exibição serem com o y invertido
	// por conta dos backgrouds diferentes
	stbi_set_flip_vertically_on_load(true);
	
	// A textura também é um objeto da GL
	glID texture;
	glGenTextures(1, &texture);
	
	// Da pra binder GL_TEXTURE_1D, GL_TEXTURE_2D e GL_TEXTURE_3D
	// da mesma forma como os diferentes tipos de Buffers que podem ser bindados 
	glBindTexture(GL_TEXTURE_2D, texture);
	{
		// Configura o filtro da textura
		// os mas básicos são o NEAREST e o LINEAR
	    //   NEAREST: pega a cor do "pixel" (texel) que a coordenada ta em cima
		//   LINEAR:  interpola as cores dos pixels próximos baseado na distância
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Configura o que acontece quando tu acessa um valor fora do ranfe 0.0 <-> 1.0
		// QUESTION: S e T são equivalentes a X e Y só que pra texturas 2D? 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Carrega a imagem usando a std_image
		int width, height, numberChannels;
		const char* path = ASSETS_DIR"/textures/container.jpg"; 
		unsigned char* data = stbi_load(
			path,
			&width, &height, &numberChannels, 3 // esse 3 significa que são 3 canais (RGB)
		);
		if(data){
			// Carrega a textura no GL_TEXTURE_2D bindado
			glTexImage2D(
				GL_TEXTURE_2D,    // target
				0,				  // nível do mipmap a ser carregado
				GL_RGB,			  // diz o número de componentes de cor na textura
				width, height,    // tamanho da textura
				0,                // coisa de legado, sempre 0
				GL_RGB,     	  // formato dos dados
				GL_UNSIGNED_BYTE, // tipo de dados 
				data              // ponteiro para os dados em si
			);
			// Gera os mipmaps
			// Texturas com menos detalhes para evitar problemas quando elas são escalonadas
			glGenerateMipmap(GL_TEXTURE_2D);  
		}
		else
		{
			cout << "ERROR::TEXTURE::FAILED_TO_LOAD" << endl;
			cout << path << endl; 
			cout << stbi_failure_reason() << endl;
		}
		stbi_image_free(data);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glID texture2;
	glGenTextures(1, &texture2);
	
	glBindTexture(GL_TEXTURE_2D, texture2);
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int width, height, numberChannels;
		const char* path = ASSETS_DIR"/textures/face.png"; 
		unsigned char* data = stbi_load(
			path,
			&width, &height, &numberChannels, 4
		);
		if(data){
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				width, height,
				0,
				GL_RGBA, GL_UNSIGNED_BYTE,
				data
			);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			cout << "ERROR::TEXTURE::FAILED_TO_LOAD" << endl;
			cout << path << endl; 
			cout << stbi_failure_reason() << endl;
		}
		stbi_image_free(data);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	
	

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
		
		// quando binda o texture ele fica relacionado ao texture unit bindado
		glBindTexture(GL_TEXTURE_2D, texture);
		
		// tem que passar o id do texture unit para o uniform sempler poder usar
		shaderProgram1.setUniform("texture1", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		shaderProgram1.setUniform("texture2", 1);
		
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		glfwSwapBuffers( window );
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
