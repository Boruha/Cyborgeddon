#include <cmp/BoundingBox.hpp>

Vector3f& BoundingBox::operator[](const int &index) {
	switch(index) {
		case 0  : return min;
		case 1  : return max;
		default : std::cerr << "Out of bound BoundingBox\n"; exit(-1);
	}
}

const Vector3f& BoundingBox::operator[](const int &index) const {
	switch(index) {
		case 0  : return min;
		case 1  : return max;
		default : std::cerr << "Out of bound BoundingBox\n"; exit(-1);
	}
}