#include <cmp/Storage.hpp>

void Storage::initData(const unsigned maxComponents) {
	pool.initData(maxComponents);

}

void Storage::cleanData() {
	for (const auto & node : nodes)
		if (node && *node)
			node->remove();

	pool.cleanData();
}
