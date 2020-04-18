#include <sys/RigidBodySystem.hpp>
#include <Engine/util/MathIntersection.hpp>

void RigidBodySystem::fixedUpdate(const Context &context, float deltaTime) {
	for (auto& sphere : context->getComponents().getComponents<TriggerMovSphere>()) {
		if (sphere) {
			vec3& velocity = *sphere.velocity;

			// recorremos cada coordenada -> x z (ignoramos y)
			for (unsigned i = 0; i < 3; i+=2) {
				if (equal_e(velocity[i], 0))
					continue;
				// sumamos la velocidad a la posicion para comprobar la posicion futura SOLO EN ESE EJE
				sphere.position[i] += velocity[i];
				// comprobamos con cada collider
				for (const auto& rigid : context->getComponents().getComponents<RigidStaticAABB>()) {
					// comprobamos la colision de la esfera con la bounding
					if (rigid && intersectionSphereAABB(sphere.position, sphere.radius, rigid.min, rigid.max)) {
						// si colisiona, dejamos la posicion como estaba
						sphere.position[i] -= velocity[i];
						// ponemos la velocidad en ese eje a 0
						velocity[i] = 0.f;
						// salimos de la funcion
						break;
					}
				}
			}
		}
	}
}