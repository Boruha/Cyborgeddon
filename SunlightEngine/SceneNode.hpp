#pragma once

#include <irrlicht/irrlicht.h>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/GenericNode.hpp>

namespace Sun {
	struct SceneNode : public GenericNode {
		// Deleted Constructors
		SceneNode() = delete;
		SceneNode& operator=(const SceneNode& node) = delete;
		SceneNode& operator=(SceneNode&& node) = delete;

		// Constructors
		SceneNode(const SceneNode& node) = default;
		SceneNode(SceneNode&& node) = default;

		explicit SceneNode(const Device& device, const Vector3f& position, const Vector3f& rotation, const Vector3f& dim, const char* mesh = nullptr, const char* texture = nullptr)
			: GenericNode(nullptr, position, rotation), meshNode(device.getInnerDevice()->getSceneManager()->addCubeSceneNode(1))
		{
			this->node = meshNode; 									// aqui se inicializa el generic node

			setScale(dim);
			setPosition();
			setRotation();

//			if (mesh) setMesh(mesh);

			if (texture)
				meshNode->setMaterialTexture(0,meshNode->getSceneManager()->getVideoDriver()->getTexture(texture));

			affectedByLight(false);
		}

		~SceneNode() override = default;

		void update(const float deltaTime) const override { GenericNode::update(deltaTime); setRotation(deltaTime); }

       // arena->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR); //makes it transparent
		void setTexture(const char* texture) const override {
			meshNode->setMaterialTexture(0,
									 meshNode->getSceneManager()->getVideoDriver()->getTexture(
											 texture));
		}

		void affectedByLight(const bool b) const { meshNode->setMaterialFlag(irr::video::EMF_LIGHTING, b); }

		private:

			irr::scene::IMeshSceneNode* meshNode = { nullptr };
	};
}