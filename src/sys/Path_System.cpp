#include <sys/Path_System.hpp>
#include <iostream>

void Path_System::init()
{ 
    graph.reserve(5);

    auto& node_0 = graph.emplace_back(MapNode(0, 0));
	auto& node_1 = graph.emplace_back(MapNode(0, 50));
	auto& node_2 = graph.emplace_back(MapNode(0, 100));

	node_0.connections.emplace_back(0, 1, 5);
	node_0.connections.emplace_back(0, 2, 16);

	node_1.connections.emplace_back(1, 0, 5);
	node_1.connections.emplace_back(1, 2, 10);
	
	node_2.connections.emplace_back(2, 0, 16);

//TEST PATH

    std::vector<int> path = calculePath(2, 1);

    if(path.empty())
        std::cout << " 0 SIZE\n\n";
    else
    {
        for(int& node : path)
        {
            std::cout << "NODO: (" <<graph.at(node).coord.x << ", " << graph.at(node).coord.y << ")\n";
        }
    }


//TEST NEAREST

    std::cout << "\nNEAREST NODE\n";
    vec3 ye = vec3(0,0,76);
    auto index = nearestNode(ye);

    std::cout << "NODO: (" <<graph.at(index).coord.x << ", " << graph.at(index).coord.y << ")\n";

}


void Path_System::update(const std::unique_ptr<GameContext> &context, float deltaTime) const {

}

std::vector<int> Path_System::calculePath(const int start, const int end)
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


int Path_System::nearestNode(vec3& point)
{
    //vector diff the player position and each node in the graph
    vec2 nearest     = vec2(graph.front().coord.x - point.x, graph.front().coord.y - point.z);
    //decision value
    float    small_dist  = nearest.length();
    //index of the nearest node
    int      small_index = 0;
    //counter
    int      i           = -1;

    for(auto& node : graph)
    {
        ++i;
        nearest.x = node.coord.x - point.x;
        nearest.y = node.coord.y - point.z;
        if(nearest.length() < small_dist)
        {
            small_dist  = nearest.length();
            small_index = i;
        }

    }
    return small_index;
}