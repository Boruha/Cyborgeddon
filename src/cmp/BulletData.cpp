#include <cmp/BulletData.hpp>

std::ostream& operator<<(std::ostream& os, const BulletData& data) {
	data.print(os, "BulletData")
	<< "\n\tSpeed: " 						<< data.speed
	<< "\n\tDamageType: ";

	switch (data.damageType) {
		case DEMON: 	os << "DEMON";
			break;
		case ANGEL: 	os << "ANGEL";
			break;
		case NEUTRAL: 	os << "NEUTRAL";
			break;
	}

	os	<< "\n\tDamage: "					<< data.damage
		<< "\n\tDistance left: " 			<< data.distance_left;

	return os;
}
