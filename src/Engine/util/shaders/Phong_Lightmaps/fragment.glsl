#version 450 core

/*------  STRUCTS  -------*/ 
struct Light
{
    vec3  position;
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
};

/*------  UNIFORMS  ------*/ 
//Light
uniform Light     light;
//Textures
uniform sampler2D texture_diffuse0;
uniform sampler2D texture_normal0;
uniform bool      has_normal;
/*-------  INPUTS  -------*/
in vec2 TexCoords;
in vec3 view_Pos;
in vec3 view_Normal;

/*-------  OUTPUTS  ------*/
layout (location = 0) out vec4 FragColor;



void main() {
    /* GENERAL  */
    vec3 vec_tex = vec3(texture(texture_diffuse0, TexCoords));

    /* AMBIENT  */
    vec3 f_amb   = light.ambient * vec_tex;
    
    /* DIFFUSE  */
    vec3 vec_normal    = normalize(view_Normal);
    vec3 vec_obj_light = normalize(light.position - view_Pos);

    vec3 f_diff        = light.diffuse * max(dot(vec_obj_light, vec_normal), 0.0) * vec_tex;

    /* SPECULAR */
    vec3 f_spec = vec3(0);

    if(has_normal)
    {
        vec3 vec_view = normalize(-view_Pos); //Camera in 0.0
        vec3 vec_spec = reflect(-vec_obj_light, vec_normal);
        vec3 vec_tex2 = vec3(texture(texture_normal0, TexCoords));

        f_spec        = light.specular * pow(max(dot(vec_spec, vec_view), 0.0), 0.2) * vec_tex2;
    }
    /* TOTAL    */
    vec3 phong  = f_amb + f_diff + f_spec;

    FragColor   = vec4(phong, 0.0); 
}
