#pragma once

#include <sys/System.hpp>
#include <ent/Entities.hpp>
#include <util/GameContext.hpp>

struct AI_System : System
{
	void init() override {  }
    void update(const std::unique_ptr<GameContext>& context) const override;

	private:

		void updateState(const Entity& enemy, const Vector3f& player_pos) const;

		static void patrolBehaviour(const Entity& enemy, const Vector3f& player_pos);
		static void pursueBehaviour(const Entity& enemy, const Vector3f& player_pos);
		static void attackBehaviour(const Entity& enemy, const Vector3f& player_pos);

		static void targetBehaviour(const Entity& enemy, const Vector3f& player_pos);
		static void seekBehaviour(const Entity& enemy);
		static void alignBehaviour(const Entity& enemy);

	struct TStateFunction {
		void (*p_func)(const Entity& enemy, const Vector3f& player_pos);
	};

	// IMPORTANTE : para acceder a este array hay que : stateFunctions[STATE DEL ENEMY].p_func(parametros)
	// 				Hay que respetar SI O SI el orden de AI_STATE con sus funciones asociadas
	//				Si el orden de AI_STATE es patrol - pursue - X - Y, el orden de las funciones tiene que ser
	//				obligatoriamente patrolFunc - pursueFunc - Xfunc - Yfunc.
	//				Si no se respeta esto, los enemigos se comportaran de manera erratica
	const TStateFunction stateFunctions[AI_State::END_STATE] // END_STATE es el tamaño del array
	{
		{ patrolBehaviour },
		{ pursueBehaviour },
		{ attackBehaviour }
	};
};
