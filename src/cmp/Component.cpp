#include <cmp/Component.hpp>
#include <iostream>

std::ostream& Component::print(std::ostream& os, const std::string& name) const {
	return 	os	<< "\n\n"		<< name
				<< " ( e_ID = " << static_cast<uint32_t>(entityID)
				<< " c_ID = " 	<< static_cast<uint32_t>(ID)
				<< " e_Type = " << static_cast<uint32_t>(entityType)
				<< " )";
}