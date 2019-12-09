#pragma once

#include <SunlightEngine/Vector2.hpp>
#include <SunlightEngine/Device.hpp>
#include <memory>

struct RenderSystem {
	RenderSystem() = default;

	RenderSystem(const Sun::Vector2u& dim, const wchar_t* name) : device(new Sun::Device(dim, name)) {  }
	~RenderSystem() { delete device; }

	int init();
	void update();

	Sun::Device* device { new Sun::Device(Sun::Vector2u(640, 480), L"Cyborgeddon") };
};