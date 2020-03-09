#include <util/State.hpp>
#include <src/Engine/util/Math.hpp>

void State::init()
{
	assert(state != NO_STATE);

	switch (state)
	{
		case NO_STATE :
			next_state = nullptr;
			break;
		case INIT :
			next_state = &State::initNextState;
			break;
		case INGAME :
			next_state = &State::ingameNextState;
			context->createLevel();
			break;
		case PAUSE :
			next_state = &State::pauseNextState;
			break;
		case ENDING :
			next_state = &State::endingNextState;
			break;
	}

	for (const auto & s : systems)
		s.system->init();
}

StateEnum State::update() {
	const double elapsed = clock.getElapsedAndReset();

	for (auto & s : systems) {
		s.delta += elapsed;

		if (s.delta >= s.fixed) {
			s.system->update(context, float(s.fixed));
			s.delta -= elapsed;
		}
	}

	return next_state(context);
}

void State::reset() const {
	for (const auto & s : systems)
		s.system->reset();
}

StateEnum State::ingameNextState(const Context& context) {
	if (context->checkVictory())
		return ENDING;
	if (context->checkDefeat())
		return ENDING;
	if (context->isKeyPressed(KEY_P))
		return PAUSE;

	return INGAME;
}

StateEnum State::pauseNextState(const Context & context) {
	if (context->isKeyPressed(KEY_INTRO))
		return INGAME;

	return PAUSE;
}