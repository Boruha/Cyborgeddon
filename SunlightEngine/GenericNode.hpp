#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <irrlicht/irrlicht.h>

namespace Sun {
	struct GenericNode {
		// Deleted constructors
		GenericNode() = delete;
		GenericNode& operator=(const GenericNode& node) = delete;
		GenericNode& operator=(GenericNode&& node) = delete;

		// Constructors
		GenericNode(const GenericNode& node) = default;
		GenericNode(GenericNode&& node) = default;

		explicit GenericNode(irr::scene::ISceneNode* node, const Vector3f& position, const Vector3f& rotation) :
			node(node), position(&position), rotation(&rotation) {
		}

		virtual ~GenericNode() = default;

		virtual void update() const { setPosition(); }
        virtual void setTexture(const char* texture) const {  }

		[[nodiscard]] Vector3f getPosition() const { return Vector3f(node->getPosition().X, node->getPosition().Y, node->getPosition().Z); }

        [[nodiscard]] Vector3f getScale() const { return Vector3f(node->getScale().X, node->getScale().Y, node->getScale().Z); }

        [[nodiscard]] Vector3f getRotation() const { return Vector3f(node->getRotation().X, node->getRotation().Y, node->getRotation().Z); }


        void removeFromScene() { if(node) node->remove(); node = nullptr; }

		protected:
			irr::scene::ISceneNode* node { nullptr };

			void setScale(const Vector3f& dim) const { node->setScale(irr::core::vector3df(dim.x, dim.y, dim.z)); }

			void setPosition() const { if(node) node->setPosition(irr::core::vector3df(position->x, position->y, position->z)); }
	        void setRotation() const { if(node) node->setRotation(irr::core::vector3df(rotation->x, rotation->y, rotation->z)); }

		private:

			const Vector3f* position { nullptr };
			const Vector3f* rotation { nullptr };
	};
}