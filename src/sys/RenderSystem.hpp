#pragma once

#include <SunlightEngine/Vector2.hpp>
#include <SunlightEngine/Device.hpp>
#include <sys/System.hpp>

struct RenderSystem : public System {
	explicit RenderSystem(const Sun::Vector2u& dim, const wchar_t * const name) : device(Sun::Device(dim, name)) { std::cout << "ventana construida\n"; }

	void init() override {  }
	void update(const std::unique_ptr<GameContext> &context, float deltaTime) const override;
	void reset() override {  }

	void updateFPS(float deltaTime);

	Sun::Device device {Sun::Vector2u(640, 480), L"Cyborgeddon" };
	Sun::Color background { 0x330044FF };

	float timeForFPS {0};
	unsigned int FPS_counter {0};
};