#include <sys/InputSystem.hpp>
#include <util/TexturePaths.hpp>
#include <ent/Entity.hpp>
#include <cassert>
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
    const Line  ray(cursorCoordToWorldCoord(mouse.x, mouse.y, 0), cursorCoordToWorldCoord(mouse.x, mouse.y, 1));

    const vec3 intersectPoint = intersectionPoint(shootingPlane, ray);

    // obtenemos la rotacion en y, a partir de la direccion entre el raton y el personaje
    phy.rotation.y = getRotationYfromXZ(intersectPoint - phy.position);
}

// TODO : llevar cada parte de este codigo a su lugar correspondiente
vec3 InputSystem::cursorCoordToWorldCoord(float x, float y, float far) const {

	glm::mat4x4 projectionMatrix = engine->scene->camera->getProjectionMatrix();
	glm::mat4x4 viewMatrix = engine->scene->camera->getViewMatrix();

    // Deshacemos [projection * view] obteniendo su inversa (para pasar de coordenadas del mundo a la pantalla, hay que hacer projection * view)
    glm::mat4x4 unprojectMatrix = glm::inverse(projectionMatrix * viewMatrix);

    // Por algun motivo OpenGL lee la Y de la pantalla de abajo a arriba, asi que invertimos la y
    y = VIEWPORT_HEIGHT - y;

    // Hay que normalizar las coordenadas entre -1 (izquierda/abajo) y +1 (derecha/arriba)
    const glm::vec4 viewportPos (x / VIEWPORT_WIDTH * 2.0 - 1, y / VIEWPORT_HEIGHT * 2.0 - 1.0, far * 2.0 - 1.0, 1.0);

    // Obtenemos las coordenadas del mundo en funcion de la distancia "far" calculada en viewportPos (profundidad desde el punto de vista de la camara)
    const glm::vec4 worldPos(unprojectMatrix * viewportPos);
    assert(worldPos.w != 0.0); // Avoid a division by zero

    return vec3(worldPos.x, worldPos.y, worldPos.z) / worldPos.w;
}