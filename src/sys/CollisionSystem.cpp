#include <sys/CollisionSystem.hpp>

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

	auto& specialBoundings 	= std::get<vector<BoundingBox>>(context->getComponents(SPECIAL_BOUNDING_BOX_TYPE));
	auto& staticBoundings	= std::get<vector<BoundingBox>>(context->getComponents(STATIC_BOUNDING_BOX_TYPE));

	for (auto& collider : specialBoundings)
		if (collider)					// ignoramos los componentes que no pertenecen a ninguna entidad
			fixBox(collider);

	for (auto& staticCollider : staticBoundings)
		if (staticCollider)				// ignoramos los componentes que no pertenecen a ninguna entidad
			fixBox(staticCollider);

	for (auto& movingBox : specialBoundings) {											// recorremos el array de objetos no estaticos
		if (movingBox && movingBox.velocity) {
			for (int i = 0; i < 3; ++i) {
				Vector3f& velocity = *movingBox.velocity;

				int numChecks = ceil(abs(velocity[i]) / (movingBox.dim[i] / 2));

				if (numChecks <= 0)
					continue;

				velocity[i] /= static_cast<float>(numChecks);

				for (int j = 0; j < numChecks; ++j) {

					moveCoord(movingBox, velocity[i], i);

					for (const auto& staticCollider : staticBoundings) {
						if (staticCollider)
							staticCollision(movingBox, velocity, staticCollider, i);

						if (Sun::equal_e(velocity[i], 0))
							break;
					}

					for (auto& dynamicCollider : specialBoundings) {
						if (dynamicCollider && movingBox.getEntityID() != dynamicCollider.getEntityID()) {
							if (dynamicCollider.type == STATIC)
								staticCollision(movingBox, velocity, dynamicCollider, i);
							else
								dynamicCollision(movingBox, velocity, dynamicCollider, context);
						}

						if (Sun::equal_e(velocity[i], 0))
							break;
					}
				}

				velocity[i] *= static_cast<float>(numChecks);
			}
		}
	}
}

void CollisionSystem::dynamicCollision(BoundingBox& movingBox, Vector3f& velocity, BoundingBox& otherBox, const std::unique_ptr<GameContext>& context) const {
	if (intersects(movingBox, otherBox)) {
		if (movingBox.getEntityType() == PLAYER && otherBox.type == DYNAMIC) {
			context->addToDestroy(otherBox.getEntityID());
			if (otherBox.getEntityType() == KEY) {
				auto& door = context->getEntityByID(otherBox.getEntityID() - 1);	// como la llave y su puerta se crean consecutivamente, la puerta siempre es (llave.ID - 1)
				door.collider->type = DYNAMIC;
			}
			otherBox.makeUndefined();	// nos ahorramos comprobaciones si hacemos que el sistema ignore la bounding
		}
	}
}

void CollisionSystem::staticCollision(BoundingBox& box, Vector3f& velocity, const BoundingBox& otherBox, const int coord) const {
	if (intersects(box, otherBox)) {
		float offset{0};

		if (velocity[coord] > 0)
			offset = otherBox.min[coord] - box.max[coord];
		else if (velocity[coord] < 0)
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
		if (!(Sun::greater_e(bounding.max[i], other.min[i]) && Sun::less_e(bounding.min[i], other.max[i])))
			return false;

	return true;
}