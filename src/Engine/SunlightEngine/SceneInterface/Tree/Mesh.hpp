#pragma once

#include <Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>
#include <Engine/SunlightEngine/SceneInterface/Resource/RMesh.hpp>

struct Mesh : IEntity {
    ~Mesh() override = default;

    void render(const mat4x4& m) const override {

    }

    void setMesh(const char * const path) {

    }

    private :
        const RMesh * mesh { nullptr };
};