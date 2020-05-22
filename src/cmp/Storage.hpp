#pragma once

#include <cmp/Components.hpp>
#include <src/Engine/EngineInterface/IEngine.hpp>
#include <unordered_map>
#include <iostream>

using std::vector;

struct IComponentVector {
	virtual ~IComponentVector() = default;
};

template <typename T>
struct ComponentVector : IComponentVector {
	~ComponentVector<T>() override = default;
	vector<T> components;
};

using ComponentMap = std::unordered_map < std::size_t, std::unique_ptr < IComponentVector > >;

struct Storage {
	explicit Storage(const IEngine * const _engine) : engine(*_engine) {  }
	~Storage() = default;

	Storage(const Storage& ) = delete;
	Storage(	  Storage&&) = delete;

	Storage& operator=(const Storage& ) = delete;
	Storage& operator=(		 Storage&&) = delete;

	void initData(unsigned maxComponents);
	void cleanData();

	void printComponentStorage() const;

	template <typename T>
	constexpr void initVector(const unsigned size) {
		auto   u_ptr = std::make_unique<ComponentVector<T>>();
		auto *   ptr = u_ptr.get();

		ptr->components.reserve(size);

		map[Component::getCmpTypeID<T>()] = std::move(u_ptr);
	}

	template <typename T>
	constexpr const vector<T>& getComponents() const {
		const auto it    = map.find ( Component::getCmpTypeID<T>() );

		const auto * cmpVector = dynamic_cast<const ComponentVector<T>*>(it->second.get());

		return cmpVector->components;
	}

	template <typename T>
	constexpr vector<T>& getComponents() {
		return const_cast<vector<T> &>(std::as_const(*this).getComponents<T>());
	}

	template <typename T>
	constexpr T& createComponent(T&& cmp) {
		const auto it = map.find ( Component::getCmpTypeID<T>() );

		auto * cmpVector  = dynamic_cast<ComponentVector<T>*>(it->second.get());

		for (auto& item : cmpVector->components)
			if (!item)
				return item = std::forward<T>(cmp);

		return cmpVector->components.emplace_back(cmp);
	}

	INode * createMesh(const std::string_view);
	INode * createAnimatedMesh(const std::string_view);

	void    setMesh(INode*, const std::string_view);
	void    setAnimatedMesh(INode*, const std::string_view);

	INode * createCamera();
	INode * createLight(const glm::vec3& diff, const glm::vec3& spe, const glm::vec3& dir);

	void removeNode(const INode * n);

private:

	template <typename T>
	constexpr void printVecInfo() const {
		const auto * cmpVector = dynamic_cast<const ComponentVector<T>*>(map.at(Component::getCmpTypeID<T>()).get());
		std::cout << "\n" << T::getName() << "\n\tSize:\t" << cmpVector->components.size() << "\n\tCapacity:\t" << cmpVector->components.capacity() << "\n";
	}

	const IEngine& engine;

	vector<INode*> nodes {};

	ComponentMap map {};
};