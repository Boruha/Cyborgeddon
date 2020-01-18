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

		explicit operator bool() const { return node != nullptr; }

		virtual void update(const float deltaTime) const { setPosition(deltaTime); }
        virtual void setTexture(const char* texture) const {  }

		[[nodiscard]] Vector3f getPosition() const { return Vector3f(node->getPosition().X, node->getPosition().Y, node->getPosition().Z); }
 		[[nodiscard]] Vector3f getRotation() const { return Vector3f(node->getRotation().X, node->getRotation().Y, node->getRotation().Z); }
 		[[nodiscard]] Vector3f getScale() 	 const { return Vector3f(node->getScale().X, node->getScale().Y, node->getScale().Z); }

        void setPos(const Vector3f& pos) const { if(node) node->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z)); }
		void setRot(const Vector3f& rot) const { if(node) node->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z)); }

        void removeFromScene() { if(node) node->remove(); node = nullptr; }

		protected:
			irr::scene::ISceneNode* node { nullptr };

			void setPosition() const { if(node) setPos(*position); }
			void setRotation() const { if(node) setPos(*position); }

			void setPosition(const float deltaTime) const { if(node) setPos(getPosition() + (*position - getPosition()) * deltaTime); }
	        void setRotation(const float deltaTime) const { if(node) setRot(getRotation() + (*rotation - getRotation()) * deltaTime); }

	        void setScale(const Vector3f& dim) const { node->setScale(irr::core::vector3df(dim.x, dim.y, dim.z)); }

		private:

			const Vector3f* position { nullptr };
			const Vector3f* rotation { nullptr };
	};
}