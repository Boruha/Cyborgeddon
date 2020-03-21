#pragma once

#include <Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>

struct Light : IEntity {

    Light() = default;
    Light(float amb, float diff, float spe) : ambient(amb), diffuse(diff), specular(spe) {}
    ~Light() override = default;

    void render(const glm::mat4& m) override { /*TODO: SHADR NEED*/ }

    void setAmbinet(const float nAmb)  { ambient  = nAmb; }
    void setDiffuse(const float nDif)  { diffuse  = nDif; }
    void setSpecular(const float nSpe) { specular = nSpe; }

    [[nodiscard]] const float getAmbient() const { return ambient;  }
    [[nodiscard]] const float getDiffuse() const { return diffuse;  }
    [[nodiscard]] const float getValue()   const { return specular; }

private:
    //Intensity of the different parts of light
    //can be all the same value but maybe we need
    //modify a specific parameter in future ver. .
    float ambient  { 0.f };
    float diffuse  { 0.f };
    float specular { 0.f };
};