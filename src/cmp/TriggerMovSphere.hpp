#pragma once

#include <cmp/Component.hpp>

struct TriggerMovSphere : Component {
    explicit TriggerMovSphere(const EntityType e_type, const EntityID e_ID, const vec3& pos, const float rad, vec3& vel)
            : Component(e_type, e_ID), position(pos), radius(rad), velocity(&vel) {  }

	[[nodiscard]] constexpr static std::string_view getName() {
		return "TriggerMovSphere";
	}

    friend std::ostream& operator<<(std::ostream& os, const TriggerMovSphere& cmp);

    vec3   	position { 0 };			// posicion del bounding
    float	radius { 0 };			// radio de la esfera/circulo
    vec3 * 	velocity { nullptr }; 	// puntero a velocity del physics cmp
};