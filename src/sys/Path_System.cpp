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

    calculePath(2, 1);

}


void Path_System::update(const std::unique_ptr<GameContext> &context, float deltaTime) const {}


void Path_System::calculePath(int start, int end)
{
    std::cout << "\n---- path ----\n";
    //index of nodes in graph
    NodeRecord startRecord = NodeRecord();
    NodeRecord* currentRecord = nullptr;
    NodeRecord* nextRecPtr = nullptr;

    //vectors of records
    std::vector<NodeRecord> open;
    std::vector<NodeRecord> close;
    std::vector<Connection> currentConn;
    std::vector<MapNode> path;
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
                nextRecPtr = &open.emplace_back();
                nextRecPtr->node = nextNode;
            }

            nextRecPtr->fromNode = conn.nodeFrom;
            nextRecPtr->toNode = conn.nodeTo;
            nextRecPtr->const_so_far = nextCost;
/*
            std::cout << "\n---- CONTENT TO OPEN ----\n";
            std::cout << "NODO: (" << graph.at(nextRecPtr->node).coord.x << ", " << graph.at(nextRecPtr->node).coord.y << ")\n";
            std::cout << "FROM: (" << graph.at(nextRecPtr->fromNode).coord.x << ", " << graph.at(nextRecPtr->fromNode).coord.y << ")\n";
            std::cout << "TO  : (" << graph.at(nextRecPtr->toNode).coord.x << ", " << graph.at(nextRecPtr->toNode).coord.y << ")\n";
            std::cout << "COST: (" << nextRecPtr->const_so_far << ")\n";
            */
            nextRecPtr = nullptr;
        }
        close.push_back(*currentRecord);
        open.erase(open.begin());
        currentRecord->sortNodeRecord(open);
    }
    std::cout << "\n---- CONTENT OPEN ----\n";
    for (auto &rec : open)
    {
        std::cout << "NODO: (" << graph.at(rec.node).coord.x << ", " << graph.at(rec.node).coord.y << ")\n";
        std::cout << "FROM: (" << graph.at(rec.fromNode).coord.x << ", " << graph.at(rec.fromNode).coord.y << ")\n";
        std::cout << "TO  : (" << graph.at(rec.toNode).coord.x << ", " << graph.at(rec.toNode).coord.y << ")\n";
        std::cout << "COST: (" << rec.const_so_far << ")\n";
    }
    std::cout << "\n\n";

    std::cout << "---- CONTENT CLOSED ----\n";
    for (auto &rec : close)
    {
        std::cout << "NODO: (" << graph.at(rec.node).coord.x << ", " << graph.at(rec.node).coord.y << ")\n";
        if(rec.fromNode > -1)
            std::cout << "FROM: (" << graph.at(rec.fromNode).coord.x << ", " << graph.at(rec.fromNode).coord.y << ")\n";
        if(rec.toNode > -1)
            std::cout << "TO  : (" << graph.at(rec.toNode).coord.x << ", " << graph.at(rec.toNode).coord.y << ")\n";
        
        std::cout << "COST: (" << rec.const_so_far << ")\n";
    }
    
    if(currentRecord->node != end);
    else
    {
        while (currentRecord->node != start)
        {
            path.insert(path.begin(), graph.at(currentRecord->node));
            currentRecord = currentRecord->find(close, currentRecord->fromNode);
        }
        path.insert(path.begin(), graph.at(currentRecord->node));

    }
    std::cout <<"\n---- SOLUTION -----\n";
    if(path.empty())
        std::cout << " 0 SIZE\n\n";
    else
    {
        for(MapNode& node : path)
        {
            std::cout << "NODO: (" <<node.coord.x << ", " << node.coord.y << ")\n";
        }
    }

}
