#pragma once

#include <util/Alias.hpp>
#include <cmp/Component.hpp>

struct Alive : public Component {
	explicit Alive(const EntityType e_type, const std::size_t e_ID) : Component(e_type, e_ID) {  };

	bool alive { true };
};