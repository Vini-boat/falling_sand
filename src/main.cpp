#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <math.h>
#include <random>

#include "glm/ext/vector_uint4_sized.hpp"
#include "types.hpp"
#include "window/window.hpp"
#include "models/vertex_layout.hpp"
#include "shaders/shader.hpp"
#include "textures/texture2d.hpp"
#include "camera/camera.hpp"
#include "logger/logger.hpp"

std::mt19937 rng(std::random_device{}());

using namespace std;

constexpr int    WINDOW_WIDTH  = 512;
constexpr int    WINDOW_HEIGHT = 512;
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
unsigned int indices[] = {
	0,1,2,
	2,3,0
};
	
constexpr int W = 64;
constexpr int H = 64;
glm::u8vec4 colors[W*H];
vector<bool> parity(W*H, 0);

constexpr auto SAND = glm::u8vec4(194, 178, 128,255);
constexpr auto AIR = glm::u8vec4(130, 200, 229,255);
constexpr auto WATER = glm::u8vec4(14, 135, 204,255);


int rng_sign(){
	bool sign = rng() & 1;
	if(sign){
		return -1;
	} else{
		return 1;
	}
}


glm::vec2 mouse_pos;
void mouse_callback(GLFWwindow* window, double posX, double posY)
{
	mouse_pos = {posX,posY};
}

void error_callback(int error, const char* description) {
    _log.error(format("GLFW: error code: {}: {}", error, description));
}

int main() 
{
	for(auto& c : colors) c = AIR;

	glfwSetErrorCallback(error_callback);
	GLFWwindow* window = initWindow(WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_NAME);
	glfwSetCursorPosCallback(window, mouse_callback);

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

	Texture2D tela(colors,W,H, {
		.min_filter = FilterMode::NEAREST,
		.mag_filter = FilterMode::NEAREST,
		.gen_mipmaps = false
	});


	const double frame_time = 1.0 / 60.0;
	auto last_update = glfwGetTime();
	unsigned long long tick = 0; 
	while(!glfwWindowShouldClose(window))
	{

		auto time = (float)glfwGetTime();
		dt = time - time_last_frame;
		time_last_frame = time;
		
		processInput(window);
		
		
		
		// update
		double u_dt = glfwGetTime() - last_update; 
		if(u_dt >= frame_time){
			bool p = tick & 1;
			tick++;
			if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)){
				int x = (int)mouse_pos.x / 8; 
				int y = (int)mouse_pos.y / 8;
				colors[y * W + x] = SAND;
			}
			if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)){
				int x = (int)mouse_pos.x / 8; 
				int y = (int)mouse_pos.y / 8;
				colors[y * W + x] = WATER;
			}
			for(int x = 0; x < W; x++){
				for(int y = 0; y < H; y++){
					int i = y * W + x;
					if(parity[i] != p) continue;
					parity[i] = !p;
					if(colors[i] == SAND){

						const int dxs[] = {0,-1,1};
						for(int dx = 0; dx < 3; dx++){
							int ny = y+1;
							int nx = x + dxs[dx] * rng_sign();
	
							if(ny == H) continue;
							if(nx == W) continue;
							if(nx == -1) continue; 
	
							int down = ny * W + nx;
							
							if(colors[down] == SAND) continue;
							
							swap(colors[i],colors[down]);
							parity.swap(parity[i],parity[down]);
							break;
						}
					}
					if(colors[i] == WATER){
						const int dxs[] = {0,-1, 1,-1, 1};
						const int dys[] = {1, 1, 1, 0, 0};
						for(int d = 0; d < 5; d++){
							int ny = y + dys[d];
							int nx = x + dxs[d] * rng_sign();
	
							if(ny == H) continue;
							if(nx == W) continue;
							if(nx == -1) continue; 
	
							int down = ny * W + nx;
							
							if(colors[down] != AIR) continue;
							
							swap(colors[i],colors[down]);
							parity.swap(parity[i],parity[down]);
							break;
						}

					}
					
				}
			}
			last_update = glfwGetTime();
		}
		
		
		// Render
		tela.bind(0);
		tela.update(colors, W, H, 0, 0);
		
		tela.bind(0);
		shaderProgram1.use();
		shaderProgram1.setUniform("color", 0);
		

		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT);
		
		
		
		VAO.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers( window );
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
