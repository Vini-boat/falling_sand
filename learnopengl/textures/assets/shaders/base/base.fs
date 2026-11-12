#version 330 core 

uniform float uColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.0, uColor, 0.0, 1.0);
}