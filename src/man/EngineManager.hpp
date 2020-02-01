#pragma once

#include <memory>
#include <unordered_map>
#include <Engine/IrrlichtEngine/IrrlichtEngine.hpp>
#include <Engine/SunlightEngine/SunlightEngine.hpp>

enum EngineType { IRRLICHT, SUNLIGHT };

struct EngineManager {
    explicit EngineManager(EngineType);

    std::shared_ptr<Engine> getEngine();

    private :

        const EngineType type;

        std::unordered_map<EngineType, std::shared_ptr<Engine>> engine {
                {IRRLICHT, std::make_shared<IrrlichtEngine>() },
                {SUNLIGHT, std::make_shared<SunlightEngine>() }
        };
};
