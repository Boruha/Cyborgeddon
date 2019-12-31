#include <sys/AI_System.hpp>

// TODO: considerar los estados de la IA como punteros a funcion
void AI_System::update(const std::unique_ptr<GameContext>& context) const {
	for (const auto& e : context->getEntities()) {
		if (e->getType() == EntityType::ENEMY) {
			auto* enemy = dynamic_cast<EntityEnemy*>(e.get());
			updateState(context->getPlayer(), e);
			targetBehaviour(e, context->getPlayer());
			stateFunctions[enemy->ai->state].p_func(e, context->getPlayer());
			seekBehaviour(e);
			alignBehaviour(e);
		}
	}
}

void AI_System::updateState(const EntityPlayer& player, const std::unique_ptr<Entity>& entity) const {
	auto* enemy = dynamic_cast<EntityEnemy*>(entity.get());

	Vector3f v_distance = enemy->physics->position - player.physics->position;
	v_distance.y = 0;

	float distance = v_distance.length();

	if (distance > 50) {
		enemy->ai->state = AI_State::PATROL_STATE;
	} else if (distance > 25) {
		enemy->ai->state = AI_State::PURSUE_STATE;
	} else {
		enemy->ai->state = AI_State::ATTACK_STATE;
	}
}

void AI_System::patrolBehaviour(const std::unique_ptr<Entity>& entity, const EntityPlayer& player) {
	auto* enemy = dynamic_cast<EntityEnemy*>(entity.get());

	Vector3f distance = enemy->physics->position - enemy->ai->target_position;
	distance.y = 0;

	if (distance.length() < 1.f) {
		enemy->ai->patrol_index = (enemy->ai->patrol_index + 1) % enemy->ai->patrol_position.size(); // sumo uno a patrol_index y evito que se pase del size del vector de patrol_position
		enemy->ai->target_position = enemy->ai->patrol_position[enemy->ai->patrol_index];
	}

//	std::cout << &enemy << " esta en su patrol\n";
}

void AI_System::pursueBehaviour(const std::unique_ptr<Entity>& entity, const EntityPlayer& player) {
//	std::cout << &entity << " esta persiguiendo al player\n";
}

void AI_System::attackBehaviour(const std::unique_ptr<Entity>& entity, const EntityPlayer& player) {
//	std::cout << &entity << " esta atacando al player\n";
}

void AI_System::targetBehaviour(const std::unique_ptr<Entity>& entity, const EntityPlayer& player) {
	auto* enemy = dynamic_cast<EntityEnemy*>(entity.get());

	if (enemy->ai->state == AI_State::PATROL_STATE)
		enemy->ai->target_position = enemy->ai->patrol_position[enemy->ai->patrol_index];
	else
		enemy->ai->target_position = player.physics->position;
}

void AI_System::seekBehaviour(const std::unique_ptr<Entity>& entity) {
	auto* enemy = dynamic_cast<EntityEnemy*>(entity.get());

	enemy->velocity->direction = enemy->ai->target_position - enemy->physics->position;
	enemy->velocity->direction.y = 0;

	if (enemy->ai->state == AI_State::ATTACK_STATE || enemy->velocity->direction.length() < 1.f)
		enemy->physics->velocity = 0;
	else
		enemy->physics->velocity = enemy->velocity->direction.normalize() * enemy->velocity->speed;
}

void AI_System::alignBehaviour(const std::unique_ptr<Entity>& entity) {
	auto* enemy = dynamic_cast<EntityEnemy*>(entity.get());

	enemy->physics->rotation.y = enemy->velocity->direction.getRotationYfromXZ();
}