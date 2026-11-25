#version 330 core 

in  vec2 uv; 

uniform usampler2D color;
uniform sampler2D palette;

out vec4 FragColor;

void main()
{
    ivec2 texCoord = ivec2(uv * vec2(textureSize(color, 0)));
    
    uvec4 pixel = texelFetch(color, texCoord, 0);
    
    FragColor = texelFetch(palette, ivec2(pixel.xy),0);
}