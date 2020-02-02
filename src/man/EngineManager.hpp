#pragma once

#include <Engine/IrrlichtEngine/IrrlichtEngine.hpp>
#include <Engine/SunlightEngine/SunlightEngine.hpp>
#include <memory>

enum EngineType { IRRLICHT, SUNLIGHT };

struct EngineManager {
    explicit EngineManager(EngineType);

    [[nodiscard]] std::unique_ptr<Engine> getEngine() const;

    private :
        const EngineType type;
};
