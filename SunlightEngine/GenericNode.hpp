#pragma once

#include <Engine/util/Vector3.hpp>
#include <glm/glm.hpp>
#include <irrlicht/irrlicht.h>

using glm::vec3;

namespace Sun {
	struct GenericNode {
		// Deleted constructors
		GenericNode() = delete;
		GenericNode& operator=(const GenericNode& node) = delete;
		GenericNode& operator=(GenericNode&& node) = delete;
		GenericNode(GenericNode&& node) = delete;

		// Constructors
		GenericNode(const GenericNode& node) = default;

		explicit GenericNode(irr::scene::ISceneNode* node, const vec3& position, const vec3& rotation) :
			node(node), position(&position), rotation(&rotation) {
		}

		virtual ~GenericNode() = default;

		explicit operator bool() const { return node != nullptr; }

		virtual void update(const float deltaTime) const { setPosition(deltaTime); }
        virtual void setTexture(const char* texture) const {  }

		[[nodiscard]] vec3 getPosition() const { return vec3(node->getPosition().X, node->getPosition().Y, node->getPosition().Z); }
 		[[nodiscard]] vec3 getRotation() const { return vec3(node->getRotation().X, node->getRotation().Y, node->getRotation().Z); }
 		[[nodiscard]] vec3 getScale() 	 const { return vec3(node->getScale().X, node->getScale().Y, node->getScale().Z); }

        void setPos(const vec3& pos) const { if(node) node->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z)); }
		void setRot(const vec3& rot) const { if(node) node->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z)); }

        void removeFromScene() { if(node) node->remove(); node = nullptr; }

		protected:
			irr::scene::ISceneNode* node { nullptr };

			void setPosition() const { if(node) setPos(*position); }
			void setRotation() const { if(node) setRot(*rotation); }

			void setPosition(const float deltaTime) const { if(node) setPos(getPosition() + (*position - getPosition()) * deltaTime); }
	        void setRotation(const float deltaTime) const { if(node) setRot(getRotation() + (*rotation - getRotation()) * deltaTime); }

	        void setScale(const vec3& dim) const { node->setScale(irr::core::vector3df(dim.x, dim.y, dim.z)); }

		private:

			const vec3* position { nullptr };
			const vec3* rotation { nullptr };
	};
}