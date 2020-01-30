#include <cmp/CharacterData.hpp>

std::ostream& operator<<(std::ostream& os, const CharacterData& data) {
	data.print(os, "CharacterData")
	<< "\n\tHealth: " 									<< data.health
	<< "\n\tMode: ";

	switch (data.mode) {
		case DEMON: 								os 	<< "DEMON";
			break;
		case ANGEL: 								os 	<< "ANGEL";
			break;
		case NEUTRAL: 								os 	<< "NEUTRAL";
			break;
	}

	os	<< "\n\tAttackDamage: "							<< data.attackDamage
		<< "\n\tDashSpeed: "							<< data.dashSpeed

		<< "\n\tSwitchingMode: " 	<< std::boolalpha 	<< data.switchingMode
		<< "\n\tAttacking: "		<< std::boolalpha 	<< data.attacking
		<< "\n\tDashing: "			<< std::boolalpha	<< data.dashing

		<< "\n\tSwitchingCooldown: "					<< data.switchingCooldown
		<< "\n\tCurrentSwitchingCooldown: "				<< data.currentSwitchingCooldown

		<< "\n\tAttackingCooldown: "					<< data.attackingCooldown
		<< "\n\tCurrentAttackingCooldown: "				<< data.currentAttackingCooldown

		<< "\n\tDashingCooldown: "						<< data.dashingCooldown
		<< "\n\tCurrentDashingCooldown: "				<< data.currentDashingCooldown;

	return os;
}