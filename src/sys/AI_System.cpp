#include <sys/AI_System.hpp>
#include <Engine/util/Math.hpp>
#include <util/SystemConstants.hpp>

// TODO: considerar los estados de la IA como punteros a funcion
void AI_System::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	const vec3 player_pos = context->getPlayer().physics->position;

	for (const auto& enemy : context->getEntities()) {
		if (enemy && enemy.ai) {
			vec3 v_distance = enemy.physics->position - player_pos;
			v_distance.y = 0;

			float distance = length(v_distance);

			if (greater_e(distance, PATROL_MIN_DISTANCE))
				stateFunctions[enemy.ai->state = PATROL_STATE].p_func(enemy, player_pos, deltaTime);
			else if (greater_e(distance, PURSUE_MIN_DISTANCE))
				stateFunctions[enemy.ai->state = PURSUE_STATE].p_func(enemy, player_pos, deltaTime);
			else if (greater_e(distance, ATTACK_MIN_DISTANCE))
				stateFunctions[enemy.ai->state = ATTACK_STATE].p_func(enemy, player_pos, deltaTime);
		}
	}
}

void AI_System::patrolBehaviour(const Entity& enemy, const vec3& player_pos, float deltaTime) {
	vec3 distance = enemy.physics->position - enemy.ai->target_position;
	distance.y = 0;

	if (greater_e(length(distance), ARRIVED_MIN_DISTANCE)) {
		basicBehaviour(enemy, enemy.ai->patrol_position[enemy.ai->patrol_index], deltaTime, true);
	} else {
		enemy.ai->patrol_index = (enemy.ai->patrol_index + 1) % enemy.ai->max_index; // sumo uno a patrol_index y evito que se pase del size del array de patrol_position (max_index)

		basicBehaviour(enemy, enemy.ai->patrol_position[enemy.ai->patrol_index], 0, false);
	}
//	std::cout << &enemy << " esta en su patrol\n";
}

void AI_System::pursueBehaviour(const Entity& enemy, const vec3& player_pos, const float deltaTime) {
	basicBehaviour(enemy, player_pos, deltaTime, true);
//	std::cout << &entity << " esta persiguiendo al player\n";
}

void AI_System::attackBehaviour(const Entity& enemy, const vec3& player_pos, const float deltaTime) {
	basicBehaviour(enemy, player_pos, 0, true);

    if(!greater_e(enemy.characterData->currentAttackingCooldown, 0.f)) {
        enemy.characterData->attacking = true;
        enemy.characterData->currentAttackingCooldown = enemy.characterData->attackingCooldown;
    }
//	std::cout << &entity << " esta atacando al player\n";
}

void AI_System::basicBehaviour(const Entity& enemy, const vec3& target, const float deltaTime, const bool align) {
	targetBehaviour(*enemy.ai, target);

	seekBehaviour(enemy, target, deltaTime);

	if (align)
		alignBehaviour(enemy, target);
}

void AI_System::targetBehaviour(AI& ai, const vec3& target) {
	ai.target_position = target;
}

void AI_System::seekBehaviour(const Entity& enemy, const vec3& target, const float deltaTime) {
	enemy.velocity->direction = target - enemy.physics->position;
	enemy.velocity->direction.y = 0;

	enemy.physics->velocity = normalize(enemy.velocity->direction) * enemy.velocity->currentSpeed * deltaTime;
}

void AI_System::alignBehaviour(const Entity& enemy, const vec3& target) {
	enemy.physics->rotation.y = nearestAngle(enemy.physics->rotation.y, getRotationYfromXZ(enemy.physics->position - target));
}