#pragma once

#include <memory>
#include <unordered_map>
#include <Engine/Engine.hpp>
#include <Engine/IrrlichtEngine/IrrlichtEngine.hpp>
#include <Engine/SunlightEngine/SunlightEngine.hpp>

enum EngineType { IRRLICHT, SUNLIGHT };

struct EngineManager : Engine {
    explicit EngineManager(EngineType);

    void init(const Vector2u&)  final;
    void run()                  final;
    void shutdown()             final;

    private :

        const EngineType type;

        std::unordered_map<EngineType, std::unique_ptr<Engine>> engine {
                {IRRLICHT, std::make_unique<IrrlichtEngine>()},
                {SUNLIGHT, std::make_unique<SunlightEngine>()}
        };
};
