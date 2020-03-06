#pragma once

#include <glm/glm.hpp>

#include <memory>

struct IObjectNode;
struct ICameraNode;
struct INode;

using glm::vec3;

struct IScene {
    virtual ~IScene() = default;

    virtual std::unique_ptr<INode> addObjectNode(std::string_view) = 0;
	virtual INode* addMeshNode(std::string_view) = 0;
	virtual std::unique_ptr<INode> addCameraNode() = 0;
	virtual INode * addFreeCameraNode() = 0;


	[[nodiscard]] virtual vec3 cursorToWorld(float, float, float) = 0;

    virtual void loadTexture(std::string_view) 		const = 0;
    virtual void unloadTexture(std::string_view) 	const = 0; // TODO : debe ser un puntero a clase textura (que hay que hcer en util/Texture)
	virtual void unloadTextures() 					const = 0;

	ICameraNode * camera { nullptr };
};