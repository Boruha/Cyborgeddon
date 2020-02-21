#pragma once

struct SoundMessage {
	explicit SoundMessage(const char * const eventName)
		: soundEventName(eventName) { }

	const char * const soundEventName { nullptr };
};

struct DeathMessage {
    explicit DeathMessage(const EntityID _ID)
        : ID(_ID){ }

    const EntityID ID {0};
};