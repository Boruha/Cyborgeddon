#pragma once

#include <cmp/ComponentVariant.hpp>

#include <unordered_map>
#include <util/ComponentType.hpp>
#include <iostream>

struct Storage {
	explicit Storage() = default;
	~Storage();

	Storage(const Storage& ) = delete;
	Storage(	  Storage&&) = delete;

	Storage& operator=(const Storage& ) = delete;
	Storage& operator=(		 Storage&&) = delete;

	[[nodiscard]] const variantComponentVectorTypes& getComponents(ComponentType type) const;
	[[nodiscard]] 		variantComponentVectorTypes& getComponents(ComponentType type);

	template <typename T, typename ... Args>
	const T& createComponent(const ComponentType type, Args&& ... args) const {
        std::cout << "\n\nComponent\n";
        printVecInfo(std::get<vector<Node_ptr>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[NODE_TYPE]));

        for (auto& item : std::get<vector<T>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type])) {
            if (!item) {
                return item = T(args...);
            }
        }

	    return std::get<vector<T>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type]).emplace_back(args...);
	}

    template <typename T, typename ... Args>
    T& createComponent(const ComponentType type, Args&& ... args) {
        return const_cast<T&>(std::as_const(*this).createComponent<T>(type, args...));
    }

	template <typename T, typename ... Args>
	const Node_ptr& createNode(Args&& ... args) const {
		std::cout << "\n\nNode_ptr\n";
		printVecInfo(std::get<vector<Node_ptr>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[NODE_TYPE]));

		for (auto& item : std::get<vector<Node_ptr>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[NODE_TYPE])) {
			if (!(*item)) {
				return item = std::make_unique<T>(args...);
			}
		}

		return std::get<vector<Node_ptr>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[NODE_TYPE]).emplace_back(std::make_unique<T>(args...));
	}

	template <typename T, typename ... Args>
	Node_ptr& createNode(Args&& ... args) {
		return const_cast<Node_ptr&>(std::as_const(*this).createNode<T>(args...));
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