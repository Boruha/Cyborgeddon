#include <cmp/RigidMovSphere.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& os, const RigidMovSphere& cmp) {
    cmp.print(os, "RigidMoveSphere")
    << "\n\tPosition: " << glm::to_string(cmp.position)
    << "\n\tRadius: " << cmp.radius
    << "\n\tVelocity: " << glm::to_string(*cmp.velocity);

    return os;
}