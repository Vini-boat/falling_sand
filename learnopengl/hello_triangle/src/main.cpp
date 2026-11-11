#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "window/window.hpp"

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
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

// Shader que recebe as coordenadas já prontas para exibir
//  Só passa as posições para a próxima etapa da rendering pipeline
const char* vertexShaderSource = ""
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4( aPos.x, aPos.y, aPos.z, 1.0f );\n"
"}\0";

// Frag Shader que pinta tudo de laranja
const char* fragmentShaderSource = ""
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4( 1.0f, 0.5f, 0.2f, 1.0f );\n"
"}\n";




int main() 
{
	GLFWwindow* window = initWindow(WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_NAME);

	// Criar o Objeto de Vertex Shader
    unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Atachar a fonte ao objeto de shader e compilar
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	{
		int ok;
		glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &ok);
		if(!ok)
		{
			char log[LOG_BUFF_SIZE];
			glGetShaderInfoLog( vertexShader, LOG_BUFF_SIZE, NULL, log);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << endl;
			cout << log << endl;
		}
	}

	// Fragments são todas as propriedades necessárias para saber a cor do pixel
	// Criar o Fragment Shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Atachar a fonte ao objeto de shader e compilar
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	{
		int ok;
		glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &ok);
		if(!ok)
		{
			char log[LOG_BUFF_SIZE];
			glGetShaderInfoLog(fragmentShader,LOG_BUFF_SIZE, NULL, log);
			cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << endl;
			cout << log << endl;
		}
	}

	// Cria a programação de shader. 
	// Ela que vai linkar a saída de um shader para a entrada de outro
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	{
		int ok;
		glGetProgramiv(shaderProgram,GL_LINK_STATUS, &ok);
		if(!ok){
			char log[LOG_BUFF_SIZE];
			glGetProgramInfoLog(shaderProgram,LOG_BUFF_SIZE,NULL,log);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << endl;
			cout << log << endl;
		}
	}

	// Seta o a programação para ser essa para as próximas drawcalls
	// não sei se da pra mudar os shaders dentro dela em tempo de execução
	// ou se é melhor trocar o programa todo
	glUseProgram(shaderProgram);

	// Pelo jeito aqui os dados do shader já estão na GPU
	// TODO: entender em quais funções que os dados vão para a GPU em si
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	// -----------------------
	// O V Array Object armazena essas configurações 
	// para cada tipo de objeto que a gente pode ter
	// essas "configurações" são as chamadas para glBindBuffe
	unsigned int VAO;
	glGenVertexArrays(1,&VAO);

	glBindVertexArray(VAO);

	// Criar o Vertex Buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	
	// Carregar os dados para o Vertex Buffer CPU -> GPU
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configura como interpretar os atributos do vertex shader
	// Que estão no VBO
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0 );
	glEnableVertexAttribArray(0);


	
	unsigned int EBO;
	glGenBuffers(1,&EBO);
	
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );



	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Render
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT ); // escolhe qual dos buffers que vai dar clear

		glUseProgram( shaderProgram );
		glBindVertexArray( VAO );
		// glDrawArrays( GL_TRIANGLES, 0, 3 );
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray( 0 );
		glfwSwapBuffers( window );
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
