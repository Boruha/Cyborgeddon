#include <cmp/BulletData.hpp>
#include <ostream>

std::ostream& operator<<(std::ostream& os, const BulletData& data) {
	data.print(os, data.getName())
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
