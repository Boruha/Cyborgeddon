#include <sys/InputSystem.hpp>
#include <util/TexturePaths.hpp>
#include <Engine/EngineInterface/IEngine.hpp>
#include <Engine/util/MathIntersection.hpp>
#include <Engine/EngineInterface/SceneInterface/ICameraNode.hpp>
#include <Engine/util/Mouse.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

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
            case KEY_SPACE:
                next->p_func = &InputSystem::space_pressed;
                break;
            case KEY_Q:
                next->p_func = &InputSystem::q_pressed;
                break;
			case KEY_LSHIFT:
				next->p_func = &InputSystem::shift_pressed;
				break;
            default : ;
        }
    }
}

// TODO: revisar los punteros a funcion. Problema -> distintos parametros para distintas acciones
//  	 posible solucion: usar gamecontext para lo necesario en cada funcion
void InputSystem::update(const Context& context, const float deltaTime) {

	auto & player    = context->getPlayer();

	// player siempre tiene velocity y physics
	auto & velocity  = *player.getComponent<Velocity>();
	auto & physics   = *player.getComponent<Physics>();

	if (velocity.currentSpeed == velocity.defaultSpeed)
	{
		velocity.direction = vec3();

		auto & data     = *player.getComponent<CharacterData>();

		data.dashing = false;

		for (const auto * next = keyMap; next->p_func; ++next)
			if (context->isKeyPressed(next->key))
				(this->*(next->p_func))(velocity, data);

		// player siempre tiene render
		auto & render = *player.getComponent<Render>();

		if (data.switchingMode)
			data.mode == DEMON ? render.node->setTexture(DEMON_TEXTURE) : render.node->setTexture(ANGEL_TEXTURE);

		const Mouse& mouse = context->getMouse();
//    std::cout << mouse.position.x << ", " << mouse.position.y << "\n";

		aim_mouse(context, physics, mouse.position);

		if (mouse.leftPressed)
		{
			if(!data.dashing && !greater_e(data.currentAttackingCooldown, 0.f))
			{
				data.attacking = true;
				data.currentAttackingCooldown = data.attackingCooldown;

				if (data.mode == DEMON)
					soundMessages.emplace_back(ATTACK_PLAYER_DEMON);
				else
					soundMessages.emplace_back(ATTACK_PLAYER_ANGEL);
			}
//	std::cout << "Click izquierdo\n";
		}

		if (mouse.rightPressed)
		{
//		std::cout << "Click derecho\n";
		}
	}

	physics.velocity = normalize(velocity.direction) * velocity.currentSpeed * deltaTime;
}

void InputSystem::w_pressed(Velocity& velocity, CharacterData& data) const { -- velocity.direction.z; /*std::cout << "W\n";*/ }
void InputSystem::a_pressed(Velocity& velocity, CharacterData& data) const { -- velocity.direction.x; /*std::cout << "A\n";*/ }
void InputSystem::s_pressed(Velocity& velocity, CharacterData& data) const { ++ velocity.direction.z; /*std::cout << "S\n";*/ }
void InputSystem::d_pressed(Velocity& velocity, CharacterData& data) const { ++ velocity.direction.x; /*std::cout << "D\n";*/ }
// Dash
void InputSystem::shift_pressed(Velocity& velocity, CharacterData& data) const {
    if(!greater_e(data.currentDashingCooldown, 0.f) && length(velocity.direction) != 0)
    {
        data.dashing = true;   // TODO : poner esto a false cuando acabe el dash (probablemente es cosa de VelocitySystem)
        data.currentDashingCooldown = data.dashingCooldown;
        velocity.currentSpeed = data.dashSpeed;

        soundMessages.emplace_back(DASH_PLAYER);
    }
// std::cout << "Shift\n";
}
// Shoot
void InputSystem::space_pressed(Velocity& velocity, CharacterData& data) const {
	if(!data.dashing && !greater_e(data.currentAttackingCooldown, 0.f))
	{
		data.attacking = true;
		data.currentAttackingCooldown = data.attackingCooldown;

        if (data.mode == DEMON)
            soundMessages.emplace_back(ATTACK_PLAYER_DEMON);
        else
            soundMessages.emplace_back(ATTACK_PLAYER_ANGEL);

    }
	std::cout << "Space\n";
}

// Switch Mode
void InputSystem::q_pressed(Velocity& velocity, CharacterData& data) const {
	if (!greater_e(data.currentSwitchingCooldown, 0)) {
		data.switchingMode = true; // TODO : poner a false switching mode cuando toque (probablemente no se necesite este bool porque solo era necesario para el sonido, y ahora mandamos mensaje)
		data.mode == DEMON ? data.mode = ANGEL : data.mode = DEMON;
		data.currentSwitchingCooldown = data.switchingCooldown;

        if (data.mode == 0)
            soundMessages.emplace_back(SWITCH_MODE_DEMON);
        else
            soundMessages.emplace_back(SWITCH_MODE_ANGEL);

	}
//	std::cout << "M\n";
}

// TODO : llevar cada parte de este codigo a su lugar correspondiente
inline void InputSystem::aim_mouse(const Context& context, Physics& phy, const glm::vec2& mouse) const {
    const Plane shootingPlane(vec3(0,1,0), phy.position.y);
    const Line  ray (
    		context->getWorldPosFromCursor(mouse.x, mouse.y, 0),
		    context->getWorldPosFromCursor(mouse.x, mouse.y, 1)
	);

	const vec3 intersectPoint = intersectionPoint(shootingPlane, ray);

    // obtenemos la rotacion en y, a partir de la direccion entre el raton y el personaje
    phy.rotation.y = getRotationYfromXZ(intersectPoint - phy.position);
//    std::cout << phy.rotation.y << std::endl;
}
