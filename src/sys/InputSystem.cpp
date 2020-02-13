#include <sys/InputSystem.hpp>
#include <util/TexturePaths.hpp>
#include <cassert>
#include <Engine/EngineInterface/IEngine.hpp>
#include <Engine/util/MathIntersection.hpp>
#include <Engine/EngineInterface/SceneInterface/ICameraNode.hpp>
#include <util/GameConstants.hpp>

void InputSystem::init() {

}

// TODO: revisar los punteros a funcion. Problema -> distintos parametros para distintas acciones
//  	 posible solucion: usar gamecontext para lo necesario en cada funcion
void InputSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const {
	auto& player = context->getPlayer();

	player.velocity->direction = vec3();

	for (const auto * next = keyMapping; next->p_func; ++next)
		if (engine->isKeyPressed(next->key))
			next->p_func(player, deltaTime);

	const Mouse& mouse = engine->getMouse();

	aim_mouse(*player.physics, mouse.position);

	if (mouse.leftPressed) {
		if(!greater_e(player.characterData->currentAttackingCooldown, 0.f)) {
			player.characterData->attacking = true;
			player.characterData->currentAttackingCooldown = player.characterData->attackingCooldown;
		}
	}

	player.physics->velocity = normalize(player.velocity->direction) * player.velocity->currentSpeed * deltaTime;
}

void InputSystem::w_pressed(Entity& player, const float deltaTime) { ++player.velocity->direction.z; }
void InputSystem::a_pressed(Entity& player, const float deltaTime) { --player.velocity->direction.x; }
void InputSystem::s_pressed(Entity& player, const float deltaTime) { --player.velocity->direction.z; }
void InputSystem::d_pressed(Entity& player, const float deltaTime) { ++player.velocity->direction.x; }
// Dash
void InputSystem::shift_pressed(Entity& player, const float deltaTime) {
    if(!greater_e(player.characterData->currentDashingCooldown, 0.f) && length(player.velocity->direction) != 0) {
        player.characterData->dashing = true;
        player.characterData->currentDashingCooldown = player.characterData->dashingCooldown;
        player.velocity->currentSpeed = player.characterData->dashSpeed;
    }
}
// Shoot
void InputSystem::space_pressed(Entity& player, const float deltaTime) {
	if(!greater_e(player.characterData->currentAttackingCooldown, 0.f)) {
		player.characterData->attacking = true;
		player.characterData->currentAttackingCooldown = player.characterData->attackingCooldown;
	}
}
// Aim
void InputSystem::left_pressed  (Entity& player, const float deltaTime) { player.physics->rotation.y -= PLAYER_ROTATION_SPEED * deltaTime; }
void InputSystem::right_pressed (Entity& player, const float deltaTime) { player.physics->rotation.y += PLAYER_ROTATION_SPEED * deltaTime; }
// Switch Mode
void InputSystem::m_pressed(Entity& player, const float deltaTime) {
	if (!greater_e(player.characterData->currentSwitchingCooldown, 0)) {
		player.characterData->switchingMode = true;
		player.characterData->mode == DEMON ? player.characterData->mode = ANGEL : player.characterData->mode = DEMON;
		player.characterData->mode == DEMON ? player.inode->get()->setTexture(DEMON_TEXTURE) : player.inode->get()->setTexture(ANGEL_TEXTURE);
		player.characterData->currentSwitchingCooldown = player.characterData->switchingCooldown;
	}
}

// TODO : llevar cada parte de este codigo a su lugar correspondiente
void InputSystem::aim_mouse(Physics& phy, const vec2& mouse) const {
    const Plane shootingPlane(vec3(0,1,0), phy.position.y);
    const Line  ray(engine->scene->cursorToWorld(mouse.x, mouse.y, 0), engine->scene->cursorToWorld(mouse.x, mouse.y, 1));

    const vec3 intersectPoint = intersectionPoint(shootingPlane, ray);

    // obtenemos la rotacion en y, a partir de la direccion entre el raton y el personaje
    phy.rotation.y = getRotationYfromXZ(intersectPoint - phy.position);
}
