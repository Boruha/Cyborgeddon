#pragma once

#include <irrlicht/irrlicht.h>

#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/GenericNode.hpp>

namespace Sun {
	struct CameraNode : public GenericNode {

		explicit CameraNode(const Device& device, const Vector3f &pos = Vector3f(), const Vector3f &target = Vector3f(0, 0, 100))
				: GenericNode(device.getInnerDevice()->getSceneManager()->addCameraSceneNode(
						nullptr,
						irr::core::vector3df(pos.x, pos.y, pos.z),
						irr::core::vector3df(target.x, target.y, target.z))) {  }

		~CameraNode() override = default;

		void setTarget(const Vector3f &target) const { dynamic_cast<irr::scene::ICameraSceneNode*>(node)->setTarget(irr::core::vector3df(target.x, target.y, target.z)); }
	};
}