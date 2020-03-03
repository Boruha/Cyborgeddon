#pragma once

#include <sys/System.hpp>

struct CooldownSystem : System {

	~CooldownSystem() override = default;

    void init() override {  };
    void update(const Context& context, float deltaTime) override;
	void reset() override {  }
};


