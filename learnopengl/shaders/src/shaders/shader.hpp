#pragma once

#include <glad/glad.h>
#include "types.hpp"

using namespace std;

#include <string>
#include <sstream>
#include <iostream>

struct ShaderProgram {
    glID ID;

    // TODO: Daria pra fazer um builder pattern mas vai ir assim por enquanto
    ShaderProgram(
        const char* vertexShaderPath,
        const char* fragmentShaderPath
    );

    void use();

    // TODO: assim ele ta buscando a localização do uniform em toda a call
    //       daria pra ter armazenado a localização do uniform e só setar ela
    //       mas daí teria que salver uma referencia para o uniform de alguma forma...
    void setUVb(
        const string& name, 
        bool value
    );

    void setUVi(
        const string& name, 
        i32 value
    );
    
    void setUVf(
        const string& name, 
        f32 value
    );


};