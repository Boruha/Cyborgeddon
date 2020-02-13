#pragma once

#include <vector>

#include <glm/glm.hpp>

using glm::vec2; 

struct Connection;

constexpr unsigned SIZE_CONNECTIONS = 5;

struct MapNode
{
    explicit    MapNode(float x, float y);
                ~MapNode() = default;

    bool                      operator!= (const MapNode&) const;
    bool                      operator== (const MapNode&) const;
    std::vector<Connection>&  getConns();

    vec2                coord { 0,0 };
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
     NodeRecord() = default;;
	~NodeRecord() = default;

    bool operator<(const NodeRecord&) const;

    void sortNodeRecord 	(std::vector<NodeRecord>&) 	  	const;
    bool contains       	(const std::vector<NodeRecord>&, int) const;

    const NodeRecord * find (const std::vector<NodeRecord>&, int) const;
		  NodeRecord * find (const std::vector<NodeRecord>&, int);

	int         node            { -1 };  //path node.
    int         fromNode        { -1 };  //bond to the node we came from.
    int         toNode          { -1 };  //bond to the node we came from.
    float       const_so_far    { 0 };   //Cost from this node to the start of the path.
};


//TODO IN FUTURE:
//  1.- CLEAN EVERYTHING
//  2.- Enhance structure
