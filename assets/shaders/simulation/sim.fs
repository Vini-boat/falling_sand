#version 330 core 

in  vec2 uv; 

uniform usampler2D color;
uniform sampler2D palette;

out vec4 FragColor;


struct Cell {
    uint type;
    uint flags;
    uint shade;
    uint data;
};

Cell decode(uint raw){
    Cell c;

    c.type  = raw         & 0xFFu;
    c.flags = (raw >> 8)  & 0x1Fu;
    c.shade = (raw >> 13) & 0x7u;
    c.data  = (raw >> 16) & 0xFFFFu;

    return c;
}




void main()
{
    ivec2 texCoord = ivec2(uv * vec2(textureSize(color, 0)));
    
    uint pixel = texelFetch(color, texCoord, 0).r;
    
    Cell cell = decode(pixel);

    FragColor = texelFetch(palette, ivec2(cell.type,cell.shade),0);
}