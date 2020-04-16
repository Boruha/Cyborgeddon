#pragma once

#include <Engine/EngineInterface/IEngine.hpp>
#include <Engine/util/Mouse.hpp>
#include <Engine/SunlightEngine/SceneInterface/Resource/ResourceManager.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/SceneManager.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/Video.hpp>

struct GLFWwindow;

struct SunlightEngine final : public virtual IEngine {

    ~SunlightEngine() override = default;

    void init(unsigned width, unsigned height, std::string_view name) final;

    [[nodiscard]] bool run() const final;

    void shutdown() const final;

    [[nodiscard]] bool isKeyPressed(unsigned code) const final;

    [[nodiscard]] const Mouse & getMouse() final;

    void clear(Color color) const final;

    void draw() const final;

    void display() const final;

    [[nodiscard]] glm::vec2 getViewport() const { return { windowWidth, windowHeight }; }

    void setViewport(const glm::vec2& v) { windowWidth = v.x, windowHeight = v.y; }

	[[nodiscard]] IVideo * loadVideo(const std::string_view path) override {
		return static_cast<IVideo *>(&video.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(path.data()),
				std::forward_as_tuple(resourceManager.get(), path)
		).first->second);
    };

	void unloadVideo(const std::string_view path) override { video.erase(path.data()); resourceManager->removeVideo(path); };

	void unloadVideos() override { for (const auto& pair : video) resourceManager->removeVideo(pair.first); video.clear(); };

	private :
		GLFWwindow * window { nullptr };
		Mouse mouse;
		unsigned windowWidth, windowHeight;

		std::unique_ptr<ResourceManager> resourceManager;
		std::unique_ptr<SceneManager> sceneManager;

		std::unordered_map<std::string, Video> video {};
};