#include <cmp/BoundingBox.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

vec3& BoundingBox::operator[](const int index) {
	return const_cast<vec3&>(std::as_const(*this).operator[](index));
}

const vec3& BoundingBox::operator[](const int index) const {
	switch(index) {
		case 0  : return min;
		case 1  : return max;
		default : std::cerr << "Out of bound BoundingBox\n"; exit(-1);
	}
}

std::ostream& operator<<(std::ostream& os, const BoundingBox& box) {
	box.print(os, "BoundingBox")
		<< "\n\tPos: " 							<< glm::to_string(*box.pos)
		<< "\n\tVel: ";		  box.velocity ? 	os << glm::to_string(*box.velocity) : os << " nullptr";
	os 	<< "\n\tMin: "							<< glm::to_string(box.min)
		<< "\n\tMax: " 							<< glm::to_string(box.max)
		<< "\n\tPasable: " 	<< std::boolalpha 	<<  box.passable
		<< "\n\tType: ";

	switch (box.type) {
		case DYNAMIC: 	os << "DYNAMIC"; 	break;
		case STATIC:	os << "STATIC";		break;
		case RAY:		os << "RAY";		break;
		case END_TYPE:	os << "ENDTYPE";	break;
	}

	return os;
}

BoundingBox::BoundingBox(const EntityType e_type, const std::size_t e_ID, const vec3& dim, vec3& pos, vec3& vel, const bool passable, const ColliderType type)
		: Component(e_type, e_ID), passable(passable), type(type), dim(dim), pos(&pos), velocity(&vel) {

}

BoundingBox::BoundingBox(const EntityType e_type, const std::size_t e_ID, const vec3& dim, vec3& pos, const bool passable, const ColliderType type)
		: Component(e_type, e_ID), passable(passable), type(type), dim(dim), pos(&pos) {

}
