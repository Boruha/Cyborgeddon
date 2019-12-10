#pragma once

#include <irrlicht/IEventReceiver.h>
#include <SunlightEngine/KeyCodes.hpp>

namespace Sun {
	struct EventReceiver : public irr::IEventReceiver {
		EventReceiver() {
			for (bool &key : keys) {
				key = false;
			}
		}

		inline bool OnEvent(const irr::SEvent &event) override {
			if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
				keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
			}
			return false;
		}

		[[nodiscard]] inline virtual bool IsKeyDown(KEY_CODE keyCode) const {
			return keys[keyCode];
		}

	private:
		bool keys[KEY_CODE_COUNT]{};
	};
} // namespace Sun