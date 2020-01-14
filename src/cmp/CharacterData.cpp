#include <cmp/CharacterData.hpp>

std::ostream& operator<<(std::ostream& os, const CharacterData& data) {
	data.print(os, "CharacterData")
	<< "\n\tMode: "   		<< std::boolalpha 	<< data.mode
	<< "\n\tHealth: " 							<< data.health
	<< "\n\tAttackDamage: "						<< data.attackDamage
	<< "\n\tAttackingCooldown: "				<< data.attackingCooldown
	<< "\n\tCurrentAttackingCooldown: "			<< data.currentAttackingCooldown
	<< "\n\tAttacking: "	<< std::boolalpha 	<< data.attacking;

	return os;
}