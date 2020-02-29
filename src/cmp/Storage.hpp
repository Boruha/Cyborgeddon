#pragma once

#include <cmp/ComponentPool.hpp>
#include <src/Engine/EngineInterface/IEngine.hpp>

struct Storage {
	explicit Storage(const IEngine * const _engine) : engine(*_engine) {  }
	~Storage() = default;

	Storage(const Storage& ) = delete;
	Storage(	  Storage&&) = delete;

	Storage& operator=(const Storage& ) = delete;
	Storage& operator=(		 Storage&&) = delete;

	[[nodiscard]] const ComponentPool& getComponents() const {
		return pool;
	}

	ComponentPool& getComponents() {
		return const_cast<ComponentPool &>(std::as_const(*this).getComponents());
	}

	template <typename T>
	const T& createComponent(T&& cmp) const {
		return pool.createComponent(std::forward<T>(cmp));
	}

    template <typename T>
    T& createComponent(T&& cmp) {
        return const_cast<T&>(std::as_const(*this).createComponent(std::forward<T>(cmp)));
    }

	INode* createIObjectNode() {
	//	std::cout << "\n\nINode\n";
	//	printVecInfo(nodes);

		for (auto& item : nodes)
			if (!(*item))
				return (item = std::move(engine.scene->addObjectNode())).get();

		return nodes.emplace_back(engine.scene->addObjectNode()).get();
	}

	INode* createICameraNode() {
	//	std::cout << "\n\nINode\n";
	//	printVecInfo(nodes);

		for (auto& item : nodes)
			if (!(*item))
				return (item = std::move(engine.scene->addCameraNode())).get();

		return nodes.emplace_back(engine.scene->addCameraNode()).get();
	}

	void initData(unsigned maxComponents);
	void cleanData();

private:

	template <typename T>
	void printVecInfo(const vector<T>& vec) const {
		std::cout 	<< "Capacity: " << vec.capacity() 	<< "\n"
					<< "Size: " 	<< vec.size() 		<< "\n";
	}

	const IEngine& engine;

	ComponentPool pool;

	vector<Node_ptr> nodes;
};