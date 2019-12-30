#pragma once

#include <sys/System.hpp>
#include <ent/Entities.hpp>
#include <util/GameContext.hpp>

struct AI_System : System
{
	void init() override {  }
    void update(const std::unique_ptr<GameContext>& context) const override;

	private:

		void updateState(const std::unique_ptr<EntityPlayer> &player, const std::unique_ptr<Entity>& entity) const;

		static void patrolBehaviour(const std::unique_ptr<Entity>& entity, const std::unique_ptr<EntityPlayer>& player);
		static void pursueBehaviour(const std::unique_ptr<Entity>& entity, const std::unique_ptr<EntityPlayer>& player);
		static void attackBehaviour(const std::unique_ptr<Entity>& entity, const std::unique_ptr<EntityPlayer>& player);

		static void targetBehaviour(const std::unique_ptr<Entity>& entity, const std::unique_ptr<EntityPlayer> &player);
		static void seekBehaviour(const std::unique_ptr<Entity>& entity);
		static void alignBehaviour(const std::unique_ptr<Entity>& entity);

	struct TStateFunction {
		void (*p_func)(const std::unique_ptr<Entity>& entity, const std::unique_ptr<EntityPlayer>& player);
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
