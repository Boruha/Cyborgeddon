#pragma once

#include <glm/glm.hpp>

#include <memory>

struct IObjectNode;
struct ICameraNode;
struct INode;

using glm::vec3;

struct IScene {
    virtual ~IScene() = default;

    virtual std::unique_ptr<INode> addObjectNode() = 0;
    virtual std::unique_ptr<INode> addCameraNode() = 0;

    [[nodiscard]] virtual vec3 cursorToWorld(float, float, float) = 0;

    virtual void loadTexture(const std::string&) const = 0;
    virtual void unloadTexture(const std::string&) const = 0; // TODO : debe ser un puntero a clase textura (que hay que hcer en util/Texture)
	virtual void unloadTextures() const = 0;

	ICameraNode * camera { nullptr };
};