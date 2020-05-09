#version 330 core

/*------  CONST    -------*/ 
//const float att_Linear = 0.0017;
const float att_Linear = 0.01;
const float att_quadra = 0.0002;

/*------  STRUCTS  -------*/ 
struct Light
{
    vec3  position;
    vec3  diffuse;
    vec3  specular;
    float far;
    samplerCubeShadow shadow_map;  //2+    -> trabajar con ComputeShadowFactor(vec3, Light);
    //samplerCube       shadow_map;  //2+  -> trabajar con ShadowCalculation(vec3, Light);

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
float ComputeShadowFactor(vec3 frag_pos, Light currentLight)
{   
    /*
        PARA ESTA FUNCIÓN:
        - La variable de la textura de sombra es 'samplerCubeShadow' 
          por el uso particular de la función texture.
        - Ahora el vector Luz - Fragmento lo es unitario para comparar con el valor [0,1] almacenado.
        - Los parametos de texture son:
            texture(
                samplerCubeShadow sampler,-> la textura
                vec4              vector, -> vector que usamos siempre, see SceneManager.cpp 'genShadowTexture()' 
                                             (la cuarta cmp es el valor con el que compara)
                [float           bias]    -> es un bias distinto al usado a mano, tiene que ver con el LoD.
            )
    */
    vec3 VecLightToObj  = frag_pos - currentLight.position;
    float distObj2Light = length(VecLightToObj) / currentLight.far;

    return texture(currentLight.shadow_map, vec4(VecLightToObj, distObj2Light - 0.005));
}


float ShadowCalculation(vec3 frag_pos, Light currentLight)
{
    /*
        PARA ESTA FUNCIÓN:
            - La variable de la textura de sombra debe ser 'samplerCube' 
              por el uso particular de la función texture.
            - Ahora el vector Luz - Fragmento [0, 299.9].
            - Los parametos de texture son:
                texture(
                    samplerCube sampler,-> la textura
                    vec3        vector, -> vector que usamos siempre
                )
            - El contenido de la textura [0,1] lo transformamos en [0, 300];
            - La comparativa es manual en este caso.
    */
    vec3 fragToLight   = frag_pos - currentLight.position;  
    float closestDepth = texture(currentLight.shadow_map, fragToLight).r;
    closestDepth      *= currentLight.far;
    float currentDepth = length(fragToLight);

    float bias   = 0.5; 

    return ((currentDepth -  bias) > closestDepth) ? 1.0 : 0.0;
}

void main()
{
    //GENERAL
    vec3 vec_tex    = vec3(texture(texture_diffuse0, TexCoords));
    vec3 vec_normal = normalize(FragNormal);
    vec3 phong      = l_Ambient * vec_tex;
    //vec3 phong      = vec3(1.0); -> test sombra solo;

    for(int i=0; i<lights_index; ++i)
    {
        vec3  vecObj2Light  = lights[i].position - FragPos;
        float distObj2Light = length(vecObj2Light);

        if(distObj2Light < 300)
        {
            float attenuation  = 1/(1 + (att_Linear * distObj2Light) + (att_quadra * distObj2Light * distObj2Light));

            vecObj2Light    = normalize(vecObj2Light);
            float cos_light = max(dot(vecObj2Light, vec_normal), 0.0);
            vec3  f_diff    = lights[i].diffuse * cos_light * vec_tex;
            phong          += f_diff * attenuation;

            if(have_normal)
            {
                vec3 vec_view  = normalize(camera_pos - FragPos); 
                vec3 vec_spec  = reflect(-vecObj2Light, vec_normal);
                vec3 vec_tex2  = vec3(texture(texture_normal0, TexCoords));
                vec3 f_spec    = lights[i].specular * pow(max(dot(vec_spec, vec_view), 0.0), 0.2) * vec_tex2;
                phong         += f_spec * attenuation;
            }
            //float shadow = ComputeShadowFactor(FragPos, lights[i]); -> para samplerCubeShadow
            //float shadow = ShadowCalculation(FragPos, lights[i]);   -> para samplerCube
            //phong *= shadow;
        }
    }

    FragColor = vec4(phong, 1.0);
}
