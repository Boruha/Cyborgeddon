#include <sys/AI_System.hpp>

// TODO: considerar los estados de la IA como punteros a funcion
void AI_System::update(const std::unique_ptr<GameContext>& context) const {
	updateState(context->getPlayer(), context->getEnemies());

	for (const auto& enemy : context->getEnemies()) {
		stateFunctions[enemy->ai.state].p_func(enemy, context->getPlayer());
		targetBehaviour(enemy, context->getPlayer());
		seekBehaviour(enemy);
		alignBehaviour(enemy);
	}
}

void AI_System::updateState(const std::unique_ptr<EntityPlayer>& player, const std::vector<std::unique_ptr<EntityEnemy>>& enemies) const {
	for (const auto& enemy : enemies) {
		Vector3f v_distance = enemy->transformable.position - player->transformable.position;
		v_distance.y = 0;

		float distance = v_distance.length();

		if (distance > 50) {
			enemy->ai.state = AI_State::PATROL_STATE;
		} else if (distance > 25) {
			enemy->ai.state = AI_State::PURSUE_STATE;
		} else {
			enemy->ai.state = AI_State::ATTACK_STATE;
		}
	}
}

void AI_System::patrolBehaviour(const std::unique_ptr<EntityEnemy> &enemy, const std::unique_ptr<EntityPlayer>& player) {
	Vector3f distance = enemy->transformable.position - enemy->ai.target_position;
	distance.y = 0;

	if (distance.length() < 1.f) {
		enemy->ai.patrol_index = ++(enemy->ai.patrol_index) % enemy->ai.patrol_position.size(); // sumo uno a patrol_index y evito que se pase del size del vector de patrol_position
		enemy->ai.target_position = enemy->ai.patrol_position[enemy->ai.patrol_index];
	}
}

void AI_System::pursueBehaviour(const std::unique_ptr<EntityEnemy> &enemy, const std::unique_ptr<EntityPlayer>& player) {
	std::cout << &enemy << " esta persiguiendo al player\n";
}

void AI_System::attackBehaviour(const std::unique_ptr<EntityEnemy> &enemy, const std::unique_ptr<EntityPlayer> &player) {
	std::cout << &enemy << " esta atacando al player\n";
}

void AI_System::targetBehaviour(const std::unique_ptr<EntityEnemy> &enemy, const std::unique_ptr<EntityPlayer> &player) {
	if (enemy->ai.state == AI_State::PATROL_STATE)
		enemy->ai.target_position = enemy->ai.patrol_position[enemy->ai.patrol_index];
	else
		enemy->ai.target_position = player->transformable.position;
}

void AI_System::seekBehaviour(const std::unique_ptr<EntityEnemy> &enemy) {
	enemy->velocity.direction = enemy->ai.target_position - enemy->transformable.position;
	enemy->velocity.direction.y = 0;

	if (enemy->ai.state == AI_State::ATTACK_STATE)
		enemy->velocity.velocity = 0;
	else
		enemy->velocity.velocity = enemy->velocity.direction.normalize() * enemy->velocity.speed;
}

void AI_System::alignBehaviour(const std::unique_ptr<EntityEnemy> &enemy) {
	enemy->transformable.rotation.y = enemy->velocity.direction.getRotationYfromXZ();
	enemy->node.setRotation(enemy->transformable.rotation);
}