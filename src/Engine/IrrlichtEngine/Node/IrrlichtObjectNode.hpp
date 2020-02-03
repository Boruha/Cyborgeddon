#pragma once

#include <Engine/EngineInterface/NodeInterface/ObjectNode.hpp>
#include <Engine/IrrlichtEngine/Node/IrrlichtGameNode.hpp>

struct IrrlichtObjectNode : ObjectNode, IrrlichtGameNode {
    explicit IrrlichtObjectNode (
            const Vector3f * const pos = nullptr,
            const Vector3f * const rot = nullptr,
            const Vector3f * const sca = nullptr
    ) : IrrlichtGameNode(pos, rot, sca) { }

    ~IrrlichtObjectNode() override = default;

    void setMesh(const char *) const override;
};