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

std::ostream& operator<<(std::ostream &os, const BoundingBox& box) {
	os 	<< "\nBoundingBox\nPos: " 	<< *box.pos 							<< "\nVel: " << *box.velocity
		<< "\nMin: " 				<<  box.min								<< "\nMax: " <<  box.max
		<< "\nPasable: " 			<< (box.passable ? "true" : "false")	<< "\nType: ";

	switch (box.type) {
		case DYNAMIC: 	os << "DYNAMIC"; 	break;
		case STATIC:	os << "STATIC";		break;
		case RAY:		os << "RAY";		break;
		case END_TYPE:	os << "ENDTYPE";	break;
	}

	return os << std::endl;
}