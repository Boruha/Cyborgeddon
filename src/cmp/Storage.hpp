#pragma once

#include <cmp/Components.hpp>

#include <SunlightEngine/Device.hpp>
#include <util/Alias.hpp>

#include <unordered_map>
#include <util/ComponentType.hpp>

using Sun::Device;

struct Storage {
	explicit Storage(std::size_t);
	~Storage();

	Storage(			   ) = delete;
	Storage(const Storage& ) = delete;
	Storage(	  Storage&&) = delete;

	Storage& operator=(const Storage& ) = delete;
	Storage& operator=(		 Storage&&) = delete;

	Node_ptr& createSceneNode (const Device& device, const Vector3f& position, const Vector3f& rotation, const Vector3f& dim, const char* mesh = nullptr, const char* texture = nullptr);
	Node_ptr& createCameraNode(const Device& device, const Vector3f& position, const Vector3f& target);

	[[nodiscard]] const variantComponentVectorTypes& getComponents(ComponentType type) const;
	[[nodiscard]] 		variantComponentVectorTypes& getComponents(ComponentType type);

	template<typename T>
	const T& createComponent(const ComponentType type, const T& cmp) const {
		std::cout 	<< cmp 			<< std::endl;
		std::cout 	<< "Capacity: " << std::get<vector<T>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type]).capacity() 	<< std::endl
					<< "Size: " 	<< std::get<vector<T>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type]).size() 		<< std::endl;

		for (auto& item : std::get<vector<T>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type]))
			if (!item)
				return *(new (&item) T(std::move(cmp)));

		return std::get<vector<T>>(const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type]).emplace_back(std::move(cmp));
	}

	template<typename T>
	T& createComponent(const ComponentType type, const T& cmp) {
		return const_cast<T&>(std::as_const(*this).createComponent(type, cmp));
	}

private:

	std::unordered_map <ComponentType, variantComponentVectorTypes> map;
};