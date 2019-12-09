#pragma once

#include <irrlicht/irrlicht.h>

#include <iostream>

#include <SunlightEngine/Vector2.hpp>
#include <SunlightEngine/Color.hpp>
#include <SunlightEngine/EventReceiver.hpp>

namespace Sun {
	struct Device {
		Device() = default;
		Device(const Vector2u& dim, const wchar_t* name) : device(irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(dim.x, dim.y)))
		{
			device->setWindowCaption(name);
		}
		~Device() { if(device) device->drop(); }

		void clear(const Color& color) { device->getVideoDriver()->beginScene(true, true,
																	   irr::video::SColor(color.getColorData().a,
																								color.getColorData().r,
																								color.getColorData().g,
																								color.getColorData().b));
		}

		void draw() { device->getSceneManager()->drawAll(); }

		void showDrawn() { device->getVideoDriver()->endScene(); }

		void setEventReceiver(EventReceiver* receiver) const { device->setEventReceiver(receiver); }

		[[nodiscard]] bool isActive() { return device->run(); }

		[[nodiscard]] irr::IrrlichtDevice* getInnerDevice() const { return device; }

		private:
			irr::IrrlichtDevice *device{nullptr};
	};
}