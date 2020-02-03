#pragma once

#include <Engine/EngineInterface/Engine.hpp>

struct SunlightEngine final : public virtual Engine {

    void init(unsigned width, unsigned height, const wchar_t * name) final {

    }

    [[nodiscard]] bool run() const final {
        return false;
    }

    void shutdown() const final {

    }

    [[nodiscard]] bool isKeyPressed(const KEY_CODE code) const final {
        return false;
    }

    [[nodiscard]] const Mouse & getMouse() const final {

    }

    void clear(const Color color) const final {

    }

    void draw() const final {

    }

    void display() const final {

    }
};