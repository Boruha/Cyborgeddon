#pragma once

#include <sys/System.hpp>
#include <util/GameConstants.hpp>
#include <Engine/EngineInterface/IEngine.hpp>
#include <Engine/util/Color.hpp>
#include <GLFW/glfw3.h>

struct RenderSystem : public System {
	explicit RenderSystem(const glm::vec2& dim, const std::string_view name, const IEngine * const engine)
		: engine(*engine) { }

	void init() override {  }
	void update(const Context &context, float deltaTime) override;
	void reset() override {  }

	const IEngine& engine;
	const Color background { static_cast<const COLOR>( BACKGROUND_COLOR ) };
};