#pragma once

#include <cmp/ComponentVariant.hpp>
#include <unordered_map>
#include <iostream>

using ComponentMap = std::unordered_map<std::size_t, variantComponentVectorTypes>;

struct ComponentPool {

	void initData(unsigned size);

	template <typename T>
	void initVector(const unsigned size) {
		std::get<vector<T>>(map[typeid(T).hash_code()] = std::move(vector<T>())).reserve(size);
	}

	void cleanData();

	template <typename T>
	const vector<T>& get() const {
		return std::get<vector<T>>(const_cast<ComponentMap &>(map)[typeid(T).hash_code()]);
	}

	template <typename T>
  	vector<T>& get() {
		return const_cast<vector<T> &>(std::as_const(*this).get<T>());
  	}

	template <typename T, typename ... Args>
	const T& createComponent(Args&& ... args) const {
	//	std::cout << "\n\nComponent\n";
	//	printVecInfo(std::get<vector<T>>(const_cast<ComponentMap &>(map)[typeid(T).hash_code()]));

		for (auto& item : std::get<vector<T>>(const_cast<ComponentMap &>(map)[typeid(T).hash_code()]))
			if (!item)
				return item = T(args...);

		return std::get<vector<T>>(const_cast<ComponentMap &>(map)[typeid(T).hash_code()]).emplace_back(args...);
	}

	template <typename T, typename ... Args>
	T& createComponent(Args&& ... args) {
		return const_cast<T&>(std::as_const(*this).createComponent(args ...));
	}

private:

	template <typename T>
	void printVecInfo(const vector<T>& vec) const {
		std::cout 	<< "Capacity: " << vec.capacity() 	<< "\n"
					 << "Size: " 	<< vec.size() 		<< "\n";
	}

	ComponentMap map;
};