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