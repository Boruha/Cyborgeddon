#pragma once

#include <util/SoundParameter.hpp>


// cuando haya que hacer sonar algo, mandaremos un mensaje de sonido
// con el nombre del evento (intentar que el nombre no sea mayor de 16
// caracteres para que c++ lo guarde en la pila y no el stack)

// La gestion de los mensajes se lleva a cabo en sound system
struct SoundMessage {
	explicit SoundMessage(const SoundParameter param)
            : tuple(getSoundTuple(param)) { }

    const sound_parameter_tuple tuple;
};


// cuando cualquier tipo de entidad tenga que morir se creara un mensaje
// de muerte, ya sea bala, enemigo, jugador...

// La gestion de los mensajes se lleva a cabo en death system
struct DeathMessage {
    explicit DeathMessage(const EntityID _ID)
        : ID(_ID) { }

    const EntityID ID {0};
};

// cuando cualquier tipo de trigger entra en contacto con otro, se lanza
// un mensaje, sin tener en cuenta el tipo de trigger en cuestion
// teniendo los ID de entidad y los tipos de cada una de ellas, podemos
// resolver la colision adecuadamente

// La gestion de los mensajes se lleva a cabo en trigger resolution system

struct TriggerMessage {
	explicit TriggerMessage(const EntityType _type1, const EntityID id1, const EntityType _type2, const EntityID id2)
		: type1(_type1), ID1(id1), type2(_type2), ID2(id2) { }

	const EntityType type1 { UNDEFINED };
	const EntityID ID1 { 0 };

	const EntityType type2 { UNDEFINED };
	const EntityID ID2 { 0 };
};

// cuando un enemigo ataca al player con un ataque basico, se lanza un mensaje
// indicando el daño realizado, y el tipo de daño en cuestion
// el player no lanza este tipo de mensajes porque la unica forma que tiene
// de atacar es lanzando balas que pueden fallar (y esto se controla en trigger
// system), mientras que los enemigos no pueden fallar sus ataques basicos
// como hay varios tipos de enemigos y cada uno de ellos ataca de una forma
// distinta con distinto tipo de daño, es necesario saber su tipo para hacer una
// adecuada gestion del mismo

// La gestion de los mensajes se lleva a cabo en attack system

struct DamageMessage {
	explicit DamageMessage(const float dmg)
		: damage(dmg) { }

	const float damage 	{ 	 0 	  };
	const ModeType type { NEUTRAL };
};