#pragma once

#include <irrlicht/IEventReceiver.h>

class EventReceiver : public irr::IEventReceiver
{
private:
	bool keys[irr::KEY_KEY_CODES_COUNT]{};

public:
	EventReceiver()
	{
		for (bool& key : keys)
		{
			key = false;
		}
	}

	inline bool OnEvent(const irr::SEvent& event) override
	{
		if(event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
		}

		return false;
	}

	[[nodiscard]] inline virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const
	{
		return keys[keyCode];
	}
};