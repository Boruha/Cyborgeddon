#include <man/EngineManager.hpp>

#include <Engine/IrrlichtEngine/IrrlichtEngine.hpp>
#include <Engine/SunlightEngine/SunlightEngine.hpp>

#include <iostream>

EngineManager::EngineManager(const EngineType engineType) : type(engineType) {  }

std::unique_ptr<IEngine> EngineManager::getEngine() const {
    switch (type) {
//        case IRRLICHT : return std::make_unique<IrrlichtEngine>();
        case SUNLIGHT : return std::make_unique<SunlightEngine>();
        default       : std::cerr << "\n\nMotor distinto de irrlicht y sunlight?!?\n\n";
                        exit(-1);
    }
}