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
            case KEY_SPACE:
                next->p_func = &InputSystem::space_pressed;
                break;
            case KEY_M:
                next->p_func = &InputSystem::m_pressed;
                break;
            case KEY_LSHIFTIRR :
                next->p_func = &InputSystem::shift_pressed;
                break;
/*			case KEY_LSHIFTGL:
				next->p_func = &InputSystem::shift_pressed;
				break;*/
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

	if (velocity.currentSpeed == velocity.defaultSpeed) {

		velocity.direction = vec3();

		// player siempre tiene character data
		auto & data = *player.getComponent<CharacterData>();

		for (const auto * next = keyMap; next->p_func; ++next)
			if (engine.isKeyPressed(next->key))
				(this->*(next->p_func))(velocity, data);

		// player siempre tiene render
		auto & render = *player.getComponent<Render>();

		if (data.switchingMode)
			data.mode == DEMON ? render.node->setTexture(DEMON_TEXTURE) : render.node->setTexture(ANGEL_TEXTURE);

		const Mouse& mouse = engine.getMouse();
//    std::cout << mouse.position.x << ", " << mouse.position.y << "\n";

		aim_mouse(physics, mouse.position);

		if (mouse.leftPressed) {
			if(!greater_e(data.currentAttackingCooldown, 0.f)) {
				data.attacking = true;
				data.currentAttackingCooldown = data.attackingCooldown;

				soundMessages.emplace_back(PLAYER_SHOOT_EVENT, PLAYER_SHOOT_PARAMETER, data.mode);
			}

//	std::cout << "Click izquierdo\n";
		}

		if (mouse.rightPressed) {
//		std::cout << "Click derecho\n";
		}

		if (velocity.currentSpeed == velocity.defaultSpeed)
			data.dashing = false;
	}

	physics.velocity = normalize(velocity.direction) * velocity.currentSpeed * deltaTime;
}

void InputSystem::w_pressed(Velocity& velocity, CharacterData& data) const { ++ velocity.direction.z; /*std::cout << "W\n";*/ }
void InputSystem::a_pressed(Velocity& velocity, CharacterData& data) const { -- velocity.direction.x; /*std::cout << "A\n";*/ }
void InputSystem::s_pressed(Velocity& velocity, CharacterData& data) const { -- velocity.direction.z; /*std::cout << "S\n";*/ }
void InputSystem::d_pressed(Velocity& velocity, CharacterData& data) const { ++ velocity.direction.x; /*std::cout << "D\n";*/ }
// Dash
void InputSystem::shift_pressed(Velocity& velocity, CharacterData& data) const {
    if(!greater_e(data.currentDashingCooldown, 0.f) && length(velocity.direction) != 0) {
        data.dashing = true;   // TODO : poner esto a false cuando acabe el dash (probablemente es cosa de VelocitySystem)
        data.currentDashingCooldown = data.dashingCooldown;
        velocity.currentSpeed = data.dashSpeed;

        soundMessages.emplace_back(DASH_PLAYER_EVENT);
    }
 //std::cout << "Shift\n";
}
// Shoot
void InputSystem::space_pressed(Velocity& velocity, CharacterData& data) const {
	if(!data.dashing && !greater_e(data.currentAttackingCooldown, 0.f)) {
		data.attacking = true;
		data.currentAttackingCooldown = data.attackingCooldown;

		soundMessages.emplace_back(PLAYER_SHOOT_EVENT, PLAYER_SHOOT_PARAMETER, data.mode);
	}
//	std::cout << "Space\n";
}

// Switch Mode
void InputSystem::m_pressed(Velocity& velocity, CharacterData& data) const {
	if (!greater_e(data.currentSwitchingCooldown, 0)) {
		data.switchingMode = true; // TODO : poner a false switching mode cuando toque (probablemente no se necesite este bool porque solo era necesario para el sonido, y ahora mandamos mensaje)
		data.mode == DEMON ? data.mode = ANGEL : data.mode = DEMON;
		data.currentSwitchingCooldown = data.switchingCooldown;

		soundMessages.emplace_back(data.mode == ANGEL ? ANGEL_CHANGE_EVENT : DEMON_CHANGE_EVENT);
	}
//	std::cout << "M\n";
}

// TODO : llevar cada parte de este codigo a su lugar correspondiente
void InputSystem::aim_mouse(Physics& phy, const glm::vec2& mouse) const {
    const Plane shootingPlane(vec3(0,1,0), phy.position.y);
    const Line  ray (
    		engine.scene->cursorToWorld(mouse.x, mouse.y, 0),
    		engine.scene->cursorToWorld(mouse.x, mouse.y, 1)
	);

    const vec3 intersectPoint = intersectionPoint(shootingPlane, ray);

    // obtenemos la rotacion en y, a partir de la direccion entre el raton y el personaje
    phy.rotation.y = getRotationYfromXZ(intersectPoint - phy.position);
}
