#include <sys/InputSystem.hpp>
#include <util/TexturePaths.hpp>
#include <ent/Entity.hpp>
#include <SunlightEngine/Device.hpp>
#include <glm/glm.hpp>
#include <cassert>
#include <Engine/util/MathIntersection.hpp>

void InputSystem::init() {
	device.setEventReceiver(&eventReceiver);
}

// TODO: revisar los punteros a funcion. Problema -> distintos parametros para distintas acciones
//  	 posible solucion: usar gamecontext para lo necesario en cada funcion
void InputSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const {
	auto& player = context->getPlayer();

	player.velocity->direction = 0;

	auto * next = keyMapping;

	while (next->p_func) {
		if (eventReceiver.IsKeyDown(next->key))
			next->p_func(player, deltaTime);
		++next;
	}

    aim_mouse(*player.physics, eventReceiver.getMouse().position);

	if (eventReceiver.getMouse().leftPressed) {
		if(!greater_e(player.characterData->currentAttackingCooldown, 0.f)) {
			player.characterData->attacking = true;
			player.characterData->currentAttackingCooldown = player.characterData->attackingCooldown;
		}
	}

	player.physics->velocity = player.velocity->direction.normalize() * player.velocity->currentSpeed * deltaTime;
}

void InputSystem::w_pressed(Entity& player, const float deltaTime) { ++player.velocity->direction.z; }
void InputSystem::a_pressed(Entity& player, const float deltaTime) { --player.velocity->direction.x; }
void InputSystem::s_pressed(Entity& player, const float deltaTime) { --player.velocity->direction.z; }
void InputSystem::d_pressed(Entity& player, const float deltaTime) { ++player.velocity->direction.x; }
// Dash
void InputSystem::shift_pressed(Entity& player, const float deltaTime) {
    if(!greater_e(player.characterData->currentDashingCooldown, 0.f) && player.velocity->direction != 0) {
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
		player.characterData->mode == DEMON ? player.node->get()->setTexture(DEMON_TEXTURE) : player.node->get()->setTexture(ANGEL_TEXTURE);
		player.characterData->currentSwitchingCooldown = player.characterData->switchingCooldown;
	}
}

// TODO : llevar cada parte de este codigo a su lugar correspondiente
void InputSystem::aim_mouse(Physics& phy, const Vector2u& mouse) const
{
    const Plane shootingPlane(Vector3f(0,1,0), phy.position.y);
    const Line  ray(cursorCoordToWorldCoord(mouse.x, mouse.y, 0), cursorCoordToWorldCoord(mouse.x, mouse.y, 1));

    const Vector3f intersectPoint = intersectionPoint(shootingPlane, ray);

    // obtenemos la rotacion en y, a partir de la direccion entre el raton y el personaje
    phy.rotation.y = (intersectPoint - phy.position).getRotationYfromXZ();
}

// TODO : llevar cada parte de este codigo a su lugar correspondiente
Vector3f InputSystem::cursorCoordToWorldCoord(float x, float y, float far) const {
    auto proj = device.getInnerDevice()->getSceneManager()->getActiveCamera()->getProjectionMatrix();

    glm::mat4x4 projectionMatrix = glm::mat4x4(
            proj[0], proj[1], proj[2], proj[3],
            proj[4], proj[5], proj[6], proj[7],
            proj[8], proj[9], proj[10], proj[11],
            proj[12], proj[13], proj[14], proj[15]);

    auto mview = device.getInnerDevice()->getSceneManager()->getActiveCamera()->getViewMatrix();

    glm::mat4x4 viewMatrix = glm::mat4x4(
            mview[0], mview[1], mview[2], mview[3],
            mview[4], mview[5], mview[6], mview[7],
            mview[8], mview[9], mview[10], mview[11],
            mview[12], mview[13], mview[14], mview[15]);

    // Deshacemos [projection * view] obteniendo su inversa (para pasar de coordenadas del mundo a la pantalla, hay que hacer projection * view)
    glm::mat4x4 unprojectMatrix = glm::inverse(projectionMatrix * viewMatrix);

    auto viewport = device.getInnerDevice()->getVideoDriver()->getViewPort();

    // Por algun motivo OpenGL lee la Y de la pantalla de abajo a arriba, asi que invertimos la y
    y = viewport.getHeight() - y;

    // Hay que normalizar las coordenadas entre -1 (izquierda/abajo) y +1 (derecha/arriba)
    glm::vec4 viewportPos (x / viewport.getWidth() * 2.0 - 1, y / viewport.getHeight() * 2.0 - 1.0, far * 2.0 - 1.0, 1.0);

    // Obtenemos las coordenadas del mundo en funcion de la distancia "far" calculada en viewportPos (profundidad desde el punto de vista de la camara)
    glm::vec4 worldPos(unprojectMatrix * viewportPos);
    assert(worldPos.w != 0.0); // Avoid a division by zero

    return Vector3(worldPos.x, worldPos.y, worldPos.z) / worldPos.w;
}