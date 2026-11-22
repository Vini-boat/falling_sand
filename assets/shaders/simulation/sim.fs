#version 330 core 

in  vec2 uv; 

uniform usampler2D tela;
uniform sampler1D paleta;

out vec4 FragColor;

void main()
{
    FragColor = texture(tela,uv);
}