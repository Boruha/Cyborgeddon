#pragma once

#include <Engine/Engine.hpp>

struct SunlightEngine : Engine {
    void init(const unsigned width, const unsigned height, const wchar_t * const name) final {

    };

    [[nodiscard]] bool run() const final {
        return false;
    };

    void shutdown() const final {

    };

    [[nodiscard]] bool isKeyPressed(KEY_CODE code) const final {
        return false;
    }

    [[nodiscard]] const Mouse& getMouse() const final {
        return mouse;
    }

    void clear(Color color) const final {

    }

    void draw() const final {

    }

    void display() const final {

    }

    private:
        Mouse mouse;
};