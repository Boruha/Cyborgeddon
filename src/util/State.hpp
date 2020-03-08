#pragma once

#include <vector>
#include <memory>

#include <sys/System.hpp>
#include <src/Engine/util/Timer.hpp>

struct SystemDeltaFixed {
	std::unique_ptr<System> system  { };
	const double fixed { 0.0 };
	double delta { 0.0 };
};

struct State {
	explicit State(const StateEnum _state) : state(_state) {  }

	void init() const;

	StateEnum update();

	void reset() const;

	template <typename T>
	void registerSystem(T sys, const double fixedDelta = FIXED_DELTA_TIME) {
		systems.emplace_back( SystemDeltaFixed { std::move(sys), fixedDelta, 0.0 } );
	}

	void setContext(Context& c, const double fixedDelta = FIXED_DELTA_TIME) {
		context = std::move(c);
	}

	const StateEnum state { NO_STATE };

	private:

		Context context;
		std::vector<SystemDeltaFixed> systems;

		Timer clock { duration::seconds }; // reloj que controla el juego
};