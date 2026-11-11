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
	-0.5f,  0.5f, 0.0f   // top left 
};

float vertices2[] = {
	0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 2 //,   // first triangle
    //1, 2, 3    // second triangle
};

unsigned int indices2[] ={
	0, 1, 2
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

// Frag Shader que pinta tudo de amarelo
const char* yellow_fragmentShaderSource = ""
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4( 1.0f, 1.0f, 0.0f, 1.0f );\n"
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

	// Fragments são todas as propriedades necessárias para saber a cor do pixel
	// Criar o Fragment Shader
	unsigned int yellow_fragmentShader;
	yellow_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Atachar a fonte ao objeto de shader e compilar
	glShaderSource(yellow_fragmentShader, 1, &yellow_fragmentShaderSource, NULL);
	glCompileShader(yellow_fragmentShader);
	{
		int ok;
		glGetShaderiv(yellow_fragmentShader,GL_COMPILE_STATUS, &ok);
		if(!ok)
		{
			char log[LOG_BUFF_SIZE];
			glGetShaderInfoLog(yellow_fragmentShader,LOG_BUFF_SIZE, NULL, log);
			cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << endl;
			cout << log << endl;
		}
	}

	// Cria a programação de shader. 
	// Ela que vai linkar a saída de um shader para a entrada de outro
	unsigned int shaderProgram1;
	shaderProgram1 = glCreateProgram();

	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader);
	glLinkProgram(shaderProgram1);
	{
		int ok;
		glGetProgramiv(shaderProgram1,GL_LINK_STATUS, &ok);
		if(!ok){
			char log[LOG_BUFF_SIZE];
			glGetProgramInfoLog(shaderProgram1,LOG_BUFF_SIZE,NULL,log);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << endl;
			cout << log << endl;
		}
	}
	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();

	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, yellow_fragmentShader);
	glLinkProgram(shaderProgram2);
	{
		int ok;
		glGetProgramiv(shaderProgram2,GL_LINK_STATUS, &ok);
		if(!ok){
			char log[LOG_BUFF_SIZE];
			glGetProgramInfoLog(shaderProgram2,LOG_BUFF_SIZE,NULL,log);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << endl;
			cout << log << endl;
		}
	}

	// Seta o a programação para ser essa para as próximas drawcalls
	// não sei se da pra mudar os shaders dentro dela em tempo de execução
	// ou se é melhor trocar o programa todo
	glUseProgram(shaderProgram1);

	// Pelo jeito aqui os dados do shader já estão na GPU
	// TODO: entender em quais funções que os dados vão para a GPU em si
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(yellow_fragmentShader);
	
	// -----------------------
	// O V Array Object armazena essas configurações 
	// para cada tipo de objeto que a gente pode ter
	// essas "configurações" são as chamadas para glBindBuffe

	unsigned int VAO1;
	glGenVertexArrays(1,&VAO1);

	// Criar o Vertex Buffer
	unsigned int VBO1;
	glGenBuffers(1, &VBO1);
	unsigned int EBO1;
	glGenBuffers(1,&EBO1);

	glBindVertexArray(VAO1);
	{
		
		// Carregar os dados para o Vertex Buffer CPU -> GPU
		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Configura como interpretar os atributos do vertex shader
		// Que estão no VBO
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0 );
		glEnableVertexAttribArray(0);
		
		
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO1 );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
	}
	glBindVertexArray(0);

	unsigned int VAO2;
	glGenVertexArrays(1,&VAO2);

	// Criar o Vertex Buffer
	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	unsigned int EBO2;
	glGenBuffers(1,&EBO2);

	glBindVertexArray(VAO2);
	{
		
		// Carregar os dados para o Vertex Buffer CPU -> GPU
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		// Configura como interpretar os atributos do vertex shader
		// Que estão no VBO
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0 );
		glEnableVertexAttribArray(0);
		
		
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO2 );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices2 ), indices2, GL_STATIC_DRAW );
	}
	glBindVertexArray(0);



	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Render
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT ); // escolhe qual dos buffers que vai dar clear

		glUseProgram( shaderProgram1 );
		glBindVertexArray( VAO1 );
		// glDrawArrays( GL_TRIANGLES, 0, 3 );
		glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		
		glUseProgram( shaderProgram2 );
		glBindVertexArray( VAO2 );
		glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers( window );
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
