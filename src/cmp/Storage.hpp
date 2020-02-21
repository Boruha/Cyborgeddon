#pragma once

#include <cmp/ComponentPool.hpp>
#include <src/Engine/EngineInterface/IEngine.hpp>

struct Storage {
	explicit Storage(const IEngine * const _engine) : engine(_engine) {  }
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

	const vector<Node_ptr>& getNodes() const {
		return nodes;
	}

	vector<Node_ptr>& getNodes() {
		return const_cast<vector<Node_ptr> &>(std::as_const(*this).getNodes());
	}

	template <typename T, typename ... Args>
	const T& createComponent(Args&& ... args) const {
        return pool.createComponent<T>(args ...);
	}

    template <typename T, typename ... Args>
    T& createComponent(Args&& ... args) {
        return const_cast<T&>(std::as_const(*this).createComponent<T>(args...));
    }

	template <typename ... Args>
	INode* createIObjectNode(Args&& ... args) {
	//	std::cout << "\n\nINode\n";
	//	printVecInfo(nodes);

		for (auto& item : nodes)
			if (!(*item))
				return (item = std::move(engine->scene->addObjectNode(args...))).get();

		return nodes.emplace_back(engine->scene->addObjectNode(args...)).get();
	}

	template <typename ... Args>
	INode* createICameraNode(Args&& ... args) {
	//	std::cout << "\n\nINode\n";
	//	printVecInfo(nodes);

		for (auto& item : nodes)
			if (!(*item))
				return (item = std::move(engine->scene->addCameraNode(args...))).get();

		return nodes.emplace_back(engine->scene->addCameraNode(args...)).get();
	}

	void initData(unsigned maxComponents);
	void cleanData();

private:

	template <typename T>
	void printVecInfo(const vector<T>& vec) const {
		std::cout 	<< "Capacity: " << vec.capacity() 	<< "\n"
					<< "Size: " 	<< vec.size() 		<< "\n";
	}

	const IEngine * const engine { nullptr };

	ComponentPool pool;

	vector<Node_ptr> nodes;
};