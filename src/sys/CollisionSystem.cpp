#include <sys/CollisionSystem.hpp>
#include <Engine/util/Math.hpp>
#include <util/SoundPaths.hpp>
#include <iostream>

// TODO: quiza sea conveniente tener 3 sistemas de colisiones distintos, uno para objetos dinamicos, otro para estaticos y otro para objetos que se muevan muy rapido (ray)

void CollisionSystem::update(const Context &context, const float deltaTime) {
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

	auto& specialBoundings 	= context->getComponents().get<BoundingBox>();
	auto& staticBoundings	= context->getComponents().get<BoundingBox>();

	for (auto& collider : specialBoundings)
		if (collider)					// ignoramos los componentes que no pertenecen a ninguna entidad
			fixBox(collider);

	for (auto& staticCollider : staticBoundings)
		if (staticCollider)				// ignoramos los componentes que no pertenecen a ninguna entidad
			fixBox(staticCollider);
/*
	for (auto& movingBox : specialBoundings) {											// recorremos el array de objetos no estaticos
		if (movingBox && movingBox.velocity) {
			for (int i = 0; i < 3; ++i) {
				vec3& velocity = *movingBox.velocity;

				const int numChecks = std::ceil(std::abs(velocity[i]) / (movingBox.dim[i] / 2));

				if (numChecks <= 0)
					continue;

				velocity[i] /= static_cast<float>(numChecks);

				for (int j = 0; j < numChecks; ++j) {

					moveCoord(movingBox, velocity[i], i);

					for (const auto& staticCollider : staticBoundings) {
						if (staticCollider)
							staticCollision(movingBox, velocity, staticCollider, i);

						if (equal_e(velocity[i], 0))
							break;
					}

					for (auto& dynamicCollider : specialBoundings) {
						if (dynamicCollider && movingBox.getEntityID() != dynamicCollider.getEntityID()) {
							if (dynamicCollider.type == STATIC || (dynamicCollider.getEntityType() == DOOR && movingBox.getEntityType() != PLAYER)) // TODO : cambiar esta condicion, no podemos depender de la puerta y el player. Considerar nuevo componente u otra variable para boundingbox
								staticCollision(movingBox, velocity, dynamicCollider, i);
							else
								dynamicCollision(movingBox, velocity, dynamicCollider, context);

						}

						if (equal_e(velocity[i], 0))
							break;
					}
				}

				velocity[i] *= static_cast<float>(numChecks);
			}
		}
	}*/
}

void CollisionSystem::dynamicCollision(BoundingBox& movingBox, vec3& velocity, BoundingBox& otherBox, const Context& context) const {
	if (intersects(movingBox, otherBox)) {
		if (movingBox.getEntityType() == PLAYER && otherBox.type == DYNAMIC) {
			//context->addToDestroy(otherBox.getEntityID());
            deathMessages.emplace_back(otherBox.getEntityID());

			if (otherBox.getEntityType() == KEY) {
				const auto& door = context->getEntityByID(otherBox.getEntityID() - 1);	// como la llave y su puerta se crean consecutivamente, la puerta siempre es (llave.ID - 1)
				door.collider->type = DYNAMIC;

                soundMessages.emplace_back(PICKUP_KEY_EVENT);   //Creo el SoundMessage de coger una llave

			}

			if(otherBox.getEntityType() == DOOR){
                soundMessages.emplace_back(OPEN_DOOR_EVENT); //Creo el SoundMessage de Abrir Puerta
			}

			otherBox.makeUndefined();	// nos ahorramos comprobaciones si hacemos que el sistema ignore la bounding
		}
	}
}

void CollisionSystem::staticCollision(BoundingBox& box, vec3& velocity, const BoundingBox& otherBox, const int coord) const {
	if (intersects(box, otherBox)) {
		float offset{0};

		if (velocity[coord] > 0)
			offset = otherBox.min[coord] - box.max[coord];
		else if (velocity[coord] < 0)
			offset = otherBox.max[coord] - box.min[coord];

		velocity[coord] += offset;

		moveCoord(box, offset, coord);

		if (equal_e(velocity[coord], 0))
			velocity[coord] = 0;
	}
}

void CollisionSystem::fixCoord(BoundingBox& bounding, const int coord) const {
	const vec3& pos = *bounding.pos;

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

void CollisionSystem::moveBox(BoundingBox& bounding, const vec3& mov) const {
	for (int i = 0; i < 2; ++i)
		bounding[i] += mov;
}

void CollisionSystem::setBox(BoundingBox& bounding, const vec3& pos) const {
	for (int i = 0; i < 3; ++i)
		setCoord(bounding, pos, i);
}

void CollisionSystem::setCoord(BoundingBox& bounding, const vec3& pos, const int coord) const {
	bounding.min[coord] = pos[coord] - (bounding.dim[coord] / 2);
	bounding.max[coord] = pos[coord] + (bounding.dim[coord] / 2);
}

bool CollisionSystem::intersects(const BoundingBox& bounding, const BoundingBox& other) const {
	for (int i = 0; i < 3; ++i)
		if (!(greater_e(bounding.max[i], other.min[i]) && less_e(bounding.min[i], other.max[i])))
			return false;

	return true;
}