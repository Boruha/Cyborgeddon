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
	explicit State(const StateEnum _state, Context& _context) : state(_state), context(_context) {  }

	void init();

	StateEnum update();

	void reset() const;

	template <typename T>
	void registerSystem(const double fixedDelta = FIXED_DELTA_TIME) {
		systems.emplace_back( SystemDeltaFixed { std::make_unique<T>(), fixedDelta, 0.0 } );
	}

	const StateEnum state { NO_STATE };

	private:

		StateEnum (*next_state)(const Context&) { nullptr };

		static StateEnum initNextState(const Context&);
		static StateEnum tutorialNextState(const Context&);
		static StateEnum ingameNextState(const Context&);
		static StateEnum pauseNextState(const Context&);
		static StateEnum endingNextState(const Context&){};

		Context& context;
		std::vector<SystemDeltaFixed> systems;

		Timer clock { duration::seconds }; // reloj que controla el juego
};