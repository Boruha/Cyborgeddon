#pragma once

#include <sys/System.hpp>

struct Selector;

struct AI_System : System
{
	void init() final;
    void update(const Context &context, float deltaTime) final;
	void reset() final {  }

protected:
    //PATHING
    [[nodiscard]] std::vector<int> calculePath(int, int, const std::vector<MapNode>&) const;
    [[nodiscard]] int nearestNode(const vec3&, const std::vector<MapNode>&) const;

private:

    void patrolBehaviour(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&) const;
    void pursueBehaviour(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&) const;
    void attackBehaviour(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>&) const;

    void basicBehaviour(AI& ai, Physics& phy, Velocity& vel, const vec3& target, float deltaTime, bool align) const;

    void targetBehaviour(AI& ai, const vec3& target) const;
    void seekBehaviour(Physics& phy, Velocity& vel, const vec3& target, float deltaTime) const;
    void alignBehaviour(Physics& phy, const vec3& target) const;

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

