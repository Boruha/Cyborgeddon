#pragma once

#include <Engine/GameNode.hpp>

struct ObjectNode : GameNode {
    ~ObjectNode() override = 0;

    virtual void setMesh(char *) const = 0;
};