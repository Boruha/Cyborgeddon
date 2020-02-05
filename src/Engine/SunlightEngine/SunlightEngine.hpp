#pragma once

#include <Engine/EngineInterface/Engine.hpp>
#include <Engine/EngineInterface/SceneInterface/Scene.hpp>

struct SunlightEngine final : public virtual Engine {

    ~SunlightEngine() override = default;

    void init(unsigned width, unsigned height, const wchar_t * name) final;

    [[nodiscard]] bool run() const final;

    void shutdown() const final;

    [[nodiscard]] bool isKeyPressed(KEY_CODE code) const final;

    [[nodiscard]] const Mouse & getMouse() const final;

    void clear(Color color) const final;

    void draw() const final;

    void display() const final;
};