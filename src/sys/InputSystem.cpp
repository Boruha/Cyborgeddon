#include <sys/InputSystem.hpp>
#include <util/TexturePaths.hpp>
#include <ent/Entity.hpp>
#include <SunlightEngine/Device.hpp>
#include <util/Vector2.hpp>
#include <glm/glm.hpp>
#include <cassert>

void InputSystem::init() {
	device.setEventReceiver(&eventReceiver);
}

// TODO: revisar los punteros a funcion. Problema -> distintos parametros para distintas acciones
//  	 posible solucion: usar gamecontext para lo necesario en cada funcion
void InputSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const {
	auto& player = context->getPlayer();

	player.velocity->direction = 0;

	auto* next = const_cast<TKey2func*>(keyMapping);

	while (next->p_func) {
		if (eventReceiver.IsKeyDown(next->key))
			next->p_func(player, deltaTime);
		++next;
	}

    aim_mouse(*player.physics, eventReceiver.getMouse().position);

	if (eventReceiver.getMouse().leftPressed) {
		if(!Sun::greater_e(player.characterData->currentAttackingCooldown, 0.f)) {
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
    if(!Sun::greater_e(player.characterData->currentDashingCooldown, 0.f) && player.velocity->direction != 0) {
        player.characterData->dashing = true;
        player.characterData->currentDashingCooldown = player.characterData->dashingCooldown;
        player.velocity->currentSpeed = player.characterData->dashSpeed;
    }
}
// Shoot
void InputSystem::space_pressed(Entity& player, const float deltaTime) {
	if(!Sun::greater_e(player.characterData->currentAttackingCooldown, 0.f)) {
		player.characterData->attacking = true;
		player.characterData->currentAttackingCooldown = player.characterData->attackingCooldown;
	}
}
// Aim
void InputSystem::left_pressed  (Entity& player, const float deltaTime) { player.physics->rotation.y -= PLAYER_ROTATION_SPEED * deltaTime; }
void InputSystem::right_pressed (Entity& player, const float deltaTime) { player.physics->rotation.y += PLAYER_ROTATION_SPEED * deltaTime; }
// Switch Mode
void InputSystem::m_pressed(Entity& player, const float deltaTime) {
	if (!Sun::greater_e(player.characterData->currentSwitchingCooldown, 0)) {
		player.characterData->switchingMode = true;
		player.characterData->mode == DEMON ? player.characterData->mode = ANGEL : player.characterData->mode = DEMON;
		player.characterData->mode == DEMON ? player.node->get()->setTexture(DEMON_TEXTURE) : player.node->get()->setTexture(ANGEL_TEXTURE);
		player.characterData->currentSwitchingCooldown = player.characterData->switchingCooldown;
	}
}

// TODO : llevar cada parte de este codigo a su lugar correspondiente
void InputSystem::aim_mouse(Physics& phy, const Sun::Vector2u& mouse) const
{
    Vector3f r_a, r_b;

    // Deshacer proyeccion para profundidad = 0 (obtenemos punto a de la recta r)
    // Deshacer proyeccion para profundidad = profundidad de la camara (obtenemos punto b de la recta r)
    cursorCoordToWorldCoord(mouse.x, mouse.y, 0, r_a);
    cursorCoordToWorldCoord(mouse.x, mouse.y, 1, r_b);

    Vector3f n  = Vector3f(0,1,0);   // normal del plano
    Vector3f ba = r_b - r_a; // vector director de la recta

    float n_a  = n.dot(r_a);
    float n_ba = n.dot(ba);

    // obtenemos el punto de interseccion entre recta y plano, siendo el plano (y - altura_disparo = 0)
    Vector3f worldPosition = r_a + (ba * ((phy.position.y - n_a) / n_ba));

    // obtenemos la rotacion en y, a partir de la direccion entre el raton y el personaje
    phy.rotation.y = (worldPosition - phy.position).getRotationYfromXZ();
}

// TODO : llevar cada parte de este codigo a su lugar correspondiente
void InputSystem::cursorCoordToWorldCoord(float x, float y, float far, Vector3f& worldCoordinates) const {
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
    assert(worldPos[3] != 0.0); // Avoid a division by zero

    worldCoordinates.x = worldPos[0] / worldPos[3];
    worldCoordinates.y = worldPos[1] / worldPos[3];
    worldCoordinates.z = worldPos[2] / worldPos[3];
}