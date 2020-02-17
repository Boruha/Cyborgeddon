#pragma once

#include <sys/System.hpp>

struct AI_System : System
{
	void init() override;
    void update(const std::unique_ptr<GameContext> &context, float deltaTime) override;
	void reset() override {  }

	private:

        void patrolBehaviour(const Entity& enemy, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&) const;
        void pursueBehaviour(const Entity& enemy, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&) const;
        void attackBehaviour(const Entity& enemy, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&) const;

        void basicBehaviour(const Entity& enemy, const vec3& target, float deltaTime, bool align) const;

        void targetBehaviour(AI& ai, const vec3& target) const;
        void seekBehaviour(const Entity& enemy, const vec3& target, float deltaTime) const;
        void alignBehaviour(const Entity& enemy, const vec3& target) const;

		//PATHING
		[[nodiscard]] std::vector<int> calculePath(int, int, const std::vector<MapNode>&) const;
		[[nodiscard]] int nearestNode(const vec3&, const std::vector<MapNode>&) const;

	struct TStateFunction {
		void (AI_System::*p_func)(const Entity& enemy, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&) const;
	};

	// IMPORTANTE :
	// 				Hay que respetar SI O SI el orden de AI_STATE con sus funciones asociadas
	//				Si el orden de AI_STATE es patrol - pursue - X - Y, el orden de las funciones tiene que ser
	//				obligatoriamente patrolFunc - pursueFunc - Xfunc - Yfunc.
	//				Si no se respeta esto, los enemigos se comportaran de manera erratica
	TStateFunction stateFunctions[AI_State::END_STATE] // END_STATE es el tamaño del array
	{
		nullptr,
		nullptr,
		nullptr
	};
};
