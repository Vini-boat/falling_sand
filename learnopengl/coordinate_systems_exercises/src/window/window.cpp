#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.hpp"
#include "assert.h"

static void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0,0,w,h);
}

GLFWwindow* initWindow(int width,int height, const char* title)
{
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width,height,title,NULL,NULL);
    assert(window != NULL);
    glfwMakeContextCurrent(window);
    int ok = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    assert(ok);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    return window;
}