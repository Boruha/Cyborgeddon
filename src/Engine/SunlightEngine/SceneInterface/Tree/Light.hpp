#pragma once

#include <Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>

struct Light : IEntity {

    Light() = default;
    Light(const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spe) : ambient(amb), diffuse(diff), specular(spe) {}
    ~Light() override = default;

    void render(const glm::mat4& m, Shader shader) override;

    void setAmbinet(const glm::vec3& nAmb)  { ambient  = nAmb; }
    void setDiffuse(const glm::vec3& nDif)  { diffuse  = nDif; }
    void setSpecular(const glm::vec3& nSpe) { specular = nSpe; }

    [[nodiscard]] const glm::vec3& getAmbient() const { return ambient;  }
    [[nodiscard]] const glm::vec3& getDiffuse() const { return diffuse;  }
    [[nodiscard]] const glm::vec3& getValue()   const { return specular; }

private:
    //Intensity of the different parts of light
    //can be all the same value but maybe we need
    //modify a specific parameter in future ver. .
    glm::vec3 ambient  { 0.f };
    glm::vec3 diffuse  { 0.f };
    glm::vec3 specular { 0.f };
};