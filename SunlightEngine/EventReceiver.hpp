#pragma once

#include <irrlicht/IEventReceiver.h>
#include <SunlightEngine/KeyCodes.hpp>

// TODO : Raton!!!!!!!!!!!!!!!!

namespace Sun {
	struct EventReceiver : public irr::IEventReceiver {
		EventReceiver() {
			std::fill(keys, keys + KEY_CODE_COUNT, false);
		}

		bool OnEvent(const irr::SEvent &event) override {
			if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
				keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
			}
			return false;
		}

		[[nodiscard]] virtual bool IsKeyDown(KEY_CODE keyCode) const {
			return keys[keyCode];
		}

	private:
		bool keys[KEY_CODE_COUNT] {  };
	};
} // namespace Sun