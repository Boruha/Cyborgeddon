#pragma once

#include <vector>
#include <algorithm>
#include <SunlightEngine/Vector2.hpp>

using Sun::Vector2f;

struct Connection;


#define SIZE_CONNECTIONS 5

struct MapNode
{
    explicit    MapNode(float x, float y);
                ~MapNode() = default;

    bool                      operator!= (const MapNode&) const;
    bool                      operator== (const MapNode&) const;
    std::vector<Connection>&  getConns();

    Vector2f                coord { 0,0 };
    std::vector<Connection> connections;
};

struct Connection
{
    explicit Connection(int from, int to, float weight) : nodeFrom(from), nodeTo(to), cost(weight) {};
    ~Connection() = default;

    int     nodeFrom    { -1 };
    int     nodeTo      { -1 };
    float   cost        { 0 };
};

struct NodeRecord
{
    explicit    NodeRecord() {};
                ~NodeRecord() = default;

    bool        operator<      (const NodeRecord&) const;

    void         sortNodeRecord (std::vector<NodeRecord>&);
    bool         contains       (std::vector<NodeRecord>&, int); 
    NodeRecord*  find           (std::vector<NodeRecord>&, int); 

    int         node            { -1 };  //path node.
    int         fromNode        { -1 };  //bond to the node we came from.
    int         toNode          { -1 };  //bond to the node we came from.
    float       const_so_far    { 0 };   //Cost from this node to the start of the path.
};


//TODO IN FUTURE:
//  1.- CLEAN EVERYTHING
//  2.- Enhance structure
