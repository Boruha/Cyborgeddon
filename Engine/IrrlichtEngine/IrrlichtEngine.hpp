#pragma once

#include <Engine/EngineInterface/Engine.hpp>
#include <Engine/IrrlichtEngine/InputEventReceiver.hpp>

#include <irrlicht/irrlicht.h>

struct IrrlichtEngine final : public virtual Engine {

    void init(unsigned width, unsigned height, const wchar_t * name) final {
        device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(width, height));
        device->setWindowCaption(name);
        device->setEventReceiver(&eventReceiver);

        sceneManager = device->getSceneManager();
        videoDriver  = device->getVideoDriver();
    }

    [[nodiscard]] bool run() const final {
        return device->run();
    }

    void shutdown() const final {
        device->drop();
    }

    [[nodiscard]] bool isKeyPressed(const KEY_CODE code) const final {
        return eventReceiver.IsKeyDown(code);
    }

    [[nodiscard]] const Mouse & getMouse() const final {
        return eventReceiver.getMouse();
    }

    void clear(const Color color) const final {
        videoDriver->beginScene(true, true, irr::video::SColor(color.a, color.r, color.g, color.b));
    }

    void draw() const final {
        sceneManager->drawAll();
    }

    void display() const final {
        videoDriver->endScene();
    }

    private:
        irr::IrrlichtDevice *       device       { nullptr };
        irr::scene::ISceneManager * sceneManager { nullptr };
        irr::video::IVideoDriver *  videoDriver  { nullptr };

        InputEventReceiver          eventReceiver;
};

