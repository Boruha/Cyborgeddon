#include <util/State.hpp>
#include <src/Engine/util/Math.hpp>
#include <src/Engine/EngineInterface/SceneInterface/IVideo.hpp>
#include <src/Engine/EngineInterface/SceneInterface/ITexture.hpp>


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
			break;
		case TUTORIAL :
			next_state = &State::tutorialNextState;
			break;
		case INGAME :
			next_state = &State::ingameNextState;
//			context->createLevel();
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
		context->createIntro(true);
		return INIT;
	}

	if (context->checkDefeat()) {
		context->createLevel();
		return INGAME;
	}

	if (context->isKeyPressed(GLFW_KEY_ESCAPE))
		return PAUSE;

	return INGAME;
}

StateEnum State::pauseNextState(const Context & context) {

	if (context->getComponents().getComponents<MenuOption>()[0].option == 0 && (context->isKeyTextInput(KEY_SPACE) || context->isKeyTextInput(KEY_INTRO))) {
        return INGAME;
    }
    //TODO:: ARREGLAR PARA QUE AL VOLVER AL JUEGO DESDE EL MENU PRINCIPAL NO VAYAN LAS ANIMACIONES A TODA LECHE
	/*if (context->getComponents().getComponents<MenuOption>()[0].option == 2 && (context->isKeyTextInput(KEY_SPACE) || context->isKeyTextInput(KEY_INTRO))) {
        auto & engine = context->getEngine();
        engine.clear(Color(BLACK), true);

        context->createIntro(true);

        return INIT;
    }*/
        return PAUSE;
	    //if(context->isKeyTextInput(KEY_SCAPE))
	   // return INGAME;
}

StateEnum State::initNextState(const Context & context) {

	if (context->getVideoIndex() > 0 && (context->isKeyTextInput(KEY_SPACE) || context->isKeyTextInput(KEY_INTRO))) {

		auto * loadScreen = context->getEngine().loadTexture("../resources/menu/load_screen/pantalla_carga.png");
		auto & engine = context->getEngine();

		switch (context->getComponents().getComponents<MenuOption>()[0].option) {

			case 0 :

				engine.clear(Color(BLACK), true);

				loadScreen->render();

				engine.display();

				context->createLevel();

				return INGAME;

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

		auto * loadScreen = context->getEngine().loadTexture("../resources/menu/load_screen/pantalla_carga.png");
		auto & engine = context->getEngine();

		engine.clear(Color(BLACK), true);

        context->createIntro(true);

		//loadScreen->render();

		//engine.display(); <-Vamos de vuelta al menu principal, la pantalla de carga no duraria na

		//context->createLevel();

		return INIT;
	}

	return TUTORIAL;
}


