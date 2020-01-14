#include <cmp/BulletData.hpp>

std::ostream& operator<<(std::ostream& os, const BulletData& data) {
	data.print(os, "BulletData")
	<< "\n\tSpeed: " 							<< data.speed
	<< "\n\tDamageType: " 	<< std::boolalpha	<< data.damageType
	<< "\n\tDamage: "							<< data.damage
	<< "\n\tDistance left: " 					<< data.distance_left;

	return os;
}
