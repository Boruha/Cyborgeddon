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

    void addConnection(Connection&);
    void sortConnections();
    const std::vector<Connection>& getConnections() const { return connections; }
    
    Vector2f& getCoord();

    const Vector2f coord {0,0};
    std::vector<Connection> connections;
};

struct Connection
{
    Connection(MapNode* from, MapNode* to, short weight) : nodeFrom(from), nodeTo(to), cost(weight) {}
    ~Connection() = default;
    bool operator< (const Connection&) const;
    //friend bool operator< (const Connection& , const Connection&) const;

    //possibly unnecessary
    MapNode* nodeFrom { nullptr };
    MapNode* nodeTo { nullptr };
    short   cost { 0 };

    //available
    // 2 options:
    //     1.- check if the connection is available (doors locked, etc..).
    //     2.- if a enemy is going to crash into a wall then continue the patrol.          
};

//TODO IN FUTURE:
//  1.- CLEAN EVERYTHING
