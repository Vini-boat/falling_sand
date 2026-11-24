#include "shader.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "utils/utils.hpp"
#include "types.hpp"
#include "logger/logger.hpp"
#include <format>

inline Logger _log;

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
                int size = 0;
                glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &size);
                std::string msg;
                msg.resize(size);
                glGetShaderInfoLog(vertexShader, size, NULL, msg.data());
                _log.error(std::format("SHADER: error compiling file {}: \n{}",vertexShaderPath, msg));
            }
        }
        _log.info(std::format("SHADER: compiled {}",vertexShaderPath));
        
        const char* f_src = fragmentShaderSourceCode.c_str();
        glShaderSource( fragmentShader, 1, &f_src, NULL);
        glCompileShader( fragmentShader);
        {
            int ok;
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ok);
            if(!ok){
                int size = 0;
                glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &size);
                std::string msg;
                msg.resize(size);
                glGetShaderInfoLog(fragmentShader, size, NULL, msg.data());
                _log.error(std::format("SHADER: error compiling file {}: \n{}",fragmentShaderPath, msg));
            }
        }
        _log.info(std::format("SHADER: compiled {}",fragmentShaderPath));
        
        
        ID = glCreateProgram();
        _log.info(std::format("SHADER: created program {}", ID));
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);
        {
            int ok;
            glGetProgramiv(ID, GL_LINK_STATUS, &ok);
            if(!ok){
                int size = 0;
                glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &size);
                std::string msg; msg.resize(size);
                glGetProgramInfoLog(ID,size,NULL,msg.data());
                _log.error(std::format("SHADER: linking program {}: \n{}",ID,msg));
            }
        }
        _log.info(std::format("SHADER: ID {}, linked shaders", ID));

    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderProgram::use()
{
    glUseProgram(ID);
}

void ShaderProgram::setUniform(
    const std::string& name, 
    bool value
){
    glUniform1i(
        glGetUniformLocation(ID,name.c_str()),
        (int)value
    );
}

void ShaderProgram::setUniform(
    const std::string& name, 
    i32 value
){
    glUniform1i(
        glGetUniformLocation(ID,name.c_str()),
        value
    );

}

void ShaderProgram::setUniform(
    const std::string& name, 
    f32 value
){
    glUniform1f(
        glGetUniformLocation(ID,name.c_str()),
        value
    );
}

void ShaderProgram::setUniform(
    const std::string& name, 
    glm::mat4 value
){
    glUniformMatrix4fv(
        glGetUniformLocation(ID,name.c_str()),
        1,                      // quantidade de matrizes
        GL_FALSE,               // Se deve transpor a matriz
        glm::value_ptr(value)
    );
}
