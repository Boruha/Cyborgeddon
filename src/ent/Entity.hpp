#pragma once

#include <util/Alias.hpp>
#include <Engine/EngineInterface/SceneInterface/INode.hpp>
#include <unordered_map>
#include <cmp/Component.hpp>
#include <iostream>

struct INode;

struct Entity
{
	explicit Entity(EntityType type);

	~Entity();

	constexpr explicit operator bool() const { return type != UNDEFINED; }

	inline static void resetIDManagementValue() { nextID = 0; }

	template <typename T>
	constexpr void addComponent(T& cmp) {
		components.emplace(Component::getCmpTypeID<T>(), &cmp);
	}

	template <typename T>
	constexpr void removeComponent() {
		const auto cmpType = Component::getCmpTypeID<T>();

		auto it = components.find(cmpType);

		if (it != components.end())
			it->second->destroy();

		components.erase(cmpType);
	}

	template <typename T>
	constexpr const T* getComponent() const {
		const auto cmpType = Component::getCmpTypeID<T>();

		auto it = components.find(cmpType);

		if (it != components.end())
			return static_cast<T*>(it->second);

		return nullptr;
	}

	template <typename T>
	constexpr T* getComponent() {
		return const_cast<T *>(std::as_const(*this).getComponent<T>());
	}

	static EntityID getNextID() {
		return nextID;
	}

	void destroy();

	[[nodiscard]] const EntityType& getType() 	const { return  type; }
	[[nodiscard]] const EntityID&  	getID() 	const { return 	  ID; }

	std::unordered_map<std::size_t, Component*> components;

	private:

		inline static EntityID nextID { 0 };

		EntityType type { UNDEFINED };
		EntityID 	ID  {     0	  	};
};

// TODO: generalizar entidades

