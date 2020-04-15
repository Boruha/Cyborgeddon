#include <util/State.hpp>
#include <src/Engine/util/Math.hpp>
#include <src/Engine/EngineInterface/SceneInterface/IVideo.hpp>
#include <zconf.h>

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

StateEnum State::initNextState(const Context & context) {
	system(std::string(std::string("sudo pmap ") + std::string(std::to_string(getpid())) + std::string(" | tail -n 1")).c_str());

	std::unique_ptr<IVideo> videoIntro = context->getEngine().loadVideo("resources/videos/intro/1_F.mp4");

	system(std::string(std::string("sudo pmap ") + std::string(std::to_string(getpid())) + std::string(" | tail -n 1")).c_str());

	std::unique_ptr<IVideo> videoBucle = context->getEngine().loadVideo("resources/videos/intro/2_F_L.mp4");

	system(std::string(std::string("sudo pmap ") + std::string(std::to_string(getpid())) + std::string(" | tail -n 1")).c_str());

	std::cout << "Hola\n";

	double framesIntro = videoIntro->getNumFrames();
	unsigned frameCounter {0};

	system(std::string(std::string("sudo pmap ") + std::string(std::to_string(getpid())) + std::string(" | tail -n 1")).c_str());
/*
	while(frameCounter < unsigned(framesIntro)) {
		context->getEngine().run();
		videoIntro->render();
		videoIntro->nextFrame();
		context->getEngine().display();
		frameCounter++;
		system(std::string(std::string("sudo pmap ") + std::string(std::to_string(getpid())) + std::string(" | tail -n 1")).c_str());
	}
*/
	videoBucle->setLoop(true);

	while(!context->getEngine().isKeyPressed(KEY_SPACE)) {
		context->getEngine().run();
		videoBucle->render();
		videoBucle->nextFrame();
		context->getEngine().display();
		system(std::string(std::string("sudo pmap ") + std::string(std::to_string(getpid())) + std::string(" | tail -n 1")).c_str());
	}

	context->getEngine().unloadVideo("resources/videos/intro/1_F.mp4");
	context->getEngine().unloadVideo("resources/videos/intro/2_F_L.mp4");

	std::cout << "Hemos descargado el video\n";

	system(std::string(std::string("sudo pmap ") + std::string(std::to_string(getpid())) + std::string(" | tail -n 1")).c_str());

	return INGAME;
}
