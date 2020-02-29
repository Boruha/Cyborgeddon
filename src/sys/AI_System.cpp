#include <sys/AI_System.hpp>

#include <Engine/util/Math.hpp>

#include <util/SystemConstants.hpp>
#include <util/SoundPaths.hpp>

void AI_System::init() {
    stateFunctions[PATROL_STATE].p_func = &AI_System::patrolBehaviour;
    stateFunctions[PURSUE_STATE].p_func = &AI_System::pursueBehaviour;
    stateFunctions[ATTACK_STATE].p_func = &AI_System::attackBehaviour;
}

void AI_System::update(const Context &context, const float deltaTime) {
	const vec3& player_pos = context->getPlayer().getComponent<Physics>()->position;

	for (auto & ai : context->getComponents().get<AI>()) {
		if (ai) {
			auto & enemy = context->getEntityByID(ai.getEntityID());

			// si tiene ai, tambien tiene physics
			auto & physics = *enemy.getComponent<Physics>();

			const float distance2 = length2 ( { physics.position.x - player_pos.x, physics.position.z - player_pos.z } );

			if (greater_e(distance2, PATROL_MIN_DISTANCE2))
				ai.state = PATROL_STATE;
			else if (greater_e(distance2, PURSUE_MIN_DISTANCE2))
				ai.state = PURSUE_STATE;
			else if (greater_e(distance2, ATTACK_MIN_DISTANCE2))
				ai.state = ATTACK_STATE;

			// si tiene ai, tambien tiene data y velocity
			auto & data     = *enemy.getComponent<CharacterData>();
			auto & velocity = *enemy.getComponent<Velocity>();

			(this->*(stateFunctions[ai.state].p_func))(ai, physics, data, velocity, player_pos, deltaTime, context);
		}
	}
}

void AI_System::patrolBehaviour(AI& ai, Physics& physics, CharacterData& data, Velocity& velocity, const vec3& player_pos, float deltaTime, const Context& context) const {

	const float distance2 ( length2 ( { physics.position.x - ai.target_position.x, physics.position.z - ai.target_position.z } ) );

	if (greater_e(distance2, ARRIVED_MIN_DISTANCE2)) {
		basicBehaviour(ai, physics, velocity, ai.patrol_position[ai.patrol_index], deltaTime, true);
	} else {
		ai.patrol_index = (ai.patrol_index + 1) % ai.max_index;
		// sumo uno a patrol_index y evito que se pase del size del array de patrol_position (max_index)

		basicBehaviour(ai, physics, velocity, ai.patrol_position[ai.patrol_index], 0, false);
	}
}

void AI_System::pursueBehaviour(AI& ai, Physics& physics, CharacterData& data, Velocity& vel, const vec3& player_pos, const float deltaTime, const Context& context) const {

    const std::vector<MapNode>& ref_graph = context->getGraph();

    if(ai.path_index < 0)
    {
        int final_path       = nearestNode(player_pos, ref_graph); //index -> mapnode + cercano a player
        int ini_path         = nearestNode(physics.position, ref_graph); //index -> mapnode + cercano a player
        ai.path_node  = ini_path;
        ai.path_index = 0;
        //guardamos el path generado, usamos el ID para identificarlo despues.
        context->setPath(ai.getEntityID(), calculePath(ini_path, final_path, ref_graph));
    }
    else
    {
        std::vector<int>& ref_path = context->getPath(ai.getEntityID());

	    const float distance2 ( length2 ( { physics.position.x - ai.target_position.x, physics.position.z - ai.target_position.z } ) );
        
        if (!greater_e(distance2, ARRIVED_MIN_DISTANCE2))
        {
            if(unsigned(++ai.path_index) < ref_path.size())
                ai.path_node  = ref_path[ai.path_index];
            else
                ai.path_index = -1;
        }
    }

	const float distance2 ( length2 ( { physics.position.x - player_pos.x, physics.position.z - player_pos.z } ) );

    if (greater_e(distance2, CHASE_MIN_DISTANCE2))
        basicBehaviour(ai, physics, vel, ref_graph[ai.path_node].coord, deltaTime, true);
    else
        basicBehaviour(ai, physics, vel, player_pos, deltaTime, true);
}

void AI_System::attackBehaviour(AI& ai, Physics& phy, CharacterData& data, Velocity& vel, const vec3& player_pos, const float deltaTime, const Context& context) const {
	
    //if we find enemy before end the pathing.
    if(ai.path_index > -1)
    {
        ai.path_index = -1;
        context->deletePath(ai.getEntityID());
    }

    basicBehaviour(ai, phy, vel, player_pos, 0, true);
    
    if(!greater_e(data.currentAttackingCooldown, 0.f)) {
        data.attacking = true;
        data.currentAttackingCooldown = data.attackingCooldown;

        soundMessages.emplace_back(ASSEMBLED_ATTACK_EVENT);
    }
}

void AI_System::basicBehaviour(AI& ai, Physics& phy, Velocity& vel, const vec3& target, const float deltaTime, const bool align) const {
	targetBehaviour(ai, target);

	seekBehaviour(phy, vel, target, deltaTime);

	if (align)
		alignBehaviour(phy, target);
}

void AI_System::targetBehaviour(AI& ai, const vec3& target) const {
	ai.target_position = target;
}

void AI_System::seekBehaviour(Physics& physics, Velocity& velocity, const vec3& target, const float deltaTime) const {
	velocity.direction = vec3(target.x - physics.position.x, 0, target.z - physics.position.z);

	physics.velocity = normalize(velocity.direction) * velocity.currentSpeed * deltaTime;
}

void AI_System::alignBehaviour(Physics& physics, const vec3& target) const {
	physics.rotation.y = nearestAngle(physics.rotation.y, getRotationYfromXZ(physics.position - target));
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