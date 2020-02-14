#include <sys/InputSystem.hpp>
#include <util/TexturePaths.hpp>
#include <util/SoundPaths.hpp>
#include <Engine/EngineInterface/IEngine.hpp>
#include <Engine/util/MathIntersection.hpp>
#include <Engine/EngineInterface/SceneInterface/ICameraNode.hpp>
#include <Engine/util/Mouse.hpp>
#include <iostream>

void InputSystem::init() {
    for (auto * next = keyMap; next->key != static_cast<KEY_CODE>(0); ++next) {
        switch (next->key) {
            case KEY_W:
                next->p_func = &InputSystem::w_pressed;
                break;
            case KEY_A:
                next->p_func = &InputSystem::a_pressed;
                break;
            case KEY_S:
                next->p_func = &InputSystem::s_pressed;
                break;
            case KEY_D:
                next->p_func = &InputSystem::d_pressed;
                break;
            case KEY_LSHIFT:
                next->p_func = &InputSystem::shift_pressed;
                break;
            case KEY_SPACE:
                next->p_func = &InputSystem::space_pressed;
                break;
            case KEY_M:
                next->p_func = &InputSystem::m_pressed;
                break;
            default : ;
        }
    }
}

// TODO: revisar los punteros a funcion. Problema -> distintos parametros para distintas acciones
//  	 posible solucion: usar gamecontext para lo necesario en cada funcion
void InputSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) {
	auto& player = context->getPlayer();

	player.velocity->direction = vec3();

	for (const auto * next = keyMap; next->p_func; ++next)
		if (engine->isKeyPressed(next->key))
            (this->*(next->p_func))(player, deltaTime);

    const Mouse& mouse = engine->getMouse();

	aim_mouse(*player.physics, mouse.position);

	std::cout << *player.characterData << std::endl;

	if (mouse.leftPressed) {
		if(!greater_e(player.characterData->currentAttackingCooldown, 0.f)) {
			player.characterData->attacking = true;
			player.characterData->currentAttackingCooldown = player.characterData->attackingCooldown;

            soundMessages.emplace_back(player.characterData->mode == ANGEL ? ANGEL_SHOOT_EVENT : DEMON_SHOOT_EVENT);
		}
	}

	player.physics->velocity = normalize(player.velocity->direction) * player.velocity->currentSpeed * deltaTime;
}

void InputSystem::w_pressed(Entity& player, const float deltaTime) const { ++player.velocity->direction.z; }
void InputSystem::a_pressed(Entity& player, const float deltaTime) const { --player.velocity->direction.x; }
void InputSystem::s_pressed(Entity& player, const float deltaTime) const { --player.velocity->direction.z; }
void InputSystem::d_pressed(Entity& player, const float deltaTime) const { ++player.velocity->direction.x; }
// Dash
void InputSystem::shift_pressed(Entity& player, const float deltaTime) const {
    if(!greater_e(player.characterData->currentDashingCooldown, 0.f) && length(player.velocity->direction) != 0) {
        player.characterData->dashing = true;   // TODO : poner esto a false cuando acabe el dash (probablemente es cosa de VelocitySystem)
        player.characterData->currentDashingCooldown = player.characterData->dashingCooldown;
        player.velocity->currentSpeed = player.characterData->dashSpeed;

        soundMessages.emplace_back(DASH_PLAYER_EVENT);
    }
}
// Shoot
void InputSystem::space_pressed(Entity& player, const float deltaTime) const {
	if(!greater_e(player.characterData->currentAttackingCooldown, 0.f)) {
		player.characterData->attacking = true;
		player.characterData->currentAttackingCooldown = player.characterData->attackingCooldown;

		soundMessages.emplace_back(player.characterData->mode == ANGEL ? ANGEL_SHOOT_EVENT : DEMON_SHOOT_EVENT);
	}
}

// Switch Mode
void InputSystem::m_pressed(Entity& player, const float deltaTime) const {
	if (!greater_e(player.characterData->currentSwitchingCooldown, 0)) {
		player.characterData->switchingMode = true; // TODO : poner a false switching mode cuando toque (probablemente no se necesite este bool porque solo era necesario para el sonido, y ahora mandamos mensaje)
		player.characterData->mode == DEMON ? player.characterData->mode = ANGEL : player.characterData->mode = DEMON;
		player.characterData->mode == DEMON ? player.inode->get()->setTexture(DEMON_TEXTURE) : player.inode->get()->setTexture(ANGEL_TEXTURE);
		player.characterData->currentSwitchingCooldown = player.characterData->switchingCooldown;

		soundMessages.emplace_back(player.characterData->mode == ANGEL ? ANGEL_CHANGE_EVENT : DEMON_CHANGE_EVENT);
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
