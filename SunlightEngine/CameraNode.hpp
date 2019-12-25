#pragma once

#include <irrlicht/irrlicht.h>

#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/GenericNode.hpp>

namespace Sun {
	struct CameraNode : public GenericNode {
		CameraNode() = default;

		explicit CameraNode(const Device& device, const Vector3f& target, const Vector3f &pos = Vector3f())
				: GenericNode(nullptr), cameraNode(device.getInnerDevice()->getSceneManager()->addCameraSceneNode()), target(&target)
				{
					this->node = cameraNode;		// aqui se inicializa el nodo padre realmente ya que la camara aun no ha sido creada
					this->setPosition(pos);
				}

		~CameraNode() override = default;

		void updateTarget() const { cameraNode->setTarget(irr::core::vector3df(target->x, target->y, target->z)); }

		private:
			irr::scene::ICameraSceneNode* cameraNode { nullptr };
			const Vector3f* target { nullptr };
	};
}