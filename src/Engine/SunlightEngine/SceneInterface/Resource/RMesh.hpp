#pragma once

#include <Engine/SunlightEngine/SceneInterface/Resource/IResource.hpp>

struct RMesh : IResource {
    ~RMesh() override = default;

    void loadResource(const char * const path) override {

    }

    private :
        float * vertex, normal, texture;
        float * v_tri, n_tri, t_tri; // vertex triangles, normal triangles, texture triangles
        unsigned long triangles { 0 };
};