#pragma once

#include <Engine/EngineInterface/IEngine.hpp>
#include <Engine/EngineInterface/SceneInterface/IScene.hpp>
#include <Engine/util/Mouse.hpp>
#include <Engine/SunlightEngine/SceneInterface/Resource/ResourceManager.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/SceneManager.hpp>

struct GLFWwindow;

struct SunlightEngine final : public virtual IEngine {

    ~SunlightEngine() override = default;

    void init(unsigned width, unsigned height, std::string_view name) final;

    [[nodiscard]] bool run() const final;

    void shutdown() const final;

    [[nodiscard]] bool isKeyPressed(KEY_CODE code) const final;

    [[nodiscard]] const Mouse & getMouse() final;

    void clear(Color color) const final;

    void draw() const final;

    void display() const final;

    glm::vec2 getViewport() { return { windowWidth, windowHeight }; }

    void setViewport(const glm::vec2& v) { windowWidth = v.x, windowHeight = v.y; }

	private :
		GLFWwindow * window { nullptr };
		Mouse mouse;
		unsigned windowWidth, windowHeight;

		std::unique_ptr<ResourceManager> resourceManager;
		std::unique_ptr<SceneManager> sceneManager;
};