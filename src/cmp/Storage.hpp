#pragma once

#include <cmp/ComponentVariant.hpp>

#include <unordered_map>
#include <util/ComponentType.hpp>
#include <iostream>
#include <src/Engine/EngineInterface/IEngine.hpp>

struct IEngine;

struct Storage {
	explicit Storage(const IEngine * const engine) : engine(engine) {  }
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
        printVecInfo(std::get<vector<T>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type]));

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
	const std::unique_ptr<INode>& createIObjectNode(Args&& ... args) const {
		std::cout << "\n\nINode\n";
		printVecInfo(std::get<vector<std::unique_ptr<INode>>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[INODE_TYPE]));

		for (auto& item : std::get<vector<std::unique_ptr<INode>>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[INODE_TYPE])) {
			if (!(*item)) {
				return item = engine->scene->addObjectNode(args...);
			}
		}

		return std::get<vector<std::unique_ptr<INode>>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[INODE_TYPE]).emplace_back(engine->scene->addObjectNode(args...));
	}

	template <typename T, typename ... Args>
	std::unique_ptr<INode>& createIObjectNode(Args&& ... args) {
		return const_cast<std::unique_ptr<INode>&>(std::as_const(*this).createIObjectNode<T>(args...));
	}

	template <typename T, typename ... Args>
	const std::unique_ptr<INode>& createICameraNode(Args&& ... args) const {
		std::cout << "\n\nINode\n";
		printVecInfo(std::get<vector<std::unique_ptr<INode>>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[INODE_TYPE]));

		for (auto& item : std::get<vector<std::unique_ptr<INode>>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[INODE_TYPE])) {
			if (!(*item)) {
				return item = engine->scene->addCameraNode(args...);
			}
		}

		return std::get<vector<std::unique_ptr<INode>>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[INODE_TYPE]).emplace_back(engine->scene->addCameraNode(args...));
	}

	template <typename T, typename ... Args>
	std::unique_ptr<INode>& createICameraNode(Args&& ... args) {
		return const_cast<std::unique_ptr<INode>&>(std::as_const(*this).createICameraNode<T>(args...));
	}

	void initData(int maxComponents);
	void cleanData();

private:

	template <typename T>
	void printVecInfo(const vector<T>& vec) const {
		std::cout 	<< "Capacity: " << vec.capacity() 	<< "\n"
					<< "Size: " 	<< vec.size() 		<< "\n";
	}

	const IEngine * const engine { nullptr };

	std::unordered_map <ComponentType, variantComponentVectorTypes> map;
};