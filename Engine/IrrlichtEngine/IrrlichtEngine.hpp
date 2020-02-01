#pragma once

#include <Engine/Engine.hpp>
#include <irrlicht/irrlicht.h>

struct IrrlichtEngine : Engine {

    void init(const Vector2u& dim)  final {
        device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(dim.x, dim.y));
    };

    void run()                      final {

    };

    void shutdown()                 final {

    };

private:
    const irr::IrrlichtDevice * device { nullptr };
};

