#include <sys/CollisionSystem.hpp>
#include <SunlightEngine/Vector3.hpp>

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

	for (auto& collider : context->getBoundingComponents())
		if (collider.getEntityType() != UNDEFINED)		// ignoramos los componentes que no pertenecen a ninguna entidad
			fixBox(collider);

	BoundingBox& playerBox = *context->getPlayer().collider;
	Physics& playerPhysics = *context->getPlayer().physics;

	for (int i = 0; i < 3; ++i) {
		if (playerPhysics.velocity[i] != 0) {																	// si es necesario
			moveCoord(playerBox,playerPhysics.velocity[i], i);			// muevo la caja en la coordenada i (x -> y -> z)

			for (auto& collider : context->getBoundingComponents()) 												// compruebo colision con esa coordenada
				if (collider.getEntityType() != UNDEFINED && collider.getEntityType() != playerBox.getEntityType())	// ahora solo usamos al player, en un futuro esto cambiara
							typeFunctions[collider.type].p_func(playerBox, playerPhysics, collider, i, context);
		}
	}
}


void CollisionSystem::dynamicCollision(BoundingBox& playerBox, Physics& physics, BoundingBox& otherBox, const int coord, const std::unique_ptr<GameContext>& context) {
	if (intersects(playerBox, otherBox)) {
		context->addToDestroy(otherBox.getEntityID()); // ahora mismo muere cualquier entidad con bounding dinamico
		if (otherBox.getEntityType() == EntityType::KEY) {
			for(auto& e : context->getEntities())
				if (e.getID() == otherBox.getEntityID() - 1)
					e.collider->type = DYNAMIC;
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