#pragma once

#include <Engine/EngineInterface/SceneInterface/Node.hpp>

struct ObjectNode : virtual Node {
    ~ObjectNode() override = default;

    virtual void setMesh(const char *) const = 0;
};