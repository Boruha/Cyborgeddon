#include <sys/MovementSystem.hpp>
#include <cmath>

//TODO / CONSIDERACION: 
// 		1. Llamar 'Update()' a un método aux del System que se encarge de llamar a sus metodos.
//		2. Todas las entidades que sufran un cambio o una operacion, debería hacerlo igual los calculos.
//		por eso igual nos conviene crear metodos "Update_X.CMP / Update_X.ACCION".
//			2.1. Antes sería imperativo unificar todas entidades que hagan X acciones o tengan X cmp.
//			2.2. Renombrar los sistemas por la componente que trabajan o por el ambito: transformableSystem / velocitySystem, etc... (estudiable)

void MovementSystem::update(const std::unique_ptr<GameContext>& context) const {

	// A la camara hay que darle la velocidad en funcion de la del jugador tras ser modificada por otros sistemas
	context->getCamera().physics->velocity = context->getPlayer().physics->velocity;

	for (auto& cmp : context->getPhysicsComponents()) {
		cmp.position += cmp.velocity;
		cmp.rotation.y = fmodf(cmp.rotation.y, 360);
	}
}

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::updatePlayerAndCamera(EntityPlayer& player, EntityCamera& camera) const {
	// TODO: esto es una basura asi como esta hecho, arreglar en el futuro
	// DASH
	if(player.velocity->speed > player.velocity->defaultSpeed)
	    player.velocity->speed -= 1.2f;
	if(player.velocity->speed < player.velocity->defaultSpeed)
	    player.velocity->speed = player.velocity->defaultSpeed;
	// TODO: las entidades deben tener velocidad maxima const, minima const y actual variable
}