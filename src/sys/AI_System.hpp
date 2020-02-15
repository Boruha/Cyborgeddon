#pragma once

#include <sys/System.hpp>

struct AI_System : System
{
	void init() override {  }
    void update(const std::unique_ptr<GameContext> &context, float deltaTime) override;
	void reset() override {  }

	private:

		static void patrolBehaviour(const Entity& enemy, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&);
		static void pursueBehaviour(const Entity& enemy, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&);
		static void attackBehaviour(const Entity& enemy, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&);

		static void basicBehaviour(const Entity& enemy, const vec3& target, float deltaTime, bool align);

		static void targetBehaviour(AI& ai, const vec3& target);
		static void seekBehaviour(const Entity& enemy, const vec3& target, float deltaTime);
		static void alignBehaviour(const Entity& enemy, const vec3& target);

		//PATHING
		static std::vector<int> calculePath(const int, const int, const std::vector<MapNode>&);
    	static int nearestNode(const vec3&, const std::vector<MapNode>&);

	struct TStateFunction {
		void (*p_func)(const Entity& enemy, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&);
	};

	// IMPORTANTE : para acceder a este array hay que : stateFunctions[STATE DEL ENEMY].p_func(parametros)
	// 				Hay que respetar SI O SI el orden de AI_STATE con sus funciones asociadas
	//				Si el orden de AI_STATE es patrol - pursue - X - Y, el orden de las funciones tiene que ser
	//				obligatoriamente patrolFunc - pursueFunc - Xfunc - Yfunc.
	//				Si no se respeta esto, los enemigos se comportaran de manera erratica
	const TStateFunction stateFunctions[AI_State::END_STATE] // END_STATE es el tamaño del array
	{
		patrolBehaviour,
		pursueBehaviour,
		attackBehaviour
	};
};
