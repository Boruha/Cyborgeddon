#pragma once

#include <vector>
#include <SunlightEngine/Vector2.hpp>

using Sun::Vector2f;
#define SIZE_CONNECTIONS 5

struct MapNode
{
    explicit MapNode(const float x, const float y);
    ~MapNode() = default;

    void addConnection(Connection);
    const std::vector<Connection>& getConnections() const { return connections; }
    Vector2f& getCoord();

    const Vector2f coord {0,0};
    std::vector<Connection> connections;

};

struct Connection
{
    explicit Connection( const MapNode* to, const short weight) : nodeTo(to), cost(weight) {}
    ~Connection() = default;
    friend bool operator< (Connection& c1, Connection& c2);

    //possibly unnecessary
    //const MapNode* nodeFrom;
    const MapNode* nodeTo;
    const short   cost;
    //available
    // 2 options:
    //     1.- check if the connection is available (doors locked, etc..).
    //     2.- if a enemy is going to crash into a wall then continue the patrol.          
};


//TODO IN FUTURE:
//  1.- CLEAN EVERYTHING
