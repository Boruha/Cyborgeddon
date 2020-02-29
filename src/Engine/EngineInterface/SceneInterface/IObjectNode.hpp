#pragma once

#include <Engine/EngineInterface/SceneInterface/INode.hpp>

struct IObjectNode : INode {
    ~IObjectNode() override = default;

    virtual void setMesh(std::string_view) = 0;
};