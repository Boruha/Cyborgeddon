#pragma once

struct SoundMessage {
	explicit SoundMessage(const char * const eventName)
		: soundEventName(eventName) { }

	const char * const soundEventName { nullptr };
};

struct DeathMessage {
    explicit DeathMessage(const EntityID _ID)
        : ID(_ID) { }

    const EntityID ID {0};
};

struct TriggerMessage {
	explicit TriggerMessage(const EntityType _type1, const EntityID _ID1, const EntityType _type2, const EntityID _ID2)
		: type1(_type1), ID1(_ID1), type2(_type2), ID2(_ID2) { }

	const EntityType type1 { UNDEFINED };
	const EntityID ID1 { 0 };

	const EntityType type2 { UNDEFINED };
	const EntityID ID2 { 0 };
};