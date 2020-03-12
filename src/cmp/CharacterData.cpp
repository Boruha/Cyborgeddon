#include <cmp/CharacterData.hpp>
#include <ostream>

CharacterData::CharacterData(   const EntityType type,          const EntityID e_ID,	        const ModeType mode,
                                const float health,             const float switchCD,           const float attackDamage,
                                const float attackingCD,        const float attackingRange,
								const float dashSpeed,          const float dashCD
                            )
                            :   Component(type,                 e_ID),                          mode(mode),
                                health(health),                 switchingCooldown(switchCD),    attackDamage(attackDamage),
                                attackingCooldown(attackingCD), attackRange(attackingRange),
								dashSpeed(dashSpeed),           dashingCooldown(dashCD)
                            {

                            }

std::ostream& operator<<(std::ostream& os, const CharacterData& data) {
	data.print(os, data.getName())
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
		<< "\n\tCurrentDashingCooldown: "				<< data.currentDashingCooldown

		<< "\n\tAttack Range: "				            << data.attackRange;

	return os;
}