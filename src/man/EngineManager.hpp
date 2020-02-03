#pragma once

#include <memory>

enum EngineType { IRRLICHT, SUNLIGHT };

struct Engine;
struct IrrlichtEngine;
struct SunlightEngine;

struct EngineManager {
    explicit EngineManager(EngineType);

    [[nodiscard]] std::unique_ptr<Engine> getEngine() const;

    private :
        const EngineType type;
};
