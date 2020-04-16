#version 450 core

/*------  CONST    -------*/ 
const float att_Linear = 0.0013;
const float att_quadra = 0.0001;

/*------  STRUCTS  -------*/ 
struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //bool used;
};

/*------  UNIFORMS  ------*/ 
uniform Light[7]  lights;
uniform int       light_index; 

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
    vec3 vec_tex       = vec3(texture(texture_diffuse0, TexCoords));
    vec3 vec_normal    = normalize(view_Normal);
    vec3 phong         = vec3(0);
    
    for(int i = 0; i<light_index; ++i)
    {
        vec3  vec_obj_light  = lights[i].position - view_Pos;
        float dist_obj_light = length(vec_obj_light);
        
        if(dist_obj_light < 300)
        {
            vec_obj_light     = normalize(vec_obj_light);
            float attenuation = 1/(1 + (att_Linear * dist_obj_light) + (att_quadra * dist_obj_light * dist_obj_light));

            vec3 f_amb  = lights[i].ambient * vec_tex;
            vec3 f_diff = lights[i].diffuse * max(dot(vec_obj_light, vec_normal), 0.0) * vec_tex;
            vec3 f_spec = vec3(0);

            if(has_normal)
            {
                vec3 vec_view  = normalize(-view_Pos); //Camera in 0.0
                vec3 vec_spec  = reflect(-vec_obj_light, vec_normal);
                vec3 vec_tex2  = vec3(texture(texture_normal0, TexCoords));

                f_spec         = lights[i].specular * pow(max(dot(vec_spec, vec_view), 0.0), 0.2) * vec_tex2;
            }

            phong += (f_amb + f_diff + f_spec) * attenuation;
        }
    }
    
    FragColor = vec4(phong, 0.0); 
}
