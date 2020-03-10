#include <sys/AI_System.hpp>

#include <Engine/util/Math.hpp>

#include <util/SystemConstants.hpp>
#include <util/SoundPaths.hpp>

/* GENERAL BEHAVIOURS DEF */
struct SeekBehaviour : BehaviourNode
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        vel.direction = vec3(ai.target_position.x - phy.position.x, 0, ai.target_position.z - phy.position.z);
        phy.velocity  = normalize(vel.direction) * vel.currentSpeed * deltaTime;
        return true;
    }
};

struct AlignBehaviour : BehaviourNode
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
	    phy.rotation.y = nearestAngle(phy.rotation.y, getRotationYfromXZ(phy.position - ai.target_position));
        return true;
    }
};

struct ArriveBehaviour : BehaviourNode
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
    	const float distance2 = length2 ({ phy.position.x - ai.target_position.x, phy.position.z - ai.target_position.z });
	    if (greater_e(distance2, ARRIVED_MIN_DISTANCE2))
            return true;
        return false;
    }
};

/* PATROL BEHAVIOURS DEF */
struct NextPatrolBehaviour : BehaviourNode
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
    	ai.patrol_index = (ai.patrol_index + 1) % ai.max_index;
		ai.target_position = ai.patrol_position[ai.patrol_index];
        return true;
    }
};

struct PatrolStateBehaviour : BehaviourNode
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        const float distance2 = length2({ phy.position.x - player_pos.x, phy.position.z - player_pos.z });
        if (greater_e(distance2, PATROL_MIN_DISTANCE2))
        {
            ai.target_position = ai.patrol_position[ai.patrol_index];
            return true;
        }
        return false;
    }
};

/* PURSE BEHAVIOURS DEF */
struct PursueStateBehaviour : BehaviourNode
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        const float distance2 = length2({ phy.position.x - player_pos.x, phy.position.z - player_pos.z });
        if (greater_e(distance2, PURSUE_MIN_DISTANCE2))
            return true;
        return false;
    }
};

struct HaveRouteBehaviour : BehaviourNode
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        if(ai.path_index > -1)
        {
            const std::vector<MapNode>& ref_graph = context->getGraph();
            ai.target_position = ref_graph[ai.path_node].coord;

            return true;
        }
        return false;
    }
};
//Se deberia comprobar que se genere todo(?)
struct CreateRouteBehaviour : BehaviourNode
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        const std::vector<MapNode>& ref_graph = context->getGraph();

        int final_path = nearestNode(player_pos, ref_graph);       //index -> mapnode + cercano a player
        int ini_path   = nearestNode(phy.position, ref_graph); //index -> mapnode + cercano a enemy
        ai.path_node   = ini_path;
        ai.path_index  = 0;
        //guardamos el path generado, usamos el ID para identificarlo despues.
        context->setPath(ai.getEntityID(), calculePath(ini_path, final_path, ref_graph));
        ai.target_position = ref_graph[ai.path_node].coord;
        
        return true;
    }
};

struct NextPursePointBehaviour : BehaviourNode
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        const std::vector<MapNode>& ref_graph = context->getGraph();
        std::vector<int>& ref_path = context->getPath(ai.getEntityID());

        if(unsigned(++ai.path_index) < ref_path.size())
        {
            ai.path_node  = ref_path[ai.path_index];
            ai.target_position = ref_graph[ai.path_node].coord;
            return true;
        }
        return false;
    }
};

struct DeletePurseBehaviour : BehaviourNode
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        ai.path_index = -1;
        ai.path_node  = -1;
        context->deletePath(ai.getEntityID());
    
        return false;
    }
};

struct ChaseStateBehaviour : BehaviourNode
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        const float distance2 = length2({ phy.position.x - player_pos.x, phy.position.z - player_pos.z });
        if (!greater_e(distance2, CHASE_MIN_DISTANCE2))
        {
            ai.target_position = player_pos;
            return true;
        }
        return false;
    }
};

