#version 460 core 
layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 inTextureCoords;
uniform mat4 projection;

out vec2 TextureCoords;

void main()
{
    gl_Position = projection * vec4(Position.xy, 0.0, 1.0);
    TextureCoords = inTextureCoords;
}