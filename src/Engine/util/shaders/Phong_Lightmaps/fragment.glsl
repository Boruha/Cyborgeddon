#version 450 core

/*------  STRUCTS  -------*/ 
struct Light
{
    vec3  position;
    vec3  ambient;
    //float diffuse;
    //float specular;
};

/*------  UNIFORMS  ------*/ 
//Light
uniform Light     light;
//Textures
uniform sampler2D texture_diffuse0;

/*-------  INPUTS  -------*/
in vec2 TexCoords;
in vec3 view_Pos;
in vec3 view_Normal;

/*-------  OUTPUTS  ------*/
layout (location = 0) out vec4 FragColor;



void main() {

    vec3 rand  = light.position * view_Normal * view_Pos;  
    vec3 phong = light.ambient * vec3(texture(texture_diffuse0, TexCoords));

    FragColor  = vec4(phong, 0.0); 
}
