#version 330 core

/*------  CONST    -------*/ 
const float att_Linear = 0.0017;
const float att_quadra = 0.0002;

/*------  STRUCTS  -------*/ 
struct Light
{
    vec3 position;
    vec3 diffuse;
    vec3 specular;
    mat4 m_LightVP;
    sampler2D shadow_map;  //2+
};

/*------  UNIFORMS  ------*/ 
uniform Light[7]  lights;
uniform int       light_index; 

uniform vec3      camera_pos;
uniform vec3      l_Ambient;

uniform sampler2D texture_diffuse0; //0
uniform sampler2D texture_normal0;  //1
uniform bool      have_normal;


/*-------  INPUTS  -------*/
in vec2 TexCoords;
in vec3 cameraSpace_pos;
in vec3 cameraSpace_normal;

/*-------  OUTPUTS  ------*/
layout (location = 0) out vec4 FragColor;

/*-------  FUNTIONS ------*/
float ShadowCalculation(float cos_light, int index){
    vec4 lightSpace_pos = lights[index].m_LightVP * vec4(cameraSpace_pos, 1.0);
    
    vec3 projCoords = lightSpace_pos.xyz / lightSpace_pos.w;  
    projCoords = projCoords * 0.5 + 0.5;              
              
    float closestDepth = texture(lights[index].shadow_map, projCoords.xy).r;              
    float bias = 0.005; //max(0.05 * (1.0 - cos_light), 0.005);
    float shadow = projCoords.z - bias > closestDepth  ? 1.0 : 0.0;              

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main() {
    vec3 vec_tex       = vec3(texture(texture_diffuse0, TexCoords));
    vec3 vec_normal    = normalize(cameraSpace_normal);
    vec3 f_amb         = l_Ambient * vec_tex;
    vec3 phong         = vec3(0);
    
    for(int i = 0; i<light_index; ++i)
    {
        vec3  vec_obj_light  = lights[i].position - cameraSpace_pos;
        float dist_obj_light = length(vec_obj_light);
        
        if(dist_obj_light < 300)
        {
            float attenuation = 1/(1 + (att_Linear * dist_obj_light) + (att_quadra * dist_obj_light * dist_obj_light));
            vec_obj_light     = normalize(vec_obj_light);
            float cos_light   = max(dot(vec_obj_light, vec_normal), 0.0);
            vec3 f_diff       = lights[i].diffuse * cos_light * vec_tex;
            vec3 f_spec       = vec3(0);

            if(have_normal)
            {
                vec3 vec_view  = normalize(camera_pos - cameraSpace_pos); 
                vec3 vec_spec  = reflect(-vec_obj_light, vec_normal);
                vec3 vec_tex2  = vec3(texture(texture_normal0, TexCoords));

                f_spec         = lights[i].specular * pow(max(dot(vec_spec, vec_view), 0.0), 0.2) * vec_tex2;
            }

            float shadow = ShadowCalculation(cos_light, i);
            phong       += (1.0 - shadow) * (f_diff + f_spec) * attenuation;
        }
        
    }
    phong    += f_amb;
    FragColor = vec4(phong, 1.0);
}
