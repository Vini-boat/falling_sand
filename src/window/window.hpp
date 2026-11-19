#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0,0,w,h);
}

GLFWwindow* initWindow(
    int width,int height, 
    const char* title, 
    GLFWframebuffersizefun resize_callback = framebuffer_size_callback
);