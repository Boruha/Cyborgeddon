#pragma once

#include <SunlightEngine/Vector2.hpp>
#include <SunlightEngine/Device.hpp>
#include <memory>

struct RenderSystem {
	RenderSystem() = default;
	RenderSystem(const Sun::Vector2u& dim, const wchar_t* name) : device(Sun::Device(dim, name)) {  }
	~RenderSystem() = default;

	void init() {  }
	void update();

	Sun::Device device {Sun::Vector2u(640, 480), L"Cyborgeddon" };
};