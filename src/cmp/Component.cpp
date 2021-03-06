#include <cmp/Component.hpp>
#include <ostream>

std::ostream& Component::print(std::ostream& os, const std::string_view name) const {
	return 	os	<< "\n\n"		<< name
				<< " ( e_ID = " << static_cast<uint32_t>(entityID)
				<< " c_ID = " 	<< static_cast<uint32_t>(ID)
				<< " e_Type = " << static_cast<uint32_t>(entityType)
				<< " )";
}