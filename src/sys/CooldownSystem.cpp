#include <sys/CooldownSystem.hpp>

bool CooldownSystem::shootReady() {

    //TODO:: NO TENER QUE ESPERAR 1S ENTRE INTENTOS DE DISPARAR JAJAJA

    std::cout << ((float)bulletClock/100000) << std::endl;
    bulletClock = clock() - startTime;
    startTime = clock();
    if(bulletClock > 100000) {
        return true;
    }
    return false;
}
