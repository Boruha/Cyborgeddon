#pragma once

#include <irrlicht/irrlicht.h>

#include <util/Vector2.hpp>
#include <SunlightEngine/Color.hpp>
#include <SunlightEngine/EventReceiver.hpp>

namespace Sun {
	struct Device {
		Device() = default;
		Device(const Vector2u& dim, const wchar_t * const name) : device(irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(dim.x, dim.y)))
		{
			device->setWindowCaption(name);
		}
		~Device() { if(device) device->drop(); }

		void clear(const Color * const color) const { device->getVideoDriver()->beginScene(true, true,
																	   irr::video::SColor(color->a,
																								color->r,
																								color->g,
																								color->b));
		}

		void draw() const { device->getSceneManager()->drawAll(); }

		void showDrawn() const { device->getVideoDriver()->endScene(); }

		void setEventReceiver(EventReceiver* receiver) const { device->setEventReceiver(receiver); }

		void setWindowName(const wchar_t * const name) const { device->setWindowCaption(name); }

		[[nodiscard]] bool isActive() const { return device->run(); }

		// TODO: eliminar esta funcion cuando todas las necesidades de irrlicht esten cubiertas
		[[nodiscard]] irr::IrrlichtDevice* getInnerDevice() const { return device; }

		private:
			irr::IrrlichtDevice* device { nullptr };
	};
}