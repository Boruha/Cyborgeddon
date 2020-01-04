#include <sys/CollisionSystem.hpp>
#include <SunlightEngine/Vector3.hpp>
#include <cmath>

// TODO: quiza sea conveniente tener 3 sistemas de colisiones distintos, uno para objetos dinamicos, otro para estaticos y otro para objetos que se muevan muy rapido (ray)

void CollisionSystem::update(const std::unique_ptr<GameContext>& context) const {
	// IMPORTANTE:  si puedo tocar dos llaves (o dos puertas) a la vez en una misma iteracion del bucle del juego,
	// 			 	las condiciones siguientes NO seran correctas. No poner puertas muy juntas y asi nos ahorramos
	// 				varias comprobaciones por bucle

	// Antes de comoprobar cualquier tipo de colision con un collider que NO sea estatico, hay que asegurarse de que
	// la caja de colision se encuentra en su sitio haciendo collider.fixBox()
	// Después de cualquier metodo que pueda modificar la caja de colisiones (comprobando con otros colliders estaticos)
	// hay que volver a hacef collider.fixBox()

	// Ejemplo: player -> fix, tras comprobar colision player-pared es posible que la caja de colisiones haya sufrido
	// 			alguna modificación. CUALQUIER metodo estatico al final debe SÍ O SÍ hacer un fix de todos los colliders
	// 			que haya tocado

	BoundingBox& playerBox = *context->getPlayer().collider;
	Physics& playerPhysics = *context->getPlayer().physics;

	for (auto& collider : context->getBoundingComponents())
		if (collider.getEntityType() != UNDEFINED)		// ignoramos los componentes que no pertenecen a ninguna entidad
			fixBox(collider);

	for (int i = 0; i < 3; ++i) {
		if (playerPhysics.velocity[i] != 0) {								// si es necesario
			moveCoord(playerBox, playerPhysics.velocity[i], i);			// muevo la caja en la coordenada i (x -> y -> z)

			for (auto& collider : context->getBoundingComponents()) 		// compruebo colision con esa coordenada
				if (collider.getEntityType() != UNDEFINED && collider.getEntityType() != playerBox.getEntityType())	// ahora solo usamos al player, en un futuro esto cambiara
						typeFunctions[collider.type].p_func(playerBox, playerPhysics, collider, i, context);
		}
	}

	bulletCollision(context);
//	if(intersects(Vector3f(-32,3,375), Vector3f(-32, 3, 385), Vector3f(105, 0, 375), Vector3f(210, 0, 375)))
//		exit(0);
}


void CollisionSystem::dynamicCollision(BoundingBox& playerBox, Physics& physics, BoundingBox& otherBox, const int coord, const std::unique_ptr<GameContext>& context) {
	if (intersects(playerBox, otherBox)) {
		context->addToDestroy(otherBox.getEntityID()); // ahora mismo muere cualquier entidad con bounding dinamico
		if (otherBox.getEntityType() == EntityType::KEY) {
			for(auto& b : context->getBoundingComponents())
				if(b.getID() == otherBox.getID() - 1) // esto funciona solamente porque al crear el par key door, la llave se crea justo despues que la puerta
					b.type = ColliderType::DYNAMIC;
		}
		otherBox.makeUndefined();	// nos ahorramos comprobaciones si hacemos que el sistema ignore la bounding
	}
}

void CollisionSystem::staticCollision(BoundingBox& box, Physics& physics, BoundingBox& otherBox, const int coord, const std::unique_ptr<GameContext>& context) {
	if (intersects(box, otherBox)) {
		float offset;

		if (physics.velocity[coord] > 0)
			offset = otherBox.min[coord] - box.max[coord];
		else
			offset = otherBox.max[coord] - box.min[coord];

		physics.velocity[coord] += offset;
		moveCoord(box, offset, coord);
	}
}

void CollisionSystem::rayCollision(BoundingBox& box, Physics& physics, BoundingBox& otherBox, const int coord, const std::unique_ptr<GameContext>& context) {

}

