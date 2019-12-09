#pragma once

#include <irrlicht/irrlicht.h>

#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/GenericNode.hpp>

namespace Sun {
	struct CameraNode : public GenericNode {
		CameraNode() = default;

		explicit CameraNode(Device *device, const Vector3f &pos = Vector3f(), const Vector3f &target = Vector3f(0, 0, 100))
				: GenericNode(device->getInnerDevice()->getSceneManager()->addCameraSceneNode(
						nullptr,
						irr::core::vector3df(pos.x, pos.y, pos.z),
						irr::core::vector3df(target.x, target.y, target.z))) {  }

		~CameraNode() override = default;

		[[nodiscard]] Vector3f getPosition() const override {
			return Vector3f(camera->getPosition().X, camera->getPosition().Y, camera->getPosition().Z);
		}

		void setPosition(const Vector3f &pos) const override { camera->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z)); }
		void setTarget(const Vector3f &target) const { camera->setPosition(irr::core::vector3df(target.x, target.y, target.z)); }

		void removeFromScene() const override { camera->getSceneManager()->addToDeletionQueue(camera); }

	private:
		irr::scene::ICameraSceneNode* camera { nullptr };
	};
}