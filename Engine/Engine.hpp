#pragma once

#include <util/Vector2.hpp>

struct Engine {
    virtual void init(const Vector2u&)      = 0;
    virtual void run()                      = 0;
    virtual void shutdown()                 = 0;
};