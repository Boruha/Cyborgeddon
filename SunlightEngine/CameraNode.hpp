#pragma once

#include <irrlicht/irrlicht.h>

#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/GenericNode.hpp>

namespace Sun {
	struct CameraNode : public GenericNode {
		// Deleted constructors
		CameraNode() = delete;
		CameraNode& operator=(const CameraNode& camera) = delete;
		CameraNode& operator=(CameraNode&& camera) = delete;
		CameraNode(CameraNode&& camera) = delete;

		// Constructors
		CameraNode(const CameraNode& camera) = default;

		explicit CameraNode(const Device& device, const vec3& target, const vec3& position, const vec3& rotation = vec3())
				: GenericNode(nullptr, position, rotation), cameraNode(device.getInnerDevice()->getSceneManager()->addCameraSceneNode()), target(&target)
				{
					this->node = cameraNode;		// aqui se inicializa el nodo padre realmente ya que la camara aun no ha sido creada
					setPosition();
					updateTarget(target);
				}

		~CameraNode() override = default;

		void update(const float deltaTime) const override {
			GenericNode::update(deltaTime);
			updateTarget(deltaTime);
		}

		irr::scene::ICameraSceneNode* getInner() { return cameraNode; }

	private:

		[[nodiscard]] vec3 getTarget() const { return vec3(cameraNode->getTarget().X,cameraNode->getTarget().Y,cameraNode->getTarget().Z); }
		void updateTarget(const float deltaTime) const { if(cameraNode) { updateTarget(getTarget() + (*target - getTarget()) * deltaTime); } }
		void updateTarget(const vec3& targ) const { cameraNode->setTarget(irr::core::vector3df(targ.x, targ.y, targ.z)); }

		irr::scene::ICameraSceneNode* cameraNode { nullptr };

		const vec3* target { nullptr };
	};
}