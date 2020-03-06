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

	template <typename T>
	void initVector(const unsigned size) {
		auto   u_ptr = std::make_unique<ComponentVector<T>>();
		auto *   ptr = u_ptr.get();

		ptr->components.reserve(size);

		map[Component::getCmpTypeID<T>()] = std::move(u_ptr);
	}

	template <typename T>
	const vector<T>& getComponents() const {
		const auto it    = map.find ( Component::getCmpTypeID<T>() );

		auto * cmpVector = dynamic_cast<ComponentVector<T>*>(it->second.get());

		return cmpVector->components;
	}

	template <typename T>
	vector<T>& getComponents() {
		return const_cast<vector<T> &>(std::as_const(*this).getComponents<T>());
	}

	template <typename T>
	T& createComponent(T&& cmp) {
		const auto it = map.find ( Component::getCmpTypeID<T>() );

		auto * cmpVector  = dynamic_cast<ComponentVector<T>*>(it->second.get());

//		std::cout << "\n\n" << cmp.getName() << "\n";
//		printVecInfo(cmpVector->components);

		for (auto& item : cmpVector->components)
			if (!item)
				return item = std::forward<T>(cmp);

		return cmpVector->components.emplace_back(cmp);
	}
/*
	INode* createMesh(std::string_view mesh) {
		std::cout << "\n\n" << "Node" << "\n";
		printVecInfo(nodes);

		INode * n { nullptr };

		for (auto& node : nodes)
			if (!(*node))
				n = (node = engine.scene->addObjectNode(mesh)).get();

		if (!n)
			n = nodes.emplace_back(engine.scene->addObjectNode(mesh)).get();

		return n;
	}

	INode* createCamera() {
		std::cout << "\n\n" << "Node" << "\n";
		printVecInfo(nodes);

		INode * n { nullptr };

		for (auto& node : nodes)
			if (!(*node))
				n = (node = engine.scene->addCameraNode()).get();

		if (!n)
			n = nodes.emplace_back(engine.scene->addCameraNode()).get();

		return n;
	}
*/

	INode* createMesh(std::string_view mesh) {
//		std::cout << "\n\n" << "Node" << "\n";
//		printVecInfo(inodes);

		return inodes.emplace_back(engine.scene->addMeshNode(mesh));
	}

	INode* createCamera() {
//		std::cout << "\n\n" << "Node" << "\n";
//		printVecInfo(inodes);

		return inodes.emplace_back(engine.scene->addFreeCameraNode());
	}
private:

	template <typename T>
	void printVecInfo(const vector<T>& vec) const {
		std::cout 	<< "Capacity: " << vec.capacity() 	<< "\n"
					<< "Size: " 	<< vec.size() 		<< "\n";
	}

	const IEngine& engine;

	vector<std::unique_ptr<INode>> nodes;
	vector<INode*> inodes;

	ComponentMap map;
};