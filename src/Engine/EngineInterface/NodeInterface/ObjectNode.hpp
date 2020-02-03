#pragma once

#include <Engine/EngineInterface/NodeInterface/GameNode.hpp>

struct ObjectNode : GameNode {
    ~ObjectNode() override = 0;

    virtual void setMesh(const char *) const = 0;
};