#include <man/EngineManager.hpp>
#include <cassert>

EngineManager::EngineManager(const EngineType engineType) : type(engineType) {  }

std::shared_ptr<Engine> EngineManager::getEngine() {
    assert(engine[type]);
    return std::move(engine[type]);
}