void CollisionSystem::bulletCollision(const std::unique_ptr<GameContext>& context) const {
	for (auto& e : context->getEntities()) {
		if (e->getType() == EntityType::BULLET) {
			auto* bullet = dynamic_cast<EntityBullet*>(e.get());

			Vector3f posPrevia = *bullet->collider->pos;
			Vector3f posNueva = *bullet->collider->pos + bullet->physics->velocity;

			for (auto& cmp : context->getBoundingComponents()) {
				if (cmp.getEntityType() == EntityType::UNDEFINED)
					continue;
				if (cmp.type == ColliderType::STATIC || cmp.getEntityType() == EntityType::ENEMY || cmp.getEntityType() == EntityType::DOOR) {
					if (intersects(posPrevia, posNueva, cmp.min, Vector3f(cmp.min.x, 0, cmp.max.z)) ||
						intersects(posPrevia, posNueva, Vector3f(cmp.min.x, 0, cmp.max.z), cmp.max) ||
						intersects(posPrevia, posNueva, cmp.max, Vector3f(cmp.max.x, 0, cmp.min.z)) ||
						intersects(posPrevia, posNueva, Vector3f(cmp.max.x, 0, cmp.min.z), cmp.min)) {
						std::cout << "Bala con segmento " << posPrevia << posNueva << "Choca con entidad tipo " << cmp.getEntityType() << std::endl;
						if (cmp.getEntityType() == EntityType::ENEMY) {
							context->addToDestroy(cmp.getEntityID());
							cmp.makeUndefined();
						}
						context->addToDestroy(bullet->getID());
						bullet->collider->makeUndefined();
						break;
					}
				}
			}
		}
	}
}

void CollisionSystem::fixCoord(BoundingBox& bounding, const int coord) {
	const Vector3f& pos = *bounding.pos;

	bounding.min[coord] = pos[coord] - (bounding.dim[coord] / 2);
	bounding.max[coord] = pos[coord] + (bounding.dim[coord] / 2);
}

void CollisionSystem::fixBox(BoundingBox& bounding) {
	const Vector3f& pos = *bounding.pos;

	for(int i = 0; i < 3; ++i) {
		bounding.min[i] = pos[i] - (bounding.dim[i] / 2);
		bounding.max[i] = pos[i] + (bounding.dim[i] / 2);
	}
}

void CollisionSystem::moveCoord(BoundingBox& bounding, float mov, int coord) {
	for (int i = 0; i < 2; ++i)
		bounding[i][coord] += mov;
}

void CollisionSystem::moveBox(BoundingBox& bounding, const Vector3f& mov) {
	for (int i = 0; i < 2; ++i)
		bounding[i] += mov;
}

bool CollisionSystem::intersects(const BoundingBox& bounding, const BoundingBox& other) {
	for (int i = 0; i < 3; ++i)
		if(bounding.max[i] <= other.min[i] || bounding.min[i] >= other.max[i])
			return false;

	return true;
}

// return -> c pertenece al segmento ab o no
bool CollisionSystem::pointOnSegment(const Vector3f& a, const Vector3f& b, const Vector3f& c)
{
	return c.x <= fmax(a.x, b.x) && c.x >= fmin(a.x, b.x) && c.z <= fmax(a.z, b.z) && c.z >= fmin(a.z, b.z);
}

// return 0  ->  abc forman una recta
// return 1  ->  abc se orientan a reloj
// return -1 ->  abc se orientan a contrarreloj
int CollisionSystem::segmentOrientation(const Vector3f& a, const Vector3f& b, const Vector3f& c)
{
	float o = (b.z - a.z) * (c.x - b.x) - (b.x - a.x) * (c.z - b.z);

	return (o == 0) ? 0 : ((o > 0) ? 1 : -1);
}

// return -> ab intersecta a cd o no
bool CollisionSystem::intersects(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Vector3f& d)
{
	// orientaciones de la combinacion de los puntos de los dos segmentos
	int o1 = segmentOrientation(a, b, c);
	int o2 = segmentOrientation(a, b, d);
	int o3 = segmentOrientation(c, d, b);
	int o4 = segmentOrientation(c, d, a);

	// caso general
	if (o1 != o2 && o3 != o4)
		return true;

	// casos especiales
	return (o1 == 0 && pointOnSegment(a, b, c)) || (o2 == 0 && pointOnSegment(a, b, d)) ||
		   (o3 == 0 && pointOnSegment(c, d, b)) || (o4 == 0 && pointOnSegment(c, d, a));
}