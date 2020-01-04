#pragma once

#include <irrlicht/irrlicht.h>

#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/GenericNode.hpp>

namespace Sun {
	struct CameraNode : public GenericNode {
		// Deleted constructors
		CameraNode() = delete;
		CameraNode& operator=(const CameraNode& camera) = delete;
		CameraNode& operator=(CameraNode&& camera) = delete;

		// Constructors
		CameraNode(const CameraNode& camera) = default;
		CameraNode(CameraNode&& camera) = default;

		explicit CameraNode(const Device& device, const Vector3f& target, const Vector3f& position, const Vector3f& rotation = Vector3f())
				: GenericNode(nullptr, position, rotation), cameraNode(device.getInnerDevice()->getSceneManager()->addCameraSceneNode()), target(&target)
				{
					this->node = cameraNode;		// aqui se inicializa el nodo padre realmente ya que la camara aun no ha sido creada
				}

		~CameraNode() override = default;

		void update() const override {
			GenericNode::update();
			updateTarget();
		}

		irr::scene::ICameraSceneNode* getInner() { return cameraNode; }

	private:

		void updateTarget() const { if(cameraNode) cameraNode->setTarget(irr::core::vector3df(target->x, target->y, target->z)); }

		irr::scene::ICameraSceneNode* cameraNode { nullptr };

		const Vector3f* target { nullptr };
	};
}