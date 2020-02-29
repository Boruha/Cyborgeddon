#include <sys/RenderSystem.hpp>
#include <Engine/util/Math.hpp>
#include <glm/gtx/string_cast.hpp>
#include <chrono>

unsigned long maxTime = 0;
unsigned long minTime = INFINITY;
unsigned long nTimes = 0;
unsigned long accum = 0;
long double average = 0.0;

void RenderSystem::update(const Context& context, const float deltaTime) {

	auto& player = context->getPlayer();
	auto& camera = context->getCamera();

	camera.getComponent<Render>()->node->setTarget(player.getComponent<Physics>()->position);
/*
	std::chrono::time_point before = std::chrono::high_resolution_clock::now();
*/
	for (auto & cmp : context->getComponents().get<Render>()) {
		if (cmp && cmp.needUpdate) {    // sin el flag el bucle dura unos 14-15ms de media, con el flag dura 2ms de media
			cmp.node->setPosition(*cmp.pos);
			cmp.node->setRotation(*cmp.rot);
			cmp.node->setScale(*cmp.sca);
		}
	}
/*
	unsigned long elapsed = (std::chrono::high_resolution_clock::now() - before).count();

	maxTime = std::max(maxTime, elapsed);
	minTime = std::min(minTime, elapsed);
	accum += elapsed;
	average = static_cast<long double>(accum) / static_cast<long double>(++nTimes);

	std::cout << "Min: " << minTime << "ns\n";
	std::cout << "Max: " << maxTime << "ns\n";
	std::cout << "Avg: " << average << "ns\n";
	std::cout << "N: "   << nTimes  << "ticks\n";
*/
	engine->clear(Color(background));
	engine->draw();
	engine->display();
}