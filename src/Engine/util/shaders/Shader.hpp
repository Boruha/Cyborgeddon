#pragma once

#include <Engine/util/glad/glad.h>
#include <string>
#include <glm/glm.hpp>

constexpr unsigned SHADER_TYPES = 3;

class Shader {
    unsigned ID { 0 };

public:

    explicit Shader(const std::string_view path [ SHADER_TYPES ]);

    void enable() const;

    void boolUniform(std::string_view name, bool value)   const;
    void intUniform(std::string_view name, int value)     const;
    void floatUniform(std::string_view name, float value) const;


    void vec2Uniform(std::string_view, const glm::vec2 &) const;
    void vec3Uniform(std::string_view, const glm::vec3 &) const;
    void vec4Uniform(std::string_view, const glm::vec4 &) const;

    void mat2Uniform(std::string_view, const glm::mat2 &) const;
    void mat3Uniform(std::string_view, const glm::mat3 &) const;
    void mat4Uniform(std::string_view, const glm::mat4 &) const;

    [[nodiscard]] unsigned getID() const {
        return ID;
    }
};