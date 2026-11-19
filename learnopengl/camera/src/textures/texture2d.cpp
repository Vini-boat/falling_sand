#include "texture2d.hpp"
#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>
#include "types.hpp"

Texture2D::Texture2D(
    const char* path,
    const Texture2DParameters& params
){
    glGenTextures(1, &ID);

    glBindTexture(GL_TEXTURE_2D, ID);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.min_filter);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.mag_filter); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrap_s);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrap_t);
        
        stbi_set_flip_vertically_on_load(params.flip_y);

        int width, height, numberChannels; 
		unsigned char* data = stbi_load(
			path,
			&width, &height, &numberChannels, 4
		);
        // força a ignorar o carregamento paralelo, resolve um problema com imagens não divisíveis por 4 e a stb
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
        if(data){
			// Carrega a textura no GL_TEXTURE_2D bindado
			glTexImage2D(
				GL_TEXTURE_2D,
				0,		  
				GL_RGBA,		  // to fixando tudo em RGBA, se só tiver RGB vai ir um alpha de 255
				width, height,    
				0,
				GL_RGBA,     	  
				GL_UNSIGNED_BYTE,  
				data              
			);
			// Gera os mipmaps
			// Texturas com menos detalhes para evitar problemas quando elas são reduzidas
			glGenerateMipmap(GL_TEXTURE_2D);  
		}
		else
		{
			std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD" << std::endl;
			std::cout << path << std::endl; 
			std::cout << stbi_failure_reason() << std::endl;
		}
		stbi_image_free(data);
        
    }
    glBindTexture(GL_TEXTURE_2D, 0);
};

void Texture2D::bind(){
    glBindTexture(GL_TEXTURE_2D, ID);
}