#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.hpp"
#include "assert.h"
#include "logger/logger.hpp"

inline Logger _log;

GLFWwindow* initWindow(
    int width,int height, 
    const char* title, 
    GLFWframebuffersizefun resize_callback
){
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width,height,title,NULL,NULL);
    assert(window != NULL);
    _log.info("GLFW: initialized");

    glfwMakeContextCurrent(window);
    int ok = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    assert(ok);
    _log.info("GLAD: initialized");
    
    glfwSetFramebufferSizeCallback(window,resize_callback);
    return window;
}