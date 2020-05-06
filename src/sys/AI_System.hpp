#pragma once

#include <sys/System.hpp>
#include <list>

struct Selector;

struct AI_System : System
{
	~AI_System() override = default;

	void init() final;
    void fixedUpdate(const Context &context, float deltaTime) final;
	void update(const Context &context, float deltaTime) override {};
	void reset() final {  }
    
    unsigned frame { 0 };

protected:
    [[nodiscard]] bool checkObstacles(const vec3&, const vec3&, float, const std::unique_ptr<GameContext>&);
    [[nodiscard]] bool checkFacing(const Physics&, const std::unique_ptr<GameContext>&);
                  void setPhase(const std::unique_ptr<GameContext>&);
                  void setInList(const std::unique_ptr<GameContext>&);

    std::unique_ptr<Selector> root;

    std::list<AI*> schedule;
};

/* BEHAVIOUR STRUCTURE DEF */
struct BehaviourNode : AI_System
{
    virtual bool run(AI&, Physics&, Velocity&, const vec3&, float, const std::unique_ptr<GameContext>&) = 0;
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
    bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        for(auto& ref_child : getChilds())
        {
            if(!ref_child->run(ai, phy, vel, player_pos, deltaTime, context))
                return false;
        }
        return true;
    }
};

struct Selector : CompoundNode
{
    /* FUNCTIONS */
    bool run(AI& ai, Physics& phy, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        for(auto& ref_child : getChilds())
        {
            if(ref_child->run(ai, phy, vel, player_pos, deltaTime, context))
                return true;
        }
        return false;
    }
};