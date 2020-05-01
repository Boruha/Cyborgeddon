#version 330 core

/*-------  INPUTS  ------*/
in vec4 FragPos;

/*-------  UNIFORMS  ------*/
uniform vec3  lightPos;
uniform float far;

void main() 
{
    gl_FragDepth = length(FragPos.xyz - lightPos) / far;
}
