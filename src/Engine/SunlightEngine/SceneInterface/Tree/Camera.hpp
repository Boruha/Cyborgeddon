#pragma once

#include <src/Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>

struct Camera : IEntity {
    ~Camera() override = default;

    void render(const mat4x4&) const override { };

    void setPerspective(const float _near, const float _far, const bool perspective) {
        near = _near;
        far  = _far;
        isPerspective = perspective;
    }

    private :
        bool isPerspective { true };
        float near { 0 };
        float far  { 1 };
};