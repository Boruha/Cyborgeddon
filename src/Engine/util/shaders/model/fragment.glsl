#version 460 core

out vec4 FragmentColor;     // color del fragment para calcular el pixel de pantalla

in vec2 TexCoords;      // coordenadas de textura que recibimos del vertex

uniform sampler2D texture_diffuse0;  // textura que corresponde a este fragmento

void main() {
    FragmentColor =  texture(texture_diffuse0, TexCoords);
}
