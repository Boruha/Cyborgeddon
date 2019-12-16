#pragma once

#include <irrlicht/irrlicht.h>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/GenericNode.hpp>

namespace Sun {
	struct SceneNode : public GenericNode {
		SceneNode() = default;
		explicit SceneNode(const Device& device, const Vector3f& pos = Vector3f(), const Vector3f &dim = Vector3f(10))
			: GenericNode(nullptr), meshNode(device.getInnerDevice()->getSceneManager()->addCubeSceneNode(1)), dim(dim)
		{
			this->node = meshNode; 									// aqui se inicializa el generic node
			this->setPosition(Vector3f(pos.x, pos.y, pos.z));
			this->setScale(dim);
			affectedByLight(false);
		}

		~SceneNode() override = default;

		void setTexture(const char* texture = "") const {
			meshNode->setMaterialTexture(0,
									 meshNode->getSceneManager()->getVideoDriver()->getTexture(
											 texture));
		}

		void affectedByLight(const bool b) const { meshNode->setMaterialFlag(irr::video::EMF_LIGHTING, b); }

		[[nodiscard]] bool intersects(const SceneNode& other) const {
			return (std::abs(other.getPosition().x - this->getPosition().x) < (other.dim.x + this->dim.x) / 2 &&
					std::abs(other.getPosition().y - this->getPosition().y) < (other.dim.y + this->dim.y) / 2 &&
					std::abs(other.getPosition().z - this->getPosition().z) < (other.dim.z + this->dim.z) / 2);
		}

		private:
			irr::scene::IMeshSceneNode* meshNode = { nullptr };
			Vector3f dim {  };
	};
}