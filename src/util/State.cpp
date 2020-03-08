#include <util/State.hpp>

void State::init() const {
	for (const auto & s : systems)
		s.system->init();

	context->init();
}

StateEnum State::update() {
	const double elapsed = clock.getElapsedAndReset();

	for (auto & s : systems) {
		s.delta += elapsed;

		if (s.delta > s.fixed) {
			s.system->update(context, float(s.fixed));
			s.delta -= elapsed;
		}
	}

	return INGAME;
}

void State::reset() const {
	for (const auto & s : systems)
		s.system->reset();
}
