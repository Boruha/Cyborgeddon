#pragma once

#include <cmp/ComponentVariant.hpp>

#include <unordered_map>
#include <util/ComponentType.hpp>

struct Storage {
	explicit Storage() = default;
	~Storage();

	Storage(const Storage& ) = delete;
	Storage(	  Storage&&) = delete;

	Storage& operator=(const Storage& ) = delete;
	Storage& operator=(		 Storage&&) = delete;

	[[nodiscard]] const variantComponentVectorTypes& getComponents(ComponentType type) const;
	[[nodiscard]] 		variantComponentVectorTypes& getComponents(ComponentType type);

	template<typename T>
	const T& createComponent(const ComponentType type, const T& cmp) const {
		std::cout 	<< cmp << "\n";
		printVecInfo(std::get<vector<T>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type]));

		for (auto& item : std::get<vector<T>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type]))
			if (!item)
				return item = T(std::move(cmp));

		return std::get<vector<T>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type]).emplace_back(std::move(cmp));
	}

	template<typename T>
	T& createComponent(const ComponentType type, const T& cmp) {
		return const_cast<T&>(std::as_const(*this).createComponent(type, cmp));
	}

	template <typename T>
	const Node_ptr& createNode(const T& node) const {
		std::cout << "\n\nNode_ptr\n";
		printVecInfo(std::get<vector<Node_ptr>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[NODE_TYPE]));

		for (auto& item : std::get<vector<Node_ptr>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[NODE_TYPE]))
			if (!(*item))
				return item = std::make_unique<T>(std::move(node));

		return std::get<vector<Node_ptr>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[NODE_TYPE]).emplace_back(std::make_unique<T>(std::move(node)));
	}

	template <typename T>
	Node_ptr& createNode(const T& node) {
		return const_cast<Node_ptr&>(std::as_const(*this).createNode(node));
	}

	void initData(int maxComponents);
	void cleanData();

private:

	template <typename T>
	void printVecInfo(const vector<T>& vec) const {
		std::cout 	<< "Capacity: " << vec.capacity() 	<< "\n"
					<< "Size: " 	<< vec.size() 		<< "\n";
	}

	std::unordered_map <ComponentType, variantComponentVectorTypes> map;
};