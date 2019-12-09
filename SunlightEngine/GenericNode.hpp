#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <irrlicht/irrlicht.h>

namespace Sun {
	struct GenericNode {
		GenericNode() = default;
		explicit GenericNode(irr::scene::ISceneNode* node) : node(node) {  }
		virtual ~GenericNode() = default;

		[[nodiscard]] virtual Vector3f getPosition() const = 0;
		virtual void setPosition(const Vector3f&) const = 0;
		virtual void removeFromScene() const = 0;

		protected:
			irr::scene::ISceneNode* node { nullptr };
	};
}