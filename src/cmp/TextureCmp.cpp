#include <cmp/TextureCmp.hpp>

#include <ostream>

std::ostream & operator<<(std::ostream &os, const TextureCmp & cmp) {
	cmp.print(os, TextureCmp::getName())
	<< "\n\tITexture: " << cmp.texture;

	return os;
}
