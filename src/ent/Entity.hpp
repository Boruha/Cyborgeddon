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

	explicit operator bool() const { return type != UNDEFINED; }

	inline static void resetIDManagementValue() { nextID = 0; }

	template <typename T>
	void addComponent(T& cmp) {
		components.emplace(Component::getCmpTypeID<T>(), &cmp);
	}

	template <typename T>
	void removeComponent() {
		const auto cmpType = Component::getCmpTypeID<T>();

		auto it = components.find(cmpType);

		if (it != components.end())
			it->second->destroy();

		components.erase(cmpType);
	}

	template <typename T>
	T* getComponent() {
		const auto cmpType = Component::getCmpTypeID<T>();

		auto it = components.find(cmpType);

		if (it != components.end())
			return static_cast<T*>(it->second);

		return nullptr;
	}

	template <typename T>
	const T* getComponent() const {
		return const_cast<const T*>(std::as_const(*this).getComponent<T>());
	}

	static EntityID getNextID() {
		return nextID;
	}

	void destroy();

	[[nodiscard]] const EntityType& getType() 	const { return  type; }
	[[nodiscard]] const EntityID&  	getID() 	const { return 	  ID; }

	std::unordered_map<std::size_t, Component*> components;

	INode				* inode 			{ nullptr };

	private:

		inline static EntityID nextID { 0 };

		EntityType type { UNDEFINED };
		EntityID 	ID  {     0	  	};
};

// TODO: generalizar entidades

