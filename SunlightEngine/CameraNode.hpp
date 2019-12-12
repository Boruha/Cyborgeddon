#pragma once

#include <irrlicht/irrlicht.h>

#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/GenericNode.hpp>

namespace Sun {
	struct CameraNode : public GenericNode {
		CameraNode() = default;

		explicit CameraNode(const Device& device, const Vector3f &pos = Vector3f(), const Vector3f &target = Vector3f(0, 0, 100))
				: GenericNode(nullptr), cameraNode(device.getInnerDevice()->getSceneManager()->addCameraSceneNode())
				{
					this->node = cameraNode;		// aqui se inicializa el nodo padre realmente ya que la camara aun no ha sido creada
					this->setPosition(pos);
					this->setTarget(target);
				}

		~CameraNode() override = default;

		void setTarget(const Vector3f &target) const { cameraNode->setTarget(irr::core::vector3df(target.x, target.y, target.z)); }

		private:
			irr::scene::ICameraSceneNode* cameraNode { nullptr };
	};
}