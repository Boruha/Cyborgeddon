#pragma once

#include <irrlicht/irrlicht.h>

#include <SunlightEngine/Exception.hpp>

namespace Sun {

	struct ColorData {
		ColorData(const u_int8_t&  r, const u_int8_t&  g, const u_int8_t&  b, const u_int8_t&  a) : r(r), g(g), b(b), a(a) {  }
        const u_int8_t&  r, g, b, a;
	};

	struct Color {
		Color() = default;

		Color(const u_int32_t& color) : r(color >> 24 & 0xFF), g(color >> 16 & 0xFF), b(color >> 8 & 0xFF), a(color & 0xFF) { }

		Color(const u_int8_t& r, const u_int8_t& g, const uint8_t& b) : r(r), g(g), b(b) {  }

		Color(const u_int8_t& r, const u_int8_t& g, const u_int8_t& b, const u_int8_t& a) : r(r), g(g), b(b), a(a) {}

		[[nodiscard]] ColorData getColorData() const { return ColorData(r,g,b,a); }

	private:
		uint8_t r{0x00}, g{0x00}, b{0x00}, a{0xFF};
	};

	enum COLOR {
	    TRANSPARENT = 0x00000000,
	    BLACK       = 0x000000FF,
	    RED         = 0xFF0000FF,
	    GREEN       = 0x00FF00FF,
	    BLUE        = 0x0000FFFF,
	    YELLOW      = 0xFFFF00FF,
	    MAGENTA     = 0xFF00FFFF,
	    CYAN        = 0x00FFFFFF,
	    WHITE       = 0xFFFFFFFF
	};
}