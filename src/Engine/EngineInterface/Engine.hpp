#pragma once

#include <Engine/util/Enum.hpp>
#include <Engine/EngineInterface/SceneInterface/Scene.hpp>

#include <memory>

struct Mouse;
struct Color;

struct Engine {
    virtual ~Engine() = default;
    virtual void init(unsigned, unsigned, const wchar_t *)  = 0;
    [[nodiscard]] virtual bool run()                  const = 0;
    virtual void shutdown()                           const = 0;

    [[nodiscard]] virtual bool isKeyPressed(KEY_CODE) const = 0;
    [[nodiscard]] virtual const Mouse & getMouse()    const = 0;

    virtual void clear(Color)                         const = 0;
    virtual void draw()                               const = 0;
    virtual void display()                            const = 0;

    std::unique_ptr<Scene> scene { nullptr };
};