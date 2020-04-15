#pragma once

#include <irrlicht/IEventReceiver.h>
#include <Engine/util/Keyboard.hpp>
#include <Engine/util/Mouse.hpp>


struct InputEventReceiver : irr::IEventReceiver {

    bool OnEvent(const irr::SEvent& event) override;

    [[nodiscard]] bool IsKeyDown(unsigned keyCode) const;

    [[nodiscard]] const Mouse& getMouse() const;

    private:
        Keyboard keyboard;
        Mouse mouse;
};