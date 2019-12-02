#pragma once

#include <irrlicht/irrlicht.h>

struct RenderSystem
{
	RenderSystem() : device(irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(640, 480))) {}
	~RenderSystem() { device->drop(); }

	int init();
	void update();

	irr::IrrlichtDevice* device { nullptr };
};