#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <math.h>

#include "window/window.hpp"
#include "types.hpp"
#include "models/vertex_layout.hpp"
#include "shaders/shader.hpp"
#include "textures/texture2d.hpp"

using namespace std;

constexpr int    WINDOW_WIDTH  = 800;
constexpr int    WINDOW_HEIGHT = 600;
constexpr char   WINDOW_NAME[] = "Hello, Window";
constexpr int 	 LOG_BUFF_SIZE = 512;

float mix_factor = 0.2f;
auto fov = 45.0f;



const auto x_axis = glm::vec3(1.0f,0.0f,0.0f);
const auto y_axis = glm::vec3(0.0f,1.0f,0.0f);
const auto z_axis = glm::vec3(0.0f,0.0f,1.0f);

auto cameraPos       = glm::vec3(0.0f,0.0f,3.0f);
auto cameraFront     = -z_axis;
auto cameraUp        = y_axis;

auto dt = 0.0f;
auto time_last_frame = 0.0f;

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
	if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		mix_factor = min(1.0f, mix_factor + 0.001f);
	if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		mix_factor = max(0.0f, mix_factor - 0.001f);

	// Muda o fov
	if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		fov = min(180.0f, fov + 0.01f);
		if(glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		fov = max(0.0f, fov - 0.01f);
		
	// camera control
	auto cameraSpeed = 2.5f * dt;
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos -= cameraFront * cameraSpeed;
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos += cameraFront * cameraSpeed;
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
// float vertices[] = {
//     // positions          // colors           // texture coords
//      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
//      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
//     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
// };
// unsigned int indices[] = {
//     0, 1, 3,   // first triangle
//     1, 2, 3    // second triangle
// }; 

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

int main() 
{
	GLFWwindow* window = initWindow(WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_NAME);

	// TODO: Assim que terminar o introduction, acho que vamos ter que abistrair essa parte.
	//       por enquando vou manter na main
	// -------------------------------------------------------------------------------------
	
	// O V Array Object armazena essas configurações 
	// para cada tipo de objeto que a gente pode ter
	// essas "configurações" são as chamadas para glBindBuffe
    // TODO: criar a imagem mental dos objetos GL no excalidraw
	
	// Criar o Vertex Buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int EBO;
	glGenBuffers(1,&EBO);
	
	VertexLayout VAO;
	VAO.bind();
	{
		
		// Carregar os dados para o Vertex Buffer CPU -> GPU
		// Expecifica o tipo de buffer a ser bindado
		//   pode ser: GL_ARRAY_BUFFER ou GL_ELEMENT_ARRAY_BUFFER comummente (existem outros)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Configura como interpretar os atributos do vertex shader
		// Que estão no VBO

		// Tem que ser chamado depois de bindar o VBO
		// já que vai setar o layout daquele Buffer
		VAO.setLayout(
			{
				{.qtdComponents = 3},
				{.qtdComponents = 2}
			}	
		);

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
		// glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
	}
	glBindVertexArray(0);
	

	
	
	
	ShaderProgram shaderProgram1(
		ASSETS_DIR"/shaders/coordinate_systems/coord.vs",
		ASSETS_DIR"/shaders/coordinate_systems/coord.fs"
	);
	
	Texture2D boxTexture(ASSETS_DIR"/textures/container.jpg",{
		.wrap_s = WrapMode::CLAMP_TO_EDGE,
		.wrap_t = WrapMode::CLAMP_TO_EDGE,
	});
	Texture2D faceTexture(ASSETS_DIR"/textures/face.png");
	
	glEnable(GL_DEPTH_TEST);
	
	while(!glfwWindowShouldClose(window))
	{
		auto time = (float)glfwGetTime();
		dt = time_last_frame - time;
		time_last_frame = time;
		
		processInput(window);
		
		// Render
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // escolhe qual dos buffers que vai dar clear
		
		shaderProgram1.use();
		VAO.bind();
		
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
		
		
		// A model matrix representa o "estado" do modelo em relação ao mundo
		//   Rotação, translação e escalonamento
		// Na prática ela muda o transform da quele objeto específico
		// auto model = glm::mat4(1.0f);
		// model = glm::rotate(model,glm::radians(-55.0f), x_axis); // Essa aqui só faz com que ele seja rotacionado em relação ao eixo x pra parecer estar "deitada"
		
		// A view matrix é como se fosse o "estado" da câmera no mundo
		//  Rotação e translação dela.
		// Na prática ela muda o transform do mundo inteiro inversamente. 
		//   Já que virar o mundo pra um lado é a mesma coisa que virar a câmera para o lado oposto
		auto view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos,cameraPos + cameraFront, cameraUp);
		// view = glm::translate(view, cameraPos); // essa aqui só faz a câmera estar um pouco pra trás para o objeto não ser clipado
		// A projection matrix represenda o "campo de visão" da câmera
		//   Ela que vai dizer o que está visível ou não.
		// Na prática ela adiciona a perspectiva. 
		//  Transformando o frustum em um cubo e também 
		//  deixa a informação de "distância da câmera" no componente w do vetor.
		//  Daí a GPU divide (x,y,z) por w, o que faz dar a sensação de perspectiva
		auto projection = glm::perspective(              // serve para descrever o "frustum" (pirâmide truncada de visão)
			glm::radians(fov),                         // FOV:                     que na prática é o ângulo vertical visível pela câmera 
			(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,  // Aspec Ratio:             define a "forma" do monitor 
			0.1f,                                        // Distância do Near plane: Conceitualmente é a distância do "olho" até a "tela", só o que está depois da tela fica visível
			100.0f                                       // Distância do Far plane:  Conceitualmente é a distância máxima que o "olho" consegue ver
		);
		
		shaderProgram1.setUniform("view", view);
		shaderProgram1.setUniform("projection", projection);
		for(int i =0; i < 10; i++)
		{
			auto model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			auto angle = 20.0f * (i+1);
			if(i % 3 == 0){
				model = glm::rotate(model,glm::radians(angle)*time,glm::vec3(1.0f,0.3f,0.5f));

			} else 
			{
				model = glm::rotate(model,glm::radians(angle),glm::vec3(1.0f,0.3f,0.5f));
			}

			shaderProgram1.setUniform("model", model);
			glDrawArrays(GL_TRIANGLES,0,36);
		}



		
		// glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers( window );
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
