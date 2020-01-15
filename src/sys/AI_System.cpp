#include <sys/AI_System.hpp>
#include <SunlightEngine/Math.hpp>

// TODO: considerar los estados de la IA como punteros a funcion
void AI_System::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	const Vector3f player_pos = context->getPlayer().physics->position;

	for (auto& ai : std::get<vector<AI>>(context->getComponents(AI_TYPE)))
		targetBehaviour(ai);

	for (const auto& enemy : context->getEntities()) {
		if (enemy.getType() != UNDEFINED && enemy.ai) {
			updateState(enemy, player_pos);
			stateFunctions[enemy.ai->state].p_func(enemy, player_pos, deltaTime);
		}
	}
}

void AI_System::updateState(const Entity& enemy, const Vector3f& player_pos) const {

	Vector3f v_distance = enemy.physics->position - player_pos;
	v_distance.y = 0;

	float distance = v_distance.length();

	if (distance > 50) {
		enemy.ai->state = AI_State::PATROL_STATE;
	} else if (distance > 25) {
		enemy.ai->state = AI_State::PURSUE_STATE;
	} else {
		enemy.ai->state = AI_State::ATTACK_STATE;
	}
}

void AI_System::patrolBehaviour(const Entity& enemy, const Vector3f& player_pos, const float deltaTime) {

	Vector3f distance = enemy.physics->position - enemy.ai->target_position;
	distance.y = 0;

	if (distance.length() < 1.f) {
		enemy.ai->patrol_index = (enemy.ai->patrol_index + 1) % enemy.ai->max_index; // sumo uno a patrol_index y evito que se pase del size del array de patrol_position (max_index)
		enemy.ai->target_position = enemy.ai->patrol_position[enemy.ai->patrol_index];
	}

	seekBehaviour(enemy, enemy.ai->target_position, deltaTime);
	alignBehaviour(enemy, enemy.ai->target_position);
//	std::cout << &enemy << " esta en su patrol\n";
}

void AI_System::pursueBehaviour(const Entity& enemy, const Vector3f& player_pos, const float deltaTime) {
	seekBehaviour(enemy, player_pos, deltaTime);
	alignBehaviour(enemy, player_pos);
//	std::cout << &entity << " esta persiguiendo al player\n";
}

void AI_System::attackBehaviour(const Entity& enemy, const Vector3f& player_pos, const float deltaTime) {
	seekBehaviour(enemy, enemy.physics->position, deltaTime);
	alignBehaviour(enemy, player_pos);
//	std::cout << &entity << " esta atacando al player\n";
}

void AI_System::targetBehaviour(AI& ai) {
	ai.target_position = ai.patrol_position[ai.patrol_index];
}

void AI_System::seekBehaviour(const Entity& enemy, const Vector3f& target, const float deltaTime) {

	enemy.velocity->direction = target - enemy.physics->position;
	enemy.velocity->direction.y = 0;
/*
	if (enemy.ai->state == AI_State::ATTACK_STATE || enemy.velocity->direction.length() < 1.f)
		enemy.physics->velocity = 0;
	else*/
		enemy.physics->velocity = enemy.velocity->direction.normalize() * enemy.velocity->currentSpeed * deltaTime;
}

void AI_System::alignBehaviour(const Entity& enemy, const Vector3f& target) {
	enemy.physics->rotation.y = Sun::nearestAngle(enemy.physics->rotation.y, (enemy.physics->position - target).getRotationYfromXZ());
}