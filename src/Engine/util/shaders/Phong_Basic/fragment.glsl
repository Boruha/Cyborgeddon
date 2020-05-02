#version 450 core

/*------  CONST    -------*/ 
const float att_Linear = 0.0017;
const float att_quadra = 0.0002;

/*------  STRUCTS  -------*/ 
struct Light
{
    vec3  position;
    vec3  diffuse;
    vec3  specular;
    float far;
    samplerCube shadow_map;  //2+
};

/*------  UNIFORMS  ------*/ 
uniform Light[7]  lights;
uniform int       lights_index; 

uniform vec3      camera_pos;
uniform vec3      l_Ambient;

uniform sampler2D texture_diffuse0; //0
uniform sampler2D texture_normal0;  //1
uniform bool      have_normal;


/*-------  INPUTS  -------*/
in vec2 TexCoords;
in vec3 FragPos;
in vec3 FragNormal;

/*-------  OUTPUTS  ------*/
layout (location = 0) out vec4 FragColor;

/*-------  FUNTIONS ------*/
float ShadowCalculation(int index)
{
    vec3 vec_obj2light = FragPos - lights[index].position;
    float closeDepth   = (texture(lights[index].shadow_map, vec_obj2light).r) * lights[index].far;
    float currentDepth = length(vec_obj2light);

    float bias   = 0.5;  //max(0.5 * (1.0 - dot(vecNormal, vecLight)), 0.005);
    float shadow = currentDepth - bias > closeDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    //GENERAL
    vec3 vec_tex    = vec3(texture(texture_diffuse0, TexCoords));
    vec3 vec_normal = normalize(FragNormal);
    vec3 f_amb      = l_Ambient * vec_tex;
    const int conta = 2;
    //DIFFUSE
    vec3  vecObj2Light  = vec3(0.0);
    float distObj2Light = 0.0;
    float cos_light     = 0.0;
    vec3  f_diff        = vec3(0.0);
    //SPECULAR
    vec3 vec_view = vec3(0.0); 
    vec3 vec_spec = vec3(0.0);
    vec3 vec_tex2 = vec3(0.0);
    vec3 f_spec   = vec3(0.0);
    //FX
    float attenuation = 0.0;
    float shadow      = 0.0;
    //FINAL
    vec3  phong   = vec3(0.0);
    //por algún motivo el loop no va si itera más de una vez.
    for(int i=1; i<1; i++)
    {
        vecObj2Light  = lights[i].position - FragPos;
        distObj2Light = length(vecObj2Light);

        if(distObj2Light < 300)
        {
            vecObj2Light = normalize(vecObj2Light);
            cos_light    = max(dot(vecObj2Light, vec_normal), 0.0);
            f_diff       = lights[i].diffuse * cos_light * vec_tex;

            attenuation  = 1/(1 + (att_Linear * distObj2Light) + (att_quadra * distObj2Light * distObj2Light));
            shadow       = ShadowCalculation(i);

            if(have_normal)
            {
                vec_view  = normalize(camera_pos - FragPos); 
                vec_spec  = reflect(-vecObj2Light, vec_normal);
                vec_tex2  = vec3(texture(texture_normal0, TexCoords));

                f_spec    = lights[i].specular * pow(max(dot(vec_spec, vec_view), 0.0), 0.2) * vec_tex2;
            }
        }
        phong += (f_diff + f_spec) * (1.0 - shadow) * attenuation;
    }
    
    phong    += f_amb;
    FragColor = vec4(phong, 1.0);
}
