#include <util/State.hpp>
#include <src/Engine/util/Math.hpp>
#include <src/Engine/EngineInterface/SceneInterface/IVideo.hpp>
#include <src/Engine/EngineInterface/SceneInterface/ITexture.hpp>
#include <thread>

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
			context->createIntro(false);
			context->getEngine().hideCursor();
			break;
		case TUTORIAL :
			next_state = &State::tutorialNextState;
			break;
        case STARTING :
            next_state = &State::startingNextState;
            break;
		case INGAME :
			next_state = &State::ingameNextState;
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

		s.system->update(context, float(elapsed));

		if (s.delta >= s.fixed) {
			s.system->fixedUpdate(context, float(s.fixed));
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
	if (context->checkVictory()) {
		context->createEnding();
		context->getEngine().hideCursor();
		return ENDING;
	}

	if (context->checkDefeat()) {
		context->getEngine().clear(Color(BLACK), true);

		context->getEngine().loadTexture("../resources/menu/load_screen/pantalla_carga.png")->render();

		context->getEngine().hideCursor();

		context->getEngine().display();

		using namespace std::chrono_literals;

		std::this_thread::sleep_for(2s);

		context->createLevel();

		context->getEngine().unhideCursor();

		return INGAME;
	}

	if (context->isKeyTextInput(KEY_SCAPE)) {
		context->getEngine().hideCursor();
		return PAUSE;
	}

	return INGAME;
}

StateEnum State::pauseNextState(const Context & context) {

	if (context->getComponents().getComponents<MenuOption>()[0].option == 0 && (context->isKeyTextInput(KEY_SPACE) || context->isKeyTextInput(KEY_INTRO))) {
		context->getEngine().unhideCursor();
        return INGAME;
	}

	if (context->isKeyTextInput(KEY_SCAPE)) {
		context->getEngine().unhideCursor();
		return INGAME;
	}

	if (context->getComponents().getComponents<MenuOption>()[0].option == 2 && (context->isKeyTextInput(KEY_SPACE) || context->isKeyTextInput(KEY_INTRO))) {
        auto & engine = context->getEngine();

        engine.clear(Color(BLACK), true);
        engine.display();

        context->createIntro(true);

        return INIT;
    }

	return PAUSE;
}

StateEnum State::initNextState(const Context & context) {

	if (context->getVideoIndex() > 0 && (context->isKeyTextInput(KEY_SPACE) || context->isKeyTextInput(KEY_INTRO))) {

		auto & engine = context->getEngine();

		switch (context->getComponents().getComponents<MenuOption>()[0].option) {

			case 0 :

				engine.clear(Color(BLACK), true);

				//loadScreen->render();

				engine.display();

				context->createBackgroundMusic(); //Musica de fondo In-Game

				context->createCinematica();

				return STARTING;

			case 1 :

				engine.clear(Color(BLACK), true);

				engine.display();

				context->createTutorial();

				return TUTORIAL;

            //PONER AQUI QUE AL PULSAR EXIT TE SAQUE DEL JUEGO

			case 3 :

				engine.clear(Color(BLACK), true);

				engine.display();

				engine.close();

			default :

				break;
		}

	}

	return INIT;
}

StateEnum State::tutorialNextState(const Context & context) {

	if (context->getVideoIndex() > 0) {

		auto & engine = context->getEngine();

		engine.clear(Color(BLACK), true);

		engine.display();

        context->createIntro(true);

		return INIT;
	}

	return TUTORIAL;
}

StateEnum State::endingNextState(const Context & context) {
	if (context->getVideoIndex() > 0) {

		auto & engine = context->getEngine();

		engine.clear(Color(BLACK), true);
		engine.display();

		context->createIntro(true);

		return INIT;
	}

	return ENDING;
}

StateEnum State::startingNextState(const Context & context) {
    if (context->getVideoIndex() > 0) {
        auto * loadScreen = context->getEngine().loadTexture("../resources/menu/load_screen/pantalla_carga.png");
        auto & engine = context->getEngine();

        engine.clear(Color(BLACK), true);

        loadScreen->render();

        engine.display();

        context->createLevel();

        context->getEngine().unhideCursor();

        return INGAME;
    }

    return STARTING;
}

void State::resetClock() {
	clock.reset();
}