/* ATTACK BEVAHOIUR DEF */
struct AttackStateBehaviour : BehaviourNode //innecesario? ad future puede tener un parametro no distancia. ai.state cambiarlo a una condition? 
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        //const float distance2 = length2({ phy.position.x - player_pos.x, phy.position.z - player_pos.z });
        //if (greater_e(distance2, ATTACK_MIN_DISTANCE2))
        //{
            ai.target_position = player_pos;
            phy.velocity = glm::vec3(0);
            return true;
        //}
        //return false;
    }
};

struct BasicAttackBehaviour : BehaviourNode 
{
    bool run(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) override 
    {
        if(!greater_e(data.currentAttackingCooldown, 0.f)) 
        {
            data.attacking = true;
            data.currentAttackingCooldown = data.attackingCooldown;
            damageMessages.emplace_back(data.attackDamage);
            soundMessages.emplace_back(ATTACK_ENEMY_ASSEMBLY);
            return true;
        }
        return false;
    }
};

/* FUNCTIONS */
void AI_System::init() {

/*-- PATROL  --*/     
    /* PATROL UPDATE */
    std::unique_ptr<Selector> patrolPoint = std::make_unique<Selector>();
    patrolPoint->childs.emplace_back(std::make_unique<ArriveBehaviour>());
    patrolPoint->childs.emplace_back(std::make_unique<NextPatrolBehaviour>());

    /* PHY UPDATE */
    std::unique_ptr<Sequence> phyUpdate = std::make_unique<Sequence>();
    phyUpdate->childs.emplace_back(std::make_unique<AlignBehaviour>());
    phyUpdate->childs.emplace_back(std::make_unique<SeekBehaviour>());
    
    /* PATROL STATE */
    std::unique_ptr<Sequence> patrolState = std::make_unique<Sequence>();
    patrolState->childs.emplace_back(std::make_unique<PatrolStateBehaviour>());
    patrolState->childs.push_back(std::move(patrolPoint));
    patrolState->childs.push_back(std::move(phyUpdate));
/*-- PATROL  --*/     

/*-- PURSUE  --*/     
    /* UPDATE PURSUE */
    std::unique_ptr<Selector> pursuePoint = std::make_unique<Selector>();
    pursuePoint->childs.emplace_back(std::make_unique<ArriveBehaviour>());
    pursuePoint->childs.emplace_back(std::make_unique<NextPursePointBehaviour>());
    pursuePoint->childs.emplace_back(std::make_unique<DeletePurseBehaviour>());

    /* GET PURSUE */
    std::unique_ptr<Sequence> getPursue = std::make_unique<Sequence>();
    getPursue->childs.emplace_back(std::make_unique<HaveRouteBehaviour>());
    getPursue->childs.push_back(std::move(pursuePoint));

    /* SET/GET PURSE */
    std::unique_ptr<Selector> setGetPursue  = std::make_unique<Selector>();    
    setGetPursue->childs.emplace_back(std::make_unique<ChaseStateBehaviour>());
    setGetPursue->childs.push_back(std::move(getPursue));
    setGetPursue->childs.emplace_back(std::make_unique<CreateRouteBehaviour>());
    
    /* PHY UPDATE */
    phyUpdate = std::make_unique<Sequence>();
    phyUpdate->childs.emplace_back(std::make_unique<AlignBehaviour>());
    phyUpdate->childs.emplace_back(std::make_unique<SeekBehaviour>());

    /* PURSUE STATE */
    std::unique_ptr<Sequence> pursueState = std::make_unique<Sequence>();
    pursueState->childs.emplace_back(std::make_unique<PursueStateBehaviour>());
    pursueState->childs.push_back(std::move(setGetPursue));
    pursueState->childs.push_back(std::move(phyUpdate));
/*-- PURSUE  --*/     

/*-- ATTACK  --*/ 
    /* PHY UPDATE */
    phyUpdate = std::make_unique<Sequence>();
    phyUpdate->childs.emplace_back(std::make_unique<AlignBehaviour>());
    phyUpdate->childs.emplace_back(std::make_unique<BasicAttackBehaviour>());

    /* ATTACK STATE */
    std::unique_ptr<Sequence> attackState = std::make_unique<Sequence>();
    attackState->childs.emplace_back(std::make_unique<AttackStateBehaviour>());
    attackState->childs.push_back(std::move(phyUpdate));
    attackState->childs.emplace_back(std::make_unique<DeletePurseBehaviour>());
/*-- ATTACK  --*/     

    root = std::make_unique<Selector>();
    root->childs.push_back(std::move(patrolState));
    root->childs.push_back(std::move(pursueState));
    root->childs.push_back(std::move(attackState));
}

