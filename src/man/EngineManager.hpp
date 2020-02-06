#pragma once

#include <memory>

enum EngineType { IRRLICHT, SUNLIGHT };

struct IEngine;
struct IrrlichtEngine;
struct SunlightEngine;

struct EngineManager {
    explicit EngineManager(EngineType);

    [[nodiscard]] std::unique_ptr<IEngine> getEngine() const;

    private :
        const EngineType type;
};
