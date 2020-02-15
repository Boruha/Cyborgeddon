#include <sys/AI_System.hpp>
#include <Engine/util/Math.hpp>
#include <util/SystemConstants.hpp>
#include <iostream>

// TODO: considerar los estados de la IA como punteros a funcion
void AI_System::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	const vec3 player_pos = context->getPlayer().physics->position;

	for (const auto& enemy : context->getEntities()) {
		if (enemy && enemy.ai) {
			const vec3 v_distance(enemy.physics->position.x - player_pos.x, 0, enemy.physics->position.z - player_pos.z);
			const float distance = length(v_distance);

			if (greater_e(distance, PATROL_MIN_DISTANCE))
				stateFunctions[enemy.ai->state = PATROL_STATE].p_func(enemy, player_pos, deltaTime, context);
			else if (greater_e(distance, PURSUE_MIN_DISTANCE))
				stateFunctions[enemy.ai->state = PURSUE_STATE].p_func(enemy, player_pos, deltaTime, context);
			else if (greater_e(distance, ATTACK_MIN_DISTANCE))
				stateFunctions[enemy.ai->state = ATTACK_STATE].p_func(enemy, player_pos, deltaTime, context);
		}
	}
}

void AI_System::patrolBehaviour(const Entity& enemy, const vec3& player_pos, float deltaTime, const std::unique_ptr<GameContext>& context) {
	const vec3 distance(enemy.physics->position.x - enemy.ai->target_position.x, 0, enemy.physics->position.z - enemy.ai->target_position.z);

	if (greater_e(length(distance), ARRIVED_MIN_DISTANCE)) {
		basicBehaviour(enemy, enemy.ai->patrol_position[enemy.ai->patrol_index], deltaTime, true);
	} else {
		enemy.ai->patrol_index = (enemy.ai->patrol_index + 1) % enemy.ai->max_index;
		// sumo uno a patrol_index y evito que se pase del size del array de patrol_position (max_index)

		basicBehaviour(enemy, enemy.ai->patrol_position[enemy.ai->patrol_index], 0, false);
	}
}

void AI_System::pursueBehaviour(const Entity& enemy, const vec3& player_pos, const float deltaTime, const std::unique_ptr<GameContext>& context) {
    
    const vec3 distance(enemy.physics->position.x - player_pos.x, 0, enemy.physics->position.z - player_pos.z);
    const std::vector<MapNode>& ref_graph = context->getGraph();

    if(enemy.ai->path_index < 0)
    {
        int final_path       = nearestNode(player_pos, ref_graph); //index -> mapnode + cercano a player
        int ini_path         = nearestNode(enemy.physics->position, ref_graph); //index -> mapnode + cercano a player
        enemy.ai->path_node  = ini_path;
        enemy.ai->path_index = 0;
        //guardamos el path generado, usamos el ID para identificarlo despues.
        context->setPath(enemy.getID(), calculePath(ini_path, final_path, ref_graph));
    }
    else
    {
        std::vector<int>& ref_path = context->getPath(enemy.getID());
	    const vec3 distance_path(enemy.physics->position.x - enemy.ai->target_position.x, 0, enemy.physics->position.z - enemy.ai->target_position.z);
        
        if (!greater_e(length(distance_path), ARRIVED_MIN_DISTANCE))
        {
            if(++enemy.ai->path_index < ref_path.size())
                enemy.ai->path_node  = ref_path[enemy.ai->path_index];
            else
                enemy.ai->path_index = -1;
        }
    }

    if (greater_e(length(distance), CHASE_MIN_DISTANCE))
    {
        basicBehaviour(enemy, ref_graph[enemy.ai->path_node].coord, deltaTime, true);
    }
    else
        basicBehaviour(enemy, player_pos, deltaTime, true);
}

void AI_System::attackBehaviour(const Entity& enemy, const vec3& player_pos, const float deltaTime, const std::unique_ptr<GameContext>& context) {
	
    //if we find enemy before end the pathing.
    if(enemy.ai->path_index > -1)
    {
        enemy.ai->path_index = -1;
        context->deletePath(enemy.getID());
    }

    basicBehaviour(enemy, player_pos, 0, true);
    
    if(!greater_e(enemy.characterData->currentAttackingCooldown, 0.f)) {
        enemy.characterData->attacking = true;
        enemy.characterData->currentAttackingCooldown = enemy.characterData->attackingCooldown;
    }
}

void AI_System::basicBehaviour(const Entity& enemy, const vec3& target, const float deltaTime, const bool align) {
	targetBehaviour(*enemy.ai, target);

	seekBehaviour(enemy, target, deltaTime);

	if (align)
		alignBehaviour(enemy, target);
}

void AI_System::targetBehaviour(AI& ai, const vec3& target) {
	ai.target_position = target;
}

void AI_System::seekBehaviour(const Entity& enemy, const vec3& target, const float deltaTime) {
	enemy.velocity->direction = target - enemy.physics->position;
	enemy.velocity->direction.y = 0;

	enemy.physics->velocity = normalize(enemy.velocity->direction) * enemy.velocity->currentSpeed * deltaTime;
}

void AI_System::alignBehaviour(const Entity& enemy, const vec3& target) {
	enemy.physics->rotation.y = nearestAngle(enemy.physics->rotation.y, getRotationYfromXZ(enemy.physics->position - target));
}


//PATHING
std::vector<int> AI_System::calculePath(const int start, const int end, const std::vector<MapNode>& graph)
{
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

int AI_System::nearestNode(const vec3& point, const std::vector<MapNode>& graph)
{
    //vector diff the player position and each node in the graph
    vec3 nearest     = vec3(graph.front().coord.x - point.x, 0,graph.front().coord.z - point.z);
    //decision value
    float    small_dist  = length(nearest);
    //index of the nearest node
    int      small_index = 0;
    //counter
    int      i           = -1;

    for(auto& node : graph)
    {
        ++i;
        nearest.x = node.coord.x - point.x;
        nearest.z = node.coord.z - point.z;
        if(length(nearest) < small_dist)
        {
            small_dist  = length(nearest);
            small_index = i;
        }

    }
    return small_index;
}