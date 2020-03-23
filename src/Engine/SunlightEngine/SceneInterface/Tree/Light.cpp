#include <src/Engine/SunlightEngine/SceneInterface/Tree/Light.hpp>

#include <Engine/util/shaders/Shader.hpp>
#include <iostream>
void Light::render(const glm::mat4& m, Shader shader) 
{
    //std::cout << "NIGGA?????? --  " << ambient << "\n";
    shader.vec3Uniform("light.ambient", ambient);
    //shader.floatUniform("diffuse" , diffuse);
    //shader.floatUniform("specular", specular);
}