void AI_System::update(const Context &context, const float deltaTime) {
	const vec3& player_pos = context->getPlayer().getComponent<Physics>()->position;

	for (auto & ai : context->getComponents().getComponents<AI>()) 
    {
		if (ai) 
        {
			auto & enemy    = context->getEntityByID(ai.getEntityID());
			auto & physics  = *enemy.getComponent<Physics>();
			auto & data     = *enemy.getComponent<CharacterData>();
			auto & velocity = *enemy.getComponent<Velocity>();

            root->run(ai, physics, data, velocity, player_pos,
                      deltaTime, context);
		}
	}
}

//PATHING
std::vector<int> AI_System::calculePath(const int start, const int end, const std::vector<MapNode>& graph) const {
    //index of nodes in graph
    NodeRecord startRecord = NodeRecord();
    NodeRecord* currentRecord = nullptr;
    NodeRecord* nextRecPtr = nullptr;

    //vectors of records
    std::vector<NodeRecord> open;
    std::vector<NodeRecord> close;
    std::vector<Connection> currentConn;
    std::vector<int> path;
    open.reserve(5);
    close.reserve(5);

    //set startRecord
    startRecord.node = start;

    //insert start node
    open.push_back(startRecord);

    //var aux
    int nextNode = -1;
    int nextCost = 0;

    currentRecord = &open.front();

    while (!open.empty())
    {
        //cogemos el primer elemento (el de menos coste acumulado) y sus conexiones
        if(currentRecord->node == end) { break; }
        currentConn = graph.at(currentRecord->node).connections;

        for(const auto& conn : currentConn)
        {
            nextNode = conn.nodeTo;
            nextCost = conn.cost + currentRecord->const_so_far;

            //pasamos a la siguiente conn si esta el nodo en 'close'
            if(currentRecord->contains(close, nextNode))
            	continue;

            //comprobamos si esta en la 'open'
            if(currentRecord->contains(open, nextNode))
            {
                nextRecPtr = currentRecord->find(open, nextNode);

                //si el coste existente es menor al actual, pasamos
                if(nextRecPtr->const_so_far <= nextCost)
                	continue;
            } else {
                //no esta en ninguna lista so creamos record
                nextRecPtr = &open.emplace_back();
                nextRecPtr->node = nextNode;
            }
            //insert/update
            nextRecPtr->fromNode = conn.nodeFrom;
            nextRecPtr->toNode = conn.nodeTo;
            nextRecPtr->const_so_far = nextCost;
            nextRecPtr = nullptr;
        }
        //move current to close and erase it from open.
        close.push_back(*currentRecord);
        open.erase(open.begin());
        currentRecord->sortNodeRecord(open);
    }
    //if we found the solution we set a MapNode path
    if(currentRecord->node == end)
    {
        while (currentRecord->node != start)
        {
            path.insert(path.begin(), currentRecord->node);
            currentRecord = currentRecord->find(close, currentRecord->fromNode);
        }

        path.insert(path.begin(), currentRecord->node);
    }
    return path;
}

int AI_System::nearestNode(const vec3& point, const std::vector<MapNode>& graph) const {
    //vector diff the player position and each node in the graph
    vec3 nearest     = vec3(graph.front().coord.x - point.x, 0,graph.front().coord.z - point.z);
    //decision value
    float    small_dist2  = length2(nearest);
    //index of the nearest node
    int      small_index = 0;
    //counter
    int      i           = -1;

    for(const auto& node : graph)
    {
        ++i;
        nearest.x = node.coord.x - point.x;
        nearest.z = node.coord.z - point.z;

        const float len2near = length2(nearest);

        if(len2near < small_dist2)
        {
            small_dist2  = len2near;
            small_index = i;
        }
    }
    return small_index;
}