#include <cmp/Component.hpp>

std::ostream& Component::print(std::ostream& os, const std::string& name) const {
	return 	os	<< "\n\n"		<< name
				<< " ( e_ID = " << entityID
				<< " c_ID = " 	<< ID
				<< " e_Type = " << entityType
				<< " )";
}