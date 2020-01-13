#include <sys/CooldownSystem.hpp>

bool CooldownSystem::dashReady() {

    generalClock = clock();

    // ENFRIAMIENTO DEL DASH (en microsegundos)
    // 100000 = Enfriamiento de 1 segundo; 1000000 = Dash cada 10 segundos
    if(dashClock - lastDash > 100000) {
        lastDash = dashClock;
        dashClock = 0;
        return true;
    }
    else {
        dashClock = generalClock;
        return false;
    }
}

void CooldownSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const {
	for (auto& data : context->getCharacterDataComponents()) {
		if ((data.attackingCooldown -= deltaTime) < 0)
			data.attackingCooldown = 0.f;
	}
}
