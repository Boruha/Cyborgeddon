#include <sys/CollisionSystem.hpp>
#include <SunlightEngine/Vector3.hpp>

// TODO: quiza sea conveniente tener 3 sistemas de colisiones distintos, uno para objetos dinamicos, otro para estaticos y otro para objetos que se muevan muy rapido (ray)

void CollisionSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	// IMPORTANTE:  si puedo tocar dos llaves (o dos puertas) a la vez en una misma iteracion del bucle del juego,
	// 			 	las condiciones siguientes NO seran correctas. No poner puertas muy juntas y asi nos ahorramos
	// 				varias comprobaciones por bucle

	// Antes de comoprobar cualquier tipo de colision con un collider que NO sea estatico, hay que asegurarse de que
	// la caja de colision se encuentra en su sitio haciendo fixBox()
	// Después de cualquier metodo que pueda modificar la caja de colisiones (comprobando con otros colliders estaticos)
	// hay que volver a hacer fixBox()

	// Ejemplo: player -> fix, tras comprobar colision player-pared es posible que la caja de colisiones haya sufrido
	// 			alguna modificación. CUALQUIER metodo estatico al final debe SÍ O SÍ hacer un fix de todos los colliders
	// 			que haya tocado

	for (auto& collider : std::get<vector<BoundingBox>>(context->getComponents(SPECIAL_BOUNDING_BOX_TYPE)))
		if (collider.getEntityType() != UNDEFINED)		// ignoramos los componentes que no pertenecen a ninguna entidad
			fixBox(collider);

	for (auto& staticCollider : std::get<vector<BoundingBox>>(context->getComponents(STATIC_BOUNDING_BOX_TYPE)))
		if (staticCollider.getEntityType() != UNDEFINED)		// ignoramos los componentes que no pertenecen a ninguna entidad
			fixBox(staticCollider);

	BoundingBox& playerBox  = *context->getPlayer().collider;
	Vector3f& 	 velocity 	= *playerBox.velocity;

	for (int i = 0; i < 3; ++i) {
		int numChecks = ceil(abs(velocity[i]) / (playerBox.dim[i] / 2));

		if (numChecks <= 0)
			continue;

		velocity[i] /= static_cast<float>(numChecks);

		for (int j = 0; j < numChecks; ++j) {
			moveCoord(playerBox, velocity[i], i);

			for (const auto& staticCollider : std::get<vector<BoundingBox>>(context->getComponents(STATIC_BOUNDING_BOX_TYPE)))
				if (staticCollider.getEntityType() != UNDEFINED)
					staticCollision(playerBox, velocity, staticCollider, i);

			for (auto& collider : std::get<vector<BoundingBox>>(context->getComponents(SPECIAL_BOUNDING_BOX_TYPE))) {
				if (collider.getEntityType() != UNDEFINED && collider.getEntityType() != playerBox.getEntityType()) {
					if (collider.type == STATIC)
						staticCollision(playerBox, velocity, collider, i);
					else
						dynamicCollision(playerBox, velocity, collider, context);
				}
			}
		}
		velocity[i] *= static_cast<float>(numChecks);
	}
}

void CollisionSystem::dynamicCollision(BoundingBox& playerBox, Vector3f& velocity, BoundingBox& otherBox, const std::unique_ptr<GameContext>& context) const {
	if (intersects(playerBox, otherBox)) {
		if (otherBox.type == DYNAMIC) {
			context->addToDestroy(otherBox.getEntityID()); // ahora mismo mueren solo llaves
			if (otherBox.getEntityType() == KEY)
				for(auto& e : context->getEntities())
					if (e.getID() == otherBox.getEntityID() - 1)
						e.collider->type = DYNAMIC;
			otherBox.makeUndefined();	// nos ahorramos comprobaciones si hacemos que el sistema ignore la bounding
		}
	}
}

void CollisionSystem::staticCollision(BoundingBox& box, Vector3f& velocity, const BoundingBox& otherBox, const int coord) const {
	if (intersects(box, otherBox)) {
		float offset{0};

		if (velocity[coord] > 0)
			offset = otherBox.min[coord] - box.max[coord];
		else
			offset = otherBox.max[coord] - box.min[coord];

		velocity[coord] += offset;

		moveCoord(box, offset, coord);

		if (Sun::equal_e(velocity[coord], 0))
			velocity[coord] = 0;
	}
}

void CollisionSystem::fixCoord(BoundingBox& bounding, const int coord) const {
	const Vector3f& pos = *bounding.pos;

	bounding.min[coord] = pos[coord] - (bounding.dim[coord] / 2);
	bounding.max[coord] = pos[coord] + (bounding.dim[coord] / 2);
}

void CollisionSystem::fixBox(BoundingBox& bounding) const {
	setBox(bounding, *bounding.pos);
}

void CollisionSystem::moveCoord(BoundingBox& bounding, float mov, int coord) const {
	for (int i = 0; i < 2; ++i)
		bounding[i][coord] += mov;
}

void CollisionSystem::moveBox(BoundingBox& bounding, const Vector3f& mov) const {
	for (int i = 0; i < 2; ++i)
		bounding[i] += mov;
}

void CollisionSystem::setBox(BoundingBox& bounding, const Vector3f& pos) const {
	for (int i = 0; i < 3; ++i)
		setCoord(bounding, pos, i);
}

void CollisionSystem::setCoord(BoundingBox& bounding, const Vector3f& pos, const int coord) const {
	bounding.min[coord] = pos[coord] - (bounding.dim[coord] / 2);
	bounding.max[coord] = pos[coord] + (bounding.dim[coord] / 2);
}

bool CollisionSystem::intersects(const BoundingBox& bounding, const BoundingBox& other) const {
	for (int i = 0; i < 3; ++i)
		if(bounding.max[i] <= other.min[i] || bounding.min[i] >= other.max[i])
			return false;

	return true;
}