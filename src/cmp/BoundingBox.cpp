#include <cmp/BoundingBox.hpp>

Vector3f& BoundingBox::operator[](const int index) {
	return const_cast<Vector3f&>(std::as_const(*this).operator[](index));
}

const Vector3f& BoundingBox::operator[](const int index) const {
	switch(index) {
		case 0  : return min;
		case 1  : return max;
		default : std::cerr << "Out of bound BoundingBox\n"; exit(-1);
	}
}

std::ostream& operator<<(std::ostream& os, const BoundingBox& box) {
	box.print(os, "BoundingBox")
	<< "\n\tPos: " 							<< *box.pos
	<< "\n\tVel: " 							<< *box.velocity
	<< "\n\tMin: " 							<<  box.min
	<< "\n\tMax: " 							<<  box.max
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

BoundingBox::BoundingBox(const EntityType e_type, const std::size_t e_ID, const Vector3f& dim, Vector3f& pos, Vector3f& vel, const bool passable, const ColliderType type)
		: Component(e_type, e_ID), passable(passable), type(type), dim(dim), pos(&pos), velocity(&vel) {

}