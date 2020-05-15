#pragma once

#include <cmp/Component.hpp>

struct INode;

struct WalkingAnimation : public Component {
	explicit WalkingAnimation(EntityType e_type, EntityID e_ID) : Component(e_type, e_ID) { };

	[[nodiscard]] constexpr static std::string_view getName() {
		return "WalkingAnimation";
	}

	friend std::ostream& operator<<(std::ostream& os, const WalkingAnimation& opt);


};