#pragma once

#include <sys/System.hpp>
#include <util/GameConstants.hpp>
#include <Engine/EngineInterface/IEngine.hpp>
#include <Engine/util/Color.hpp>

struct RenderSystem : public System {
	explicit RenderSystem(const glm::vec2& dim, const wchar_t * const name, const IEngine * const engine = nullptr) : engine(engine) { }

	void init() override {  }
	void update(const std::unique_ptr<GameContext> &context, float deltaTime) override;
	void reset() override {  }

	const IEngine * const engine { nullptr };
	const Color background { static_cast<const COLOR>( BACKGROUND_COLOR ) };
};