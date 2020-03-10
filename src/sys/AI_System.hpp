#pragma once

#include <sys/System.hpp>

struct Selector;

struct AI_System : System
{
<<<<<<< HEAD
	~AI_System() override = default;

	void init() override;
    void update(const Context &context, float deltaTime) override;
	void reset() override {  }

	private:

		constexpr void patrolBehaviour(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&) const;
		void pursueBehaviour(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&) const;
		constexpr void attackBehaviour(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&) const;

		constexpr void basicBehaviour(AI& ai, Physics& phy, Velocity& vel, const vec3& target, float deltaTime, bool align) const;

		constexpr void targetBehaviour(AI& ai, const vec3& target) const;
		constexpr void seekBehaviour(Physics& phy, Velocity& vel, const vec3& target, float deltaTime) const;
		constexpr void alignBehaviour(Physics& phy, const vec3& target) const;

		//PATHING
		[[nodiscard]] std::vector<int> calculePath(int, int, const std::vector<MapNode>&) const;
		[[nodiscard]] int nearestNode(const vec3&, const std::vector<MapNode>&) const;

	struct TStateFunction {
		void (AI_System::*p_func)(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&) const;
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
=======
	void init() final;
    void update(const Context &context, float deltaTime) final;
	void reset() final {  }

protected:
    //PATHING
    [[nodiscard]] std::vector<int> calculePath(int, int, const std::vector<MapNode>&) const;
    [[nodiscard]] int nearestNode(const vec3&, const std::vector<MapNode>&) const;

private:
    /* Behaviour Tree */
    std::unique_ptr<Selector> root;
};

/* BEHAVIOUR STRUCTURE DEF */
struct BehaviourNode : AI_System
{
    virtual bool run(AI&, Physics&, CharacterData&, Velocity&, const vec3&, float, const std::unique_ptr<GameContext>&) = 0;
};

struct CompoundNode : BehaviourNode
{
    CompoundNode()  { childs.reserve(3); }
    ~CompoundNode() { childs.clear(); }

    /* FUNCTIONS */
    //void addChild(std::unique_ptr<BehaviourNode>& p_BeNode) { childs.push_back(p_BeNode); }
    const std::vector<std::unique_ptr<BehaviourNode>>& getChilds() const { return childs; };

    /* DATA */
    std::vector<std::unique_ptr<BehaviourNode>> childs;
};

struct Sequence : CompoundNode
{
    /* FUNCTIONS */
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        for(auto& ref_child : getChilds())
        {
            if(!ref_child.get()->run(ai, phy, data, vel, player_pos, deltaTime, context))
                return false;
        }
        return true;
    }
>>>>>>> Behaviour_Tree
};

struct Selector : CompoundNode
{
    /* FUNCTIONS */
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        for(auto& ref_child : getChilds())
        {
            if(ref_child.get()->run(ai, phy, data, vel, player_pos, deltaTime, context))
                return true;
        }
        return false;
    }
};

