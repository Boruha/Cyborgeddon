#pragma once

#include <vector>
#include <algorithm>
#include <SunlightEngine/Vector2.hpp>

using Sun::Vector2f;

struct Connection;


#define SIZE_CONNECTIONS 5

struct MapNode
{
    explicit MapNode(const float x, const float y);
    ~MapNode() = default;

    const Vector2f coord {0,0};
    std::vector<Connection> connections;
};

struct Connection
{
    Connection(MapNode* from, MapNode* to, short weight) : nodeFrom(from), nodeTo(to), cost(weight) {}
    ~Connection() = default;

    MapNode* nodeFrom { nullptr };
    MapNode* nodeTo { nullptr };
    float   cost { 0 };
};

struct NodeRecord
{
    explicit NodeRecord();
    ~NodeRecord() = default;

    bool operator< (const NodeRecord&) const;
    void sortNodeRecord(std::vector<NodeRecord>&);
    bool contains(std::vector<NodeRecord>&, MapNode*); 
    NodeRecord& find(std::vector<NodeRecord>&, MapNode*); 

    MapNode* node;              //path node.
    Connection* fromConn;       //bond to the node we came from.
    float const_so_far { 0 };   //Cost from this node to the start of the path.
};


//TODO IN FUTURE:
//  1.- CLEAN EVERYTHING
//  2.- Enhance structure
