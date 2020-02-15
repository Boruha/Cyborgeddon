#pragma once

#include <src/Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>

struct Light : IEntity {
    ~Light() override = default;

    void render(const mat4x4&) const override {  }

    void setValue(const vec3& newValue) { value = newValue; };
    [[nodiscard]] const vec3& getValue() const { return value; }

    private:

        vec3 value { };
};