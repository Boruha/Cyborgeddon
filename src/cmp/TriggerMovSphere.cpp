#include <cmp/TriggerMovSphere.hpp>
#include <ostream>
#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& os, const TriggerMovSphere& cmp) {
    cmp.print(os, cmp.getName())
    << "\n\tPosition: " << glm::to_string(cmp.position)
    << "\n\tRadius: " << cmp.radius
    << "\n\tVelocity: " << glm::to_string(*cmp.velocity);

    return os;
}