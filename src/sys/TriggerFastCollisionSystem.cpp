#include <sys/TriggerFastCollisionSystem.hpp>

#include <Engine/util/MathIntersection.hpp>

#include <tuple>

// funcion para calcular el cuadrado de la distancia en el plano XZ
// a partir de min y max de un aabb, y origin como punto de referencia
float getSquareDistanceXZ(const vec3& origin, const vec3& min, const vec3& max) {
	return manhattan( { 0.5f * (min.x + max.x), 0.5f * (min.z + max.z) }, { origin.x, origin.z } );
}

// funcion para calcular el cuadrado de la distancia en el plano XZ
// desde origin hasta end
float getSquareDistanceXZ(const vec3& origin, const vec3& end) {
	return manhattan( { origin.x, origin.z } , { end.x, end.z } );
}

void TriggerFastCollisionSystem::fixedUpdate(const Context &context, float deltaTime) {
	for (auto & fast : context->getComponents().getComponents<TriggerFastMov>()) {
		if (fast) {
			// todas las operaciones que utilizaremos sobre la distancia seran al cuadrado para
			// evitar usar raices cuadradas

			// primero necesito saber la linea de trayectoria de la bala, que la obtengo con la
			// recta -> A = pos, B = pos + vel

			const Line line(fast.position, fast.position + fast.velocity * deltaTime);

			// necesito almacenar la informacion del disparo si este golpea algo

			// id -> id de la entidad golpeada
			// type -> tipo de la entidad golpeada
			// sqDist -> cuadrado de la distancia a la que se encuentra la entidad golpeada
			// respecto a la posicion inicial de la bala

			const float inf = std::numeric_limits<float>::infinity();

			auto [id, type, sqDist] = std::tuple<EntityID, EntityType, float>(0, UNDEFINED, inf );

			// primero vamos a recorrer los componentes rigidos y estaticos
			for (const auto & rigid : context->getComponents().getComponents<RigidStaticAABB>()) {
				// si estan operativos e interseccionan en el plano XZ (ignorando la y)
				// entonces vemos si hay que almacenar sus datos

				if (rigid && lineAABBIntersectionXZ(line, rigid.min, rigid.max)) {
					// nos guardamos la distancia al cuadrado

					const float squareDistance = getSquareDistanceXZ(line.a, rigid.min,
					                                                 rigid.max); // distancia al cuadrado desde el origen de la bala hasta el objeto (bullet.a -> posInicial, bullet.b -> posFinal)
					// si sqDist es < 0 significa que todavia no hemos guardado nada
					// si no lo es, tenemos que ver si la entidad golpeada esta mas
					// cerca que la anterior almacenada

					if (less_e(squareDistance, sqDist)) {
						id = rigid.getEntityID();
						type = rigid.getEntityType();
						sqDist = squareDistance;
					}
				}
			}

			// ahora vamos a comprobar los trigger estaticos aabb, que son presumiblemente
			// puertas y llaves, pero esto puede cambiar
			for (const auto &trigger : context->getComponents().getComponents<TriggerStaticAABB>()) {
				// passable es un flag que determina si la bala puede atravesar o no la entidad que lo contiene

				if (trigger && !trigger.passable && lineAABBIntersectionXZ(line, trigger.min, trigger.max)) {
					// mismo procedimiento

					const float squareDistance = getSquareDistanceXZ(line.a, trigger.min,
					                                                 trigger.max); // distancia al cuadrado desde el origen de la bala hasta el objeto (bullet.a -> posInicial, bullet.b -> posFinal)
					// y mismo procedimiento

					if (less_e(squareDistance, sqDist)) {
						id = trigger.getEntityID();
						type = trigger.getEntityType();
						sqDist = squareDistance;
					}
				}
			}

			// para ver si tocamos un trigger esferico/circular (recordamos que es plano XZ y no se tiene en cuenta la y)
			// tenemos que saber cual es la distancia maxima al cuadrado que viaja la bala

			// esto es asi porque el la funcion de interseccion linea y esfera, solo nos dice si hay contacto o no
			// en una recta infinita, no en un segmento concreto, por lo que una bala puede golpear una entidad que este
			// lejisimos y nosotros ni siquiera saberlo

			// para ahorrar calculos utilizo directamente la velocidad de la bala que es constante,
			// en lugar de calcularla a partir de fast.velocity (que tambien se puede, pero de momento
			// en nuestro juego lo unico que controla este sistema son las balas disparadas por el
			// personaje)

			constexpr float BULLET_SPEED2 = BULLET_SPEED * BULLET_SPEED;

			const float squareDistanceTravelled = BULLET_SPEED2 * deltaTime * deltaTime;

			// utilizamos el mismo procedimiento

			for (const auto &trigger : context->getComponents().getComponents<TriggerMovSphere>()) {
				// como las balas salen del player, y el player tiene este tipo de colisionador
				// tenemos que asegurarnos de que lo ignoramos a la hora de comprobar la colision

				// de lo contrario siempre golpeara al player, y asi ahorramos comprobaciones en
				// el sistema de resolucion de triggers
				if (trigger && trigger.getEntityType() != PLAYER && intersectionLineSphereXZ(line, trigger.position, trigger.radius)) {
					// utilizamos ahora la funcion de la distancia al cuadrado sabiendo directamente la posicion
					// del trigger (antes la calculabamos a partir de min y max de aabb, ahora sabemos el centro
					// de la esfera/circunferencia)

					const float squareDistance = getSquareDistanceXZ(line.a, trigger.position);

					// lo primero que queremos saber es si la distancia al cuadrado a la que golpeamos la entidad
					// es menor que la distancia al cuadrado que recorre la bala

					// queremos saberlo primero, porque es mucho mas probable que la bala haya chocado con
					// una entidad anteriormente

					if (squareDistance < squareDistanceTravelled) {
						if (less_e(squareDistance, sqDist)) {
							id = trigger.getEntityID();
							type = trigger.getEntityType();
							sqDist = squareDistance;
						}
					}
				}
			}

			// si el cuadrado de la distancia es positivo significa que hemos chocado con algo significativo

			if (type) {
				// asi que nos encargamos de mandar al sistema de resolucion de trigger
				// un mensaje con la informacion necesaria para gestionar la colision
				triggerMessages.emplace_back(fast.getEntityType(), fast.getEntityID(), type, id);

				// y mandamos mensaje de muerte de la bala porque se entiende que choque con lo que choque
				// la bala va a morir

				// esto es asi porque nos aseguramos que con cada tipo de colisionador, no comprobamos
				// colisiones que la bala deba ignorar
				deathMessages.emplace_back(fast.getEntityID());
			}

			// IMPORTANTISIMO actualizar la posicion del colisionador de la bala tras
			// la comprobacion de todas las colisiones pertinentes

			fast.position += fast.velocity * deltaTime;
		}
	}
}
