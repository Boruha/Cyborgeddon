#version 450 core

layout (location = 0) in vec3 aPos;         // posicion del vertex
layout (location = 1) in vec3 aNormal;      // normal del vertex
layout (location = 2) in vec2 aTexCoords;   // coordenada de textura del vertex

out vec2 TexCoords;     // coordenada de textura que va a recibir el fragment

uniform mat4 model;         // matriz modelo
uniform mat4 view;          // matriz vista
uniform mat4 projection;    // matriz proyeccion

uniform mat4 mvp;

// TODO : calcular mvp antes puede ser interesante para mejorar rendimiento

void main() {
    TexCoords = aTexCoords;
    gl_Position = mvp * vec4(aPos, 1.f);
}
