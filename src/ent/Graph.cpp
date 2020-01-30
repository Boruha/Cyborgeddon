#include <ent/Graph.hpp>

//MAPNODE FUNCTIONS
MapNode::MapNode(const float x, const float y) : coord(x,y) { connections.reserve(SIZE_CONNECTIONS); }

//NODE RECORD FUNCTIONS
bool NodeRecord::operator< (const NodeRecord& c) const { return this->const_so_far < c.const_so_far; }

void NodeRecord::sortNodeRecord(std::vector<NodeRecord>& records) { std::sort(records.begin(), records.end()); }

bool NodeRecord::contains(std::vector<NodeRecord>& records, MapNode* node)
{
    bool result = false;

    for(NodeRecord& rec : records)
    {
        if(rec.node == node){
            result = true;
            break;
        }
    }
    return result;
} 

NodeRecord& NodeRecord::find(std::vector<NodeRecord>& records, MapNode* node)
{
    NodeRecord& result;

    for(NodeRecord& rec : records)
    {
        if(rec.node == node){
            result = rec;
            break;
        }
    }
    return result;
}