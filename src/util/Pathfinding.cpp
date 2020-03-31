#include <util/Pathfinding.hpp>
#include <Engine/util/Math.hpp>

std::vector<int> calculePath(const int start, const int end, const std::vector<MapNode>& graph) 
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

int nearestNode(const vec3& point, const std::vector<MapNode>& graph) 
{
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