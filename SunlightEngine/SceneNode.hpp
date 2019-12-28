#pragma once

#include <irrlicht/irrlicht.h>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/GenericNode.hpp>

namespace Sun {
	struct SceneNode : public GenericNode {
		SceneNode() = default;
		explicit SceneNode(const Device& device, const Vector3f& position, const Vector3f& rotation, const Vector3f& dim, const char* mesh = nullptr, const char* texture = nullptr)
			: GenericNode(nullptr, position, rotation), meshNode(device.getInnerDevice()->getSceneManager()->addCubeSceneNode(1))
		{
			this->node = meshNode; 									// aqui se inicializa el generic node

			setScale(dim);

//			if (mesh) setMesh(mesh);

			if (texture)
				setTexture(texture);

			affectedByLight(false);
		}

		~SceneNode() override = default;

		void update() const override { GenericNode::update(); setRotation(); }

       // arena->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR); //makes it transparent
		void setTexture(const char* texture = "") const {
			meshNode->setMaterialTexture(0,
									 meshNode->getSceneManager()->getVideoDriver()->getTexture(
											 texture));
		}

		void affectedByLight(const bool b) const { meshNode->setMaterialFlag(irr::video::EMF_LIGHTING, b); }

		private:

			irr::scene::IMeshSceneNode* meshNode = { nullptr };
	};
}