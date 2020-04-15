#pragma once

#include <sys/System.hpp>
#include <Engine/EngineInterface/IEngine.hpp>

struct RenderSystem : System {
	~RenderSystem() override = default;

	void init() override {  }
	void update(const Context &context, float deltaTime) override;
	void reset() override {  }
};