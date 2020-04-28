#pragma once

#include <Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>

struct Light : IEntity {

    explicit Light(const glm::vec3& diff, const glm::vec3& spe) : diffuse(diff), specular(spe) {}
    ~Light() override = default;

    void render(const glm::mat4& m, Shader shader, bool visualShader) override;

    void setDiffuse(const glm::vec3& nDif)  { diffuse  = nDif; }
    void setSpecular(const glm::vec3& nSpe) { specular = nSpe; }
    void setProjection(const glm::mat4& mP) { projection = mP; }
    void setVProject_m(const glm::mat4& vP) { viewProj_m = vP; }

    [[nodiscard]] const glm::vec3&  getDiffuse()    const { return diffuse;    }
    [[nodiscard]] const glm::vec3&  getSpecular()   const { return specular;   }
    [[nodiscard]] const glm::mat4&  getProjection() const { return projection; }
    [[nodiscard]] const glm::mat4&  getVProject_m() const { return viewProj_m; }
    [[nodiscard]] const std::size_t getID()         const { return ID; }

private:
    glm::vec3 diffuse        { 0.f };
    glm::vec3 specular       { 0.f };
    glm::mat4 projection     {  1  };
    glm::mat4 viewProj_m     {  1  };

    std::size_t ID { nextID++ };
	inline static std::size_t nextID { 0 };
};