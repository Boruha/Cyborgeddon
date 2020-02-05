#pragma once

#include <Engine/EngineInterface/SceneInterface/ObjectNode.hpp>
#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtNode.hpp>

namespace irr::scene {
    struct IMeshSceneNode;
    struct ISceneManager;
}

struct IrrlichtObjectNode final :  ObjectNode, IrrlichtNode {
    explicit IrrlichtObjectNode (
            irr::scene::ISceneManager * = nullptr,
            const glm::vec3 *  = nullptr,
            const glm::vec3 *  = nullptr,
            const glm::vec3 *  = nullptr
    );

    ~IrrlichtObjectNode() final = default;

    void setMesh(const char *) const override;

    protected :
        irr::scene::IMeshSceneNode * meshNode { nullptr };
};