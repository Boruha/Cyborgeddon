#pragma once

#include <irrlicht/irrlicht.h>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/GenericNode.hpp>

namespace Sun {
	struct SceneNode : public GenericNode {
		SceneNode() = default;

		explicit SceneNode(const Device& device, const Vector3f& pos = Vector3f(), const Vector3f &dim = Vector3f(10))
			: GenericNode(device.getInnerDevice()->getSceneManager()->addCubeSceneNode(
					dim.x,
					nullptr,
					-1,
					irr::core::vector3df(pos.x, pos.y, pos.z)))
		{
			affectedByLight(false);
		}

		~SceneNode() override = default;

		void setTexture(const char *texture = "") const {
			node->setMaterialTexture(0,
									 node->getSceneManager()->getVideoDriver()->getTexture(
											 texture));
		}

		void affectedByLight(const bool b) const { node->setMaterialFlag(irr::video::EMF_LIGHTING, b); }
	};
}