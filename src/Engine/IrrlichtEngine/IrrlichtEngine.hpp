#pragma once

#include <Engine/EngineInterface/IEngine.hpp>
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

struct IrrlichtEngine final : public virtual IEngine {

    ~IrrlichtEngine() override = default;

    void init(unsigned width, unsigned height, const std::wstring& name) override;

    [[nodiscard]] bool run() const override;

    void shutdown() const override;

    [[nodiscard]] bool isKeyPressed(KEY_CODE code) const override;

    [[nodiscard]] const Mouse & getMouse() override;

    void clear(Color color) const override;

    void draw() const override;

    void display() const override;

    private:
        irr::IrrlichtDevice *       device       { nullptr };
        irr::scene::ISceneManager * sceneManager { nullptr };
        irr::video::IVideoDriver *  videoDriver  { nullptr };

        InputEventReceiver          eventReceiver;
};