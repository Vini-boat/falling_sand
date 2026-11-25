#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <math.h>
#include <random>
#include <type_traits>

#include "glm/ext/vector_uint4_sized.hpp"
#include "textures/image2d.hpp"
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


enum Type : u8 {
	AIR = 0,
	SAND,
	WATER,
	STONE,
	COUNT
};

struct Cell {
	Type type;
	u8 flags : 5;
	u8 shade : 3;
	u16 data;
};
// Necessários pra não quebrar o layout dos dados
static_assert(sizeof(Cell) == 4);
static_assert(is_trivially_copyable<Cell>());
	
constexpr int W = 64;
constexpr int H = 64;
Cell colors[W*H];
vector<bool> parity(W*H, 0);



int main() 
{
	for(auto& c : colors) c.type = AIR;

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
		.format = {
			.internal = GL_R32UI,
			.format   = GL_RED_INTEGER,
			.type	  = GL_UNSIGNED_INT
		},
		.gen_mipmaps = false
	});

	Image2D palette_image(ASSETS_DIR"/textures/palette.png");
	Texture2D palette(palette_image, {
		.min_filter = FilterMode::NEAREST,
		.mag_filter = FilterMode::NEAREST,	
		.gen_mipmaps = false
	});

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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
				colors[y * W + x].type = Type::SAND;
				colors[y * W + x].shade = rng() & 7;
			}
			if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)){
				int x = (int)mouse_pos.x / 8; 
				int y = (int)mouse_pos.y / 8;
				colors[y * W + x].type = Type::WATER;
				colors[y * W + x].shade = rng() & 7;
			}
			if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3)){
				int x = (int)mouse_pos.x / 8; 
				int y = (int)mouse_pos.y / 8;
				colors[y * W + x].type = Type::AIR;
			}
			if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_4)){
				int x = (int)mouse_pos.x / 8; 
				int y = (int)mouse_pos.y / 8;
				colors[y * W + x].type = Type::STONE;
				colors[y * W + x].shade = rng() & 7;
			}
			for(int x = 0; x < W; x++){
				for(int y = 0; y < H; y++){
					int i = y * W + x;
					if(parity[i] != p) continue;
					parity[i] = !p;
					switch (colors[i].type) {
						case COUNT:
						case AIR:
							break;
						case SAND: {
							const int dxs[] = {0,-1,1};
							for(int dx = 0; dx < 3; dx++){
								int ny = y+1;
								int nx = x + dxs[dx] * rng_sign();
		
								if(ny == H) continue;
								if(nx == W) continue;
								if(nx == -1) continue; 
		
								int down = ny * W + nx;
								
								if(colors[down].type == SAND) continue;
								if(colors[down].type == STONE) continue;
								
								swap(colors[i],colors[down]);
								parity.swap(parity[i],parity[down]);
								break;
							}
							break;

						}
						case WATER:{
							const int dxs[] = {0,-1, 1,-1, 1};
							const int dys[] = {1, 1, 1, 0, 0};
							for(int d = 0; d < 5; d++){
								int ny = y + dys[d];
								int nx = x + dxs[d] * rng_sign();
		
								if(ny == H) continue;
								if(nx == W) continue;
								if(nx == -1) continue; 
		
								int down = ny * W + nx;
								
								if(colors[down].type != AIR) continue;
								
								swap(colors[i],colors[down]);
								parity.swap(parity[i],parity[down]);
								break;
							}
							break;
						}
						case STONE: {
							
							break;
						}
					}
				}
			}
			last_update = glfwGetTime();
		}
		
		
		// Render
		_log.glCheck("");
		shaderProgram1.use();
		_log.glCheck("");
		
		tela.bind(0);
		_log.glCheck("");
		tela.update(colors, W, H, 0, 0);
		_log.glCheck("");
		palette.bind(1);
		_log.glCheck("");
		
		shaderProgram1.setUniform("color", 0);
		_log.glCheck("");
		shaderProgram1.setUniform("palette", 1);
		_log.glCheck("");
		

		glClearColor(0.529f, 0.808f, 0.922f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT);
		
		
		
		VAO.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers( window );
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
