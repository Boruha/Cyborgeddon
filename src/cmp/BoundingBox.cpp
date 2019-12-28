#include <cmp/BoundingBox.hpp>

AABB::AABB(const Vector3f& dimension, const Vector3f& position) {
	setBox(dimension, position);
}

void AABB::setBox(const Vector3f& dimension, const Vector3f& position) {
	max.x = position.x + dimension.x / 2;
	min.x = position.x - dimension.x / 2;

	max.y = position.y + dimension.y / 2;
	min.y = position.y - dimension.y / 2;

	max.z = position.z + dimension.z / 2;
	min.z = position.z - dimension.z / 2;
}

void AABB::moveCoord(const float& mov, const int& coord) {
	min[coord] += mov;
	max[coord] += mov;
}

bool AABB::intersects(const AABB& box) const {
	return max.x > box.min.x
		&& min.x < box.max.x

		&& max.y > box.min.y
		&& min.y < box.max.y

		&& max.z > box.min.z
		&& min.z < box.max.z;
}

void BoundingBox::fixBox() {
	box.setBox(dim, *pos);
}

void BoundingBox::moveCoord(const float& mov, const int& coord) {
	box.moveCoord(mov, coord);
}

bool BoundingBox::intersects(const BoundingBox& other) const {
	return box.intersects(other.box);
}