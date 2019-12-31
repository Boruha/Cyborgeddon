#include <sys/CollisionSystem.hpp>
#include <algorithm>
#include <SunlightEngine/Vector3.hpp>

//Estrategias para implementar la colision

/*         1-Lenyendo teclas
 *
 * Si pulsamos W -> player.velocity.velocity.Z =  1
 * Si pulsamos A -> player.velocity.velocity.Z = -1
 * Si pulsamos S -> player.velocity.velocity.X =  1
 * Si pulsamos D -> player.velocity.velocity.Z = -1
 */

/*         2-Comprobando la cara del cuadrado que esta chochando
 *
 * Si choca la cara delantera  -> player.velocity.velocity.Z =  1
 * Si choca la cara trasera    -> player.velocity.velocity.Z = -1
 * Si chocha la cara izquierda -> player.velocity.velocity.X =  1
 * Si choca la cara derecha    -> player.velocity.velocity.Z = -1
 */

/*         3-(la mas viable para mi) - Guardar ultima posicion
 * Consistiría en almacenar la ultima posicion anterior a la colision y restaurarla
 * en caso de que haya una. Ahorraria mogollon de comprobaciones tontas.
 */

/*
void MovementSystem::move(const std::vector<std::unique_ptr<EntityPlayer>>& players) {
    for(auto & player : players)
    {
        // CUIDADO -> lo que vemos en la escena es el NODO, si no modificamos player.nodo, no se va a mover nada
        player->transformable.position = player->node.getPosition();

        player->velocity.velocity = player->velocity.direccion.normalize() * player->velocity.speed;
        player->transformable.position += player->velocity.velocity;

        player->node.setPosition(player->transformable.position);
    }
}
*/

// TODO: separar colisiones estaticas (donde se corrige posicion) de las simples (solo comprobamos si colisiona)

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

	for (auto& collider : context->getBoundingComponents())
		if (collider.getEntityType() != UNDEFINED && collider.getEntityType() != playerBox.getEntityType())	// ahora solo usamos al player, en un futuro esto cambiara
			typeFunctions[collider.type].p_func(playerBox, playerPhysics, collider, context);
}

// TODO: considerar matar al enemigo mas cercano a la bala y no el primero en el vector de enemigos
void CollisionSystem::update(std::vector<EntityEnemy>& enemies, std::vector<EntityBullet>& bullets) const {
	for (auto& bullet : bullets) {
		for (auto& enemy : enemies) {
			if (intersects(*enemy.collider, *bullet.collider)) {
				enemy.physics->velocity = 0;
				bullet.physics->velocity = 0;
				break;									// la bala muere, no seguimos comprobando con esa bala
			}
		}
	}
}

void CollisionSystem::update(EntityPlayer& player, const std::vector<EntityEnemy>& enemies) const {
	for (const auto& enemy : enemies) {
		if (intersects(*player.collider, *enemy.collider)) {
			player.health--;
		}
	}
}

void CollisionSystem::update(const EntityPlayer& player, const std::vector<EntityWall>& walls) const {
	for (const auto& wall : walls) {
		for(int i = 0; i < 3; ++i) {
			if (player.physics->velocity[i] != 0) {
				moveCoord(*player.collider, player.physics->velocity[i], i);

				float offset {0};

				if (intersects(*player.collider, *wall.collider)) {
					if (player.physics->velocity[i] > 0)
						offset = wall.collider->min[i] - player.collider->max[i];
					else
						offset = wall.collider->max[i] - player.collider->min[i];

					player.physics->velocity[i] += offset;
					moveCoord(*player.collider, offset, i);
				}
			}
		}
		// tras la colision con la pared que es estatica, corregimos collider
		fixBox(*player.collider);
	}
}

void CollisionSystem::update(const std::vector<EntityDoor>& doors, std::vector<EntityBullet>& bullets) const {
	for (auto& bullet : bullets) {
		for (const auto& door : doors) {
			if (intersects(*door.collider,*bullet.collider)) {
				bullet.physics->velocity = 0;
				break;					// la bala muere, no seguimos comprobando con otras puertas
			}
		}
	}
}

void CollisionSystem::update(const std::vector<EntityWall>& walls, std::vector<EntityBullet>& bullets) const {
	for (auto& bullet : bullets) {
		for (const auto& wall : walls) {
			if (intersects(*wall.collider,*bullet.collider)) {
				bullet.physics->velocity = 0;
				break;					// la bala muere, no seguimos comprobando con otras paredes
			}
		}
	}
}

// TODO: GENERALIZAR, SE REPITE CASI TODO (ideas: bool que "mata" entidades, herencias, )

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

void CollisionSystem::moveCoord(BoundingBox& bounding, const float mov, const int coord) {
	for (int i = 0; i < 2; ++i)
		bounding[i][coord] += mov;
}

bool CollisionSystem::intersects(const BoundingBox& bounding, const BoundingBox& other) {
	for (int i = 0; i < 3; ++i)
		if(bounding.max[i] <= other.min[i] || bounding.min[i] >= other.max[i])
			return false;

	return true;
}

void CollisionSystem::dynamicCollision(BoundingBox& playerBox, Physics& physics, const BoundingBox& otherBox, const std::unique_ptr<GameContext>& context) {
	if (intersects(playerBox, otherBox)) {
		context->addToDestroy(otherBox.getEntityID()); // ahora mismo muere cualquier entidad con bounding dinamico
		if (otherBox.getEntityType() == EntityType::KEY) {
			for(auto& b : context->getBoundingComponents())
				if(b.getID() == otherBox.getID() - 1)
					b.type = ColliderType::DYNAMIC;
		}
	}
}

void CollisionSystem::staticCollision(BoundingBox& box, Physics& physics, const BoundingBox& otherBox, const std::unique_ptr<GameContext>& context) {
	for(int i = 0; i < 3; ++i) {
		if (physics.velocity[i] != 0) {
			moveCoord(box, physics.velocity[i], i);

			float offset;

			if (intersects(box, otherBox)) {
				if (physics.velocity[i] > 0)
					offset = otherBox.min[i] - box.max[i];
				else
					offset = otherBox.max[i] - box.min[i];

				physics.velocity[i] += offset;
				moveCoord(box, offset, i);
			}
		}
		fixBox(box);
	}
}

void CollisionSystem::rayCollision(BoundingBox& box, Physics& physics, const BoundingBox& otherBox, const std::unique_ptr<GameContext>& context) {

}
