#pragma once

#include <Engine/EngineInterface/IEngine.hpp>
#include <Engine/EngineInterface/SceneInterface/IScene.hpp>
#include <Engine/util/Mouse.hpp>

struct GLFWwindow;

struct SunlightEngine final : public virtual IEngine {

    ~SunlightEngine() override = default;

    void init(unsigned width, unsigned height, const std::wstring& name) final;

    [[nodiscard]] bool run() const final;

    void shutdown() const final;

    [[nodiscard]] bool isKeyPressed(KEY_CODE code) const final;

    [[nodiscard]] const Mouse & getMouse() final;

    void clear(Color color) const final;

    void draw() const final;

    void display() const final;

	private :
		GLFWwindow * window { nullptr };
		Mouse mouse;
		unsigned windowWidth, windowHeight;
};