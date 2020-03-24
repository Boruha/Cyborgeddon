#include <src/Engine/SunlightEngine/SceneInterface/Tree/Light.hpp>

#include <Engine/util/shaders/Shader.hpp>
#include <iostream>
void Light::render(const glm::mat4& m, Shader shader) 
{
    //std::cout << "NIGGA?????? --  " << ambient << "\n";
    shader.vec3Uniform("light.ambient" , ambient);
    shader.vec3Uniform("light.diffuse" , diffuse);
    shader.vec3Uniform("light.specular", specular);
}
