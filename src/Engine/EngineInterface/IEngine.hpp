#pragma once

#include <Engine/util/Enum.hpp>
#include <Engine/EngineInterface/SceneInterface/IScene.hpp>
#include <Engine/util/Mouse.hpp>

#include <memory>

struct Mouse;
struct Color;
struct Rect;

struct IEngine {
    virtual ~IEngine() = default;
    virtual void init(unsigned, unsigned, const wchar_t *)  = 0;
    [[nodiscard]] virtual bool run()                  const = 0;
    virtual void shutdown()                           const = 0;

    [[nodiscard]] virtual bool isKeyPressed(KEY_CODE) const = 0;
    [[nodiscard]] virtual const Mouse & getMouse()    const = 0;

    virtual void clear(Color)                         const = 0;
    virtual void draw()                               const = 0;
    virtual void display()                            const = 0;

    std::unique_ptr<IScene> scene { nullptr };
};