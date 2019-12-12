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

		void clear(Color* color) const { device->getVideoDriver()->beginScene(true, true,
																	   irr::video::SColor(color->getColorData().a,
																								color->getColorData().r,
																								color->getColorData().g,
																								color->getColorData().b));
		}

		void draw() const { device->getSceneManager()->drawAll(); }

		void showDrawn() const { device->getVideoDriver()->endScene(); }

		void setEventReceiver(EventReceiver* receiver) const { device->setEventReceiver(receiver); }

		[[nodiscard]] bool isActive() const { return device->run(); }

		// TODO: eliminar esta funcion cuando todas las necesidades de irrlicht esten cubiertas
		[[nodiscard]] irr::IrrlichtDevice* getInnerDevice() const { return device; }

		private:
			irr::IrrlichtDevice *device{nullptr};
	};
}