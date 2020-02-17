#pragma once

#include <Engine/EngineInterface/SceneInterface/INode.hpp>

struct IObjectNode : INode {
    ~IObjectNode() override = default;

    virtual void setMesh(const char *) = 0;
};