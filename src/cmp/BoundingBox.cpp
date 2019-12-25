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

void AABB::move(const Vector3f& mov) {
	max.x += mov.x;
	min.x += mov.x;

	max.y += mov.y;
	min.y += mov.y;

	max.z += mov.z;
	min.z += mov.z;
}

bool AABB::intersects(const AABB& box) const {
	return max.x > box.min.x
		&& min.x < box.max.x

		&& max.y > box.min.y
		&& min.y < box.max.y

		&& max.z > box.min.z
		&& min.z < box.max.z;
}

void BoundingBox::setPosition(const Vector3f& position) {
	pos = position;
	box.setBox(dim, pos);
}

void BoundingBox::move(const Vector3f& mov) {
	pos += mov;
	box.move(mov);
}

bool BoundingBox::intersects(const BoundingBox& other) const {
	return box.intersects(other.box);
}