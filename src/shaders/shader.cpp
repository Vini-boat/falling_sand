#include "shader.hpp"
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "utils/utils.hpp"
#include "types.hpp"


constexpr int LOG_BUFF_SIZE = 512;

ShaderProgram::ShaderProgram(
    const char* vertexShaderPath,
    const char* fragmentShaderPath
){
    std::string vertexShaderSourceCode = getFileContent(vertexShaderPath);
    std::string fragmentShaderSourceCode = getFileContent(fragmentShaderPath);
    
    glID vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glID fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    {
        const char* v_src = vertexShaderSourceCode.c_str();
        glShaderSource( vertexShader, 1, &v_src, NULL);
        glCompileShader( vertexShader);
        {
            int ok;
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ok);
            if(!ok){
                char log[LOG_BUFF_SIZE];
                glGetShaderInfoLog(vertexShader, LOG_BUFF_SIZE, NULL, log);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
			    std::cout << log << std::endl;
            }
        }
        
        const char* f_src = fragmentShaderSourceCode.c_str();
        glShaderSource( fragmentShader, 1, &f_src, NULL);
        glCompileShader( fragmentShader);
        {
            int ok;
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ok);
            if(!ok){
                char log[LOG_BUFF_SIZE];
                glGetShaderInfoLog(fragmentShader, LOG_BUFF_SIZE, NULL, log);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl;
			    std::cout << log << std::endl;
            }
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);
        {
            int ok;
            glGetProgramiv(ID, GL_LINK_STATUS, &ok);
            if(!ok){
                char log[LOG_BUFF_SIZE];
                glGetProgramInfoLog(ID,LOG_BUFF_SIZE,NULL,log);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl;
			    std::cout << log << std::endl;
            }
        }
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderProgram::use()
{
    glUseProgram(ID);
}

void ShaderProgram::setUVb(
    const std::string& name, 
    bool value
){
    glUniform1i(
        glGetUniformLocation(ID,name.c_str()),
        (int)value
    );
}

void ShaderProgram::setUVi(
    const std::string& name, 
    i32 value
){
    glUniform1i(
        glGetUniformLocation(ID,name.c_str()),
        value
    );

}

void ShaderProgram::setUVf(
    const std::string& name, 
    f32 value
){
    glUniform1f(
        glGetUniformLocation(ID,name.c_str()),
        value
    );
}
