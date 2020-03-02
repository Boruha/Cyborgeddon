#include <Engine/util/Timer.hpp>

#include <iostream>

Timer::Timer() {
	start();
}

void Timer::start() {
	before = std::chrono::high_resolution_clock::now();
}

void Timer::getInfo(const Duration& d) {

	reset();

	std::cout << "Elapsed: " 	<< elapsed * d.ratio << d.unit;
	std::cout << "Min: " 		<< minTime * d.ratio << d.unit;
	std::cout << "Max: " 		<< maxTime * d.ratio << d.unit;
	std::cout << "Avg: " 		<< average * d.ratio << d.unit;
	std::cout << "Ticks: "   	<< nTimes << "\n\n";
}

void Timer::reset() {
	auto now = std::chrono::high_resolution_clock::now();

	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - before).count();

	processInfo();

	before = now;
}

double Timer::getElapsed(const Duration& d) {
	elapsed = (std::chrono::high_resolution_clock::now() - before).count();

	processInfo();

	return d.ratio * elapsed;
}

void Timer::processInfo() {
	maxTime = std::max(maxTime, elapsed);
	minTime = std::min(minTime, elapsed);

	accum += elapsed;

	average = accum / ++nTimes;
}
