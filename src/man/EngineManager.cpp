#include <man/EngineManager.hpp>

EngineManager::EngineManager(const EngineType engineType) : type(engineType) {

}

void EngineManager::init(const Vector2u& dim) {
    engine[type]->init(dim);
}

void EngineManager::run() {
    engine[type]->run();
}

void EngineManager::shutdown() {
    engine[type]->shutdown();
}
