#version 460 core

in vec2 TextureCoords;
uniform sampler2D tex;
uniform vec4 textColour = { 1, 0, 0, 1 };

out vec4 colour;

void main()
{
    colour =  vec4(textColour.xyz, textColour.w * texture(tex, TextureCoords).r);
}