#pragma once

#include <src/Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>

struct Mesh : IEntity {
    ~Mesh() override = default;

    void render(const mat4x4& m) const override {

    }

    void setMesh(const char * const path) {

    }

    private :
        const void * mesh { nullptr };
};