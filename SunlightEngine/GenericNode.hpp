#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <irrlicht/irrlicht.h>

namespace Sun {
	struct GenericNode {
		GenericNode() = default;
		explicit GenericNode(irr::scene::ISceneNode* node) : node(node) {  }
		virtual ~GenericNode() { removeFromScene(); }

		[[nodiscard]] Vector3f getPosition() const { return Vector3f(node->getPosition().X, node->getPosition().Y, node->getPosition().Z); }
		void setPosition(const Vector3f& pos) const { node->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z)); }


        protected:
			irr::scene::ISceneNode* node { nullptr };

	    private:
		    void removeFromScene() const { node->getSceneManager()->addToDeletionQueue(node); }

	};
}