#include <sys/Path_System.hpp>
#include <array>

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

}


void Path_System::update(const std::unique_ptr<GameContext> &context, float deltaTime) const {}


std::vector<int> Path_System::calculePath(int start, int end)
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

        for(auto& conn : currentConn)
        {
            nextNode = conn.nodeTo;
            nextCost = conn.cost + currentRecord->const_so_far;

            //pasamos a la siguiente conn si esta el nodo en 'close'
            if(currentRecord->contains(close, nextNode)) { continue; }

            //comprobamos si esta en la 'open'
            if(currentRecord->contains(open, nextNode))
            {
                nextRecPtr = currentRecord->find(open, nextNode);

                //si el coste existente es menor al actual, pasamos
                if(nextRecPtr->const_so_far <= nextCost) { continue; }
            }
            else
            {
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


void Path_System::nearestNode(Vector3f& point)
{
    Vector3f nearest   = Vector3f(graph.front().coord.x - point.x, 0, graph.front().coord.y - point.z);
    Vector3f auxVector = Vector3f(0);
    float small_dist   = nearest.length();

    for(auto& node : graph)
    {
        auxVector.x = node.coord.x - point.x;
        auxVector.z = node.coord.y - point.z;
        if(auxVector.length() < small_dist)
        {
            small_dist = auxVector.length();
            nearest    = auxVector; 
        }

    }
}