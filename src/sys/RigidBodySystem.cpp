#include <sys/RigidBodySystem.hpp>
#include <Engine/util/MathIntersection.hpp>

void RigidBodySystem::update(const Context &context, float deltaTime) {
	for (auto& rigid : context->getComponents().get<RigidMovSphere>()) {
		if (rigid) {
			vec3& velocity = *rigid.velocity;

			// recorremos cada coordenada -> x z (ignoramos y)
			for (unsigned i = 0; i < 3; i+=2) {
				if (equal_e(velocity[i], 0))
					continue;
				// sumamos la velocidad a la posicion para comprobar la posicion futura SOLO EN ESE EJE
				rigid.position[i] += velocity[i];
				// comprobamos con cada collider
				for (const auto& collider : context->getComponents().get<BoundingBox>()) {
					// comprobamos la colision de la esfera con la bounding
					if (collider && intersectionSphereAABB(rigid.position, rigid.radius, collider.min, collider.max)) {
						// si colisiona, dejamos la posicion como estaba
						rigid.position[i] -= velocity[i];
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