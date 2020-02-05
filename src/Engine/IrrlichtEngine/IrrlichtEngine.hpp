#pragma once

#include <Engine/EngineInterface/Engine.hpp>
#include <Engine/IrrlichtEngine/InputEventReceiver.hpp>
#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtScene.hpp>

#include <memory>

namespace irr {
	struct IrrlichtDevice;

	namespace scene {
		struct ISceneManager;
	}

	namespace video {
		struct IVideoDriver;
	}
}

struct IrrlichtEngine final : public virtual Engine {

    ~IrrlichtEngine() override = default;

    void init(unsigned width, unsigned height, const wchar_t * name) final;

    [[nodiscard]] bool run() const final;

    void shutdown() const final;

    [[nodiscard]] bool isKeyPressed(KEY_CODE code) const final;

    [[nodiscard]] const Mouse & getMouse() const final;

    void clear(Color color) const final;

    void draw() const final;

    void display() const final;

    private:
        irr::IrrlichtDevice *       device       { nullptr };
        irr::scene::ISceneManager * sceneManager { nullptr };
        irr::video::IVideoDriver *  videoDriver  { nullptr };

        InputEventReceiver          eventReceiver;

};

