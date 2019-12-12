#pragma once

#include <SunlightEngine/Vector2.hpp>
#include <SunlightEngine/Device.hpp>

struct RenderSystem {
	explicit RenderSystem(const Sun::Vector2u& dim, const wchar_t* name) : device(Sun::Device(dim, name)) {  }

	void init() {  }
	void update();

	Sun::Device device {Sun::Vector2u(640, 480), L"Cyborgeddon" };
	Sun::Color background { 0x330044FF };
};