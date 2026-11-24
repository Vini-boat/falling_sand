#include "texture2d.hpp"
#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>
#include "types.hpp"

Texture2D::Texture2D(
    const void* data,
	int width, int height,
    const Texture2DParameters& params
){
    glGenTextures(1, &ID);

    glBindTexture(GL_TEXTURE_2D, ID);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.min_filter);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.mag_filter); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrap_s);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrap_t);
        // força a ignorar o carregamento paralelo, resolve um problema com imagens não divisíveis por 4 e a stb
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
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
		if(params.gen_mipmaps) glGenerateMipmap(GL_TEXTURE_2D);  
        
    }
    glBindTexture(GL_TEXTURE_2D, 0);
};

void Texture2D::update(
	const void *data, 
	int width, int height, 
	int offset_x, int offset_y
){
	glTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		offset_x,
		offset_y,
		width,
		height,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data
	);
};

void Texture2D::bind(unsigned int texture_unit){
	glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, ID);
};