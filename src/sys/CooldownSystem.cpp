#include <sys/CooldownSystem.hpp>

bool CooldownSystem::shootReady() {

    generalClock = clock();

    // CADENCIA DE LA BALA (en microsegundos)
    // 100000 = Enfriamiento de 1 segundo; 30000 ~= 3 balas/segundo
    if(bulletClock - lastBullet > 100000) {
        lastBullet = bulletClock;
        bulletClock = 0;
        return true;
    }
    else {
        bulletClock = generalClock;
        return false;
    }
}

bool CooldownSystem::dashReady() {

    generalClock = clock();

    // ENFRIAMIENTO DEL DASH (en microsegundos)
    // 100000 = Enfriamiento de 1 segundo; 1000000 = Dash cada 10 segundos
    if(dashClock - lastDash > 1000000) {
        lastDash = dashClock;
        dashClock = 0;
        return true;
    }
    else {
        dashClock = generalClock;
        return false;
    }
}