#version 460 core

layout (location = 0) in vec3 aPos;     // posicion en el mundo
layout (location = 1) in vec3 aColor;   // color que le pasaremos al fragment

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);
    vertexColor = vec4(aColor, 1);
}
