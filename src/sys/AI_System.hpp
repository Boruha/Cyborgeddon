#pragma once

#include <sys/System.hpp>

struct Selector;

struct AI_System : System
{
	~AI_System() override = default;

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
    ~CompoundNode() override = default;

    /* FUNCTIONS */
    [[nodiscard]] const std::vector<std::unique_ptr<BehaviourNode>>& getChilds() const { return childs; };

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
            if(!ref_child->run(ai, phy, data, vel, player_pos, deltaTime, context))
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
            if(ref_child->run(ai, phy, data, vel, player_pos, deltaTime, context))
                return true;
        }
        return false;
    }
};