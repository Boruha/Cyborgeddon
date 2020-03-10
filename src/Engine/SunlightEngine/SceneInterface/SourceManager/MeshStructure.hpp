#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal_vec;
    glm::vec2 uv_tex_coord;

};

struct Texture
{
    unsigned int id;
    std::string type;
};

