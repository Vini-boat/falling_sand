#include "texture2d.hpp"
#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>
#include "image2d.hpp"
#include "types.hpp"
#include "logger/logger.hpp"

Texture2D::Texture2D(
    const void* data,
	int width, int height,
    const Texture2DParameters& params
){
	format = params.format;
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
			format.internal,		  // to fixando tudo em RGBA, se só tiver RGB vai ir um alpha de 255
			width, height,    
			0,
			format.format,     	  
			format.type,  
			data              
		);
		_log.glCheck("glTexImage2D");
		// Gera os mipmaps
		// Texturas com menos detalhes para evitar problemas quando elas são reduzidas
		if(params.gen_mipmaps) glGenerateMipmap(GL_TEXTURE_2D);  
        
    }
    glBindTexture(GL_TEXTURE_2D, 0);
	_log.info(std::format("TEXTURE: created {}", ID));
};

Texture2D::Texture2D(
	const Image2D& img,
	const Texture2DParameters& params
) : Texture2D(
		img.data,
		img.width,
		img.height,
		params
){}

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
		format.format,
		format.type,
		data
	);
	_log.glCheck("glTexSubImage2D");
};

void Texture2D::bind(unsigned int texture_unit){
	glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, ID);
};