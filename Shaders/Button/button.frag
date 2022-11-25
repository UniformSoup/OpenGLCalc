#version 460 core

uniform sampler2D tex;
uniform vec3 buttonColour;

out vec4 colour;

void main()
{
    colour = vec4(buttonColour, 1.f);
}