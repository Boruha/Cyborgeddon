#pragma once

struct SoundMessage {
	explicit SoundMessage(const char * const eventName)
		: soundEventName(eventName) { }

	const char * const soundEventName { nullptr };
};