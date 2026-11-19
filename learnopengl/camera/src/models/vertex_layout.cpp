#include "vertex_layout.hpp"
#include <glad/glad.h>
#include "types.hpp"
#include <assert.h>



VertexLayout::VertexLayout(){    
    glGenVertexArrays(1, &ID);
}

void VertexLayout::setLayout(
    const std::vector<VertexLayoutAttributes>& attrs
){
    assert(attrs.size() <= 16 && "Numero máximo de atributos OpenGL?");
    int stride = 0; for(auto attr : attrs) stride += attr.size;
    int offset = 0;
    for(int i = 0; i < attrs.size(); i++){
        auto attr = attrs[i];
        if(attr.qtdComponents == 0) continue;

        glVertexAttribPointer( 
			i, 						   // Indice do atributo a ser configurado
			attr.qtdComponents, 	   // Número de componentes, tem que ser 1,2,3 ou 4 (pq vai virar um vec)
			attr.dataType,			   // Data type de cada componenete
			attr.normalize, 		   // fala se o dado tem que ser normalizado, entre -1 e 1 para signed e 0 e 1 para unsigned
			                           //   só vale pra GL_BYTE E GL_UNSIGNED_BYTE
			stride,				       // stride, o offset em bytes entre o mesmo atributo em vértices consecutivos
			(void*)(uintptr_t)(offset) // offset inicial para o começo do atributo
		);
        glEnableVertexAttribArray(i);
        offset += attr.size;
    }
}

void VertexLayout::bind(){
    glBindVertexArray(ID);
}
