#include <sys/MovementSystem.hpp>

//TODO / CONSIDERACION: 
// 		1. Llamar 'Update()' a un método aux del System que se encarge de llamar a sus metodos.
//		2. Todas las entidades que sufran un cambio o una operacion, debería hacerlo igual los calculos.
//		por eso igual nos conviene crear metodos "Update_X.CMP / Update_X.ACCION".
//			2.1. Antes sería imperativo unificar todas entidades que hagan X acciones o tengan X cmp.
//			2.2. Renombrar los sistemas por la componente que trabajan o por el ambito: transformableSystem / velocitySystem, etc... (estudiable)

void MovementSystem::update(const std::unique_ptr<GameContext>& context) const {

	for (auto& cmp : context->getPhysicsComponents()) {
		cmp.position += cmp.velocity;
		cmp.rotation.y > 0 ? cmp.rotation.y -= 360 : cmp.rotation.y += 360;
	}

	updatePlayerAndCamera(context->getPlayer(), context->getCamera());

	checkMaxDist_Bullet(context->getEntities());
}

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::updatePlayerAndCamera(std::unique_ptr<EntityPlayer>& player, std::unique_ptr<EntityCamera>& camera) const {
    camera->physics->position += player->physics->velocity;

	// TODO: esto es una basura asi como esta hecho, arreglar en el futuro
	// DASH
	if(player->velocity->speed > player->velocity->defaultSpeed)
	    player->velocity->speed -= 1.2f;
	if(player->velocity->speed < player->velocity->defaultSpeed)
	    player->velocity->speed = player->velocity->defaultSpeed;
	// TODO: las entidades deben tener velocidad maxima const, minima const y actual variable
}

// TODO: reconsiderar logica -> las balas nunca se frenan. Para evitar calculos se podria tener una variable
//  	 llamada "distancia recorrida" que fuese sumando speed en cada iteracion y que la condicion de muerte sea
//  	 que esa distancia sea mayor a la distancia maxima de vida de la bala
// BORU: Una consideración buenarda.
void MovementSystem::checkMaxDist_Bullet(const std::vector<std::unique_ptr<Entity>>& entities) const {
	for (const auto& e : entities) {
		if(e->getType() == EntityType::BULLET) {
			auto* bullet = dynamic_cast<EntityBullet*>(e.get());
			bullet->distance_left -= bullet->physics->velocity.length();
			if(bullet->distance_left <= 0)
				bullet->alive.alive = false;
		}
	}
}