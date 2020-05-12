#pragma once

#include <Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>

struct Light : IEntity {

    explicit Light(const glm::vec3& diff, const glm::vec3& spe) : diffuse(diff), specular(spe) {}
    ~Light() override = default;

    void render(const glm::mat4& m, Shader shader, bool visualShader) override;

    [[nodiscard]] const std::size_t getID() const { return ID; }

    glm::vec3 direccion;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::mat4 projection;
    glm::mat4 m_VP;

    float near { 1.f };
    float far  { 600 };

    unsigned FBO;
    unsigned shadow_map;

private:
    std::size_t ID { nextID++ };
	inline static std::size_t nextID { 0 };
};