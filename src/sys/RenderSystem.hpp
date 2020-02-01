#pragma once

#include <SunlightEngine/Device.hpp>
#include <sys/System.hpp>

struct RenderSystem : public System {
	explicit RenderSystem(const Vector2u& dim, const wchar_t * const name) : device(Sun::Device(dim, name)) { }

	void init() override {  }
	void update(const std::unique_ptr<GameContext> &context, float deltaTime) const override;
	void reset() override {  }

	void updateFPS(float deltaTime);

	Sun::Device device;
	Sun::Color background { 0x330044FF };

	float timeForFPS {0};
	unsigned int FPS_counter {0};
};