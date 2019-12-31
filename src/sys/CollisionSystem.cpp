#include <sys/CollisionSystem.hpp>
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