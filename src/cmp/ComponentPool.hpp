#pragma once

#include <cmp/ComponentVariant.hpp>
#include <unordered_map>
#include <iostream>

using ComponentMap = std::unordered_map<std::size_t, variantComponentVectorTypes>;

struct ComponentPool {

	void initData(unsigned size);

	template <typename T>
	void initVector(const unsigned size) {
		std::get<vector<T>>(map[Component::getCmpTypeID<T>()] = std::move(vector<T>())).reserve(size);
	}

	void cleanData();

	template <typename T>
	const vector<T>& get() const {
		return std::get<vector<T>>(const_cast<ComponentMap &>(map)[Component::getCmpTypeID<T>()]);
	}

	template <typename T>
  	vector<T>& get() {
		return const_cast<vector<T> &>(std::as_const(*this).get<T>());
  	}

	template <typename T>
	const T& createComponent(T&& cmp) const {
			std::cout << "\n\n" << cmp.getName() << "\n";
			printVecInfo(std::get<vector<T>>(const_cast<ComponentMap &>(map)[Component::getCmpTypeID<T>()]));

		for (auto& item : std::get<vector<T>>(const_cast<ComponentMap &>(map)[Component::getCmpTypeID<T>()]))
			if (!item)
				return item = cmp;

		return std::get<vector<T>>(const_cast<ComponentMap &>(map)[Component::getCmpTypeID<T>()]).emplace_back(cmp);
	}

	template <typename T>
	T& createComponent(T&& cmp) {
		return const_cast<T&>(std::as_const(*this).createComponent(std::forward<T>(cmp)));
	}

private:

	template <typename T>
	void printVecInfo(const vector<T>& vec) const {
		std::cout 	<< "Capacity: " << vec.capacity() 	<< "\n"
					 << "Size: " 	<< vec.size() 		<< "\n";
	}

	ComponentMap map;
};