#pragma once

#include <irrlicht/IEventReceiver.h>
#include <SunlightEngine/KeyCodes.hpp>
#include <Engine/util/Mouse.hpp>

namespace Sun {

	struct EventReceiver : irr::IEventReceiver {

		bool OnEvent(const irr::SEvent& event) override {
			switch (event.EventType) {

				case irr::EET_MOUSE_INPUT_EVENT :

					switch(event.MouseInput.Event) {

						case irr::EMIE_LMOUSE_PRESSED_DOWN :
							mouse.leftPressed = true;
							break;

						case irr::EMIE_LMOUSE_LEFT_UP :
							mouse.leftPressed = false;
							break;

						case irr::EMIE_RMOUSE_PRESSED_DOWN :
							mouse.rightPressed = true;
							break;

						case irr::EMIE_RMOUSE_LEFT_UP :
							mouse.rightPressed = false;
							break;

						case irr::EMIE_MOUSE_MOVED :
							mouse.position.x = event.MouseInput.X;
							mouse.position.y = event.MouseInput.Y;
							break;

						default:
							// No usamos la rueda de momento, ni los botones auxiliares
							break;
					}

					break;

				case irr::EET_KEY_INPUT_EVENT :

					keyboard.keys[event.KeyInput.Key] = event.KeyInput.PressedDown;

					break;

				default : break;
			}

			return false;
		}

		[[nodiscard]] virtual bool IsKeyDown(KEY_CODE keyCode) const {
			return keyboard.keys[keyCode];
		}

		[[nodiscard]] const Mouse& getMouse() const { return mouse; }

		[[nodiscard]] const Keyboard& getKeyboard() const { return keyboard; }

	private:

		Keyboard keyboard;
		Mouse mouse;
	};
} // namespace Sun