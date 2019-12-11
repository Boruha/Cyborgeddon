#include <sys/AI_System.hpp>
#include <cmath>

// TODO: considerar los estados de la IA como punteros a funcion
void AI_System::update(const std::vector<std::unique_ptr<EntityPlayer>> &players,
					   const std::vector<std::unique_ptr<EntityEnemy>> &enemies) {

	for (auto &player : players) {
		for (auto &enemy : enemies) {
			enemy->velocity.direccion.x = player->transformable.position.x - enemy->transformable.position.x;
			enemy->velocity.direccion.z = player->transformable.position.z - enemy->transformable.position.z;
			float distance = sqrt(enemy->velocity.direccion.x * enemy->velocity.direccion.x + enemy->velocity.direccion.z * enemy->velocity.direccion.z);

			if (distance > 25) {
				enemy->ai_state = 1;
				chasing(enemy, distance);
			} else {
				enemy->ai_state = 0;
				enemy->velocity.direccion *= 0;
			}
		}
	}
}

// TODO: El enemigo solo rota cuando nos va a seguir.
//  	 Debe rotar sin importar la distancia o rotar interpoladamente al salir de su rango
// 		 Ahora nos sigue hasta que entramos en su rango, pero gira repentinamente cuando volvemos
// 		 a entrar a esta funcion
void AI_System::chasing(const std::unique_ptr<EntityEnemy> &enemy, const float &distance) {
	enemy->velocity.direccion /= distance;

	float angle = getAngleAxisZ(enemy->velocity.direccion);
	enemy->transformable.rotation = Vector3f(0, angle, 0);
	enemy->node.setRotation(enemy->transformable.rotation);
}

// TODO: La clase vector debe encargarse de devolver este angulo
float AI_System::getAngleAxisZ(const Vector3f &direccion) {
	float modul_XZ = direccion.length();

	float cos = direccion.z / modul_XZ;
	auto angle = (std::acos(cos) * 180) / 3.1415;

	if (cos >= 0 && direccion.x < 0)
		angle = -angle;
	else if (cos < 0 && direccion.x < 0)
		angle = 180 - (angle - 180);

	return angle;
}