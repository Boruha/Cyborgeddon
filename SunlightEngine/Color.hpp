#pragma once

#include <irrlicht/irrlicht.h>

#include <SunlightEngine/Exception.hpp>

namespace Sun {

	struct ColorData {
		ColorData(unsigned short r, unsigned short g, unsigned short b, unsigned short a) : r(r), g(g), b(b), a(a) {  }
		unsigned short r, g, b, a;
	};

	struct Color {
		Color() = default;

		Color(const u_int32_t& color) : r(color >> 24u & 0xFF), g(color >> 16 & 0xFF), b(color >> 8 & 0xFF), a(color & 0xFF) { }

		Color(const unsigned short &r, const unsigned short &g, const unsigned short &b) : r(r), g(g), b(b) {}

		Color(const unsigned short &r, const unsigned short &g, const unsigned short &b, const unsigned short &a) : r(r), g(g), b(b), a(a) {}

		void setRGB(unsigned short &vr, unsigned short &vg, unsigned short &vb) {
			checkRange(vr);
			checkRange(vg);
			checkRange(vb);

			r = vr;
			g = vg;
			b = vb;
		}

		void setRGBA(unsigned short &vr, unsigned short &vg, unsigned short &vb, unsigned short &va) {
			checkRange(vr);
			checkRange(vg);
			checkRange(vb);
			checkRange(va);

			r = vr;
			g = vg;
			b = vb;
			a = va;
		}

		static void checkRange(unsigned short &value) {
			if (0 > value || value > 255)
				throw OutOfRangeException("Color", 0, 255);
		}

		[[nodiscard]] ColorData getColorData() const { return ColorData(r,g,b,a); }

	private:
		unsigned short r{0}, g{0}, b{0}, a{255};
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