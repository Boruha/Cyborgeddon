#include <Engine/util/Timer.hpp>

#include <iostream>
#include <utility>

Timer::Timer(Duration  d) : dur(std::move(d)) {
	start();
}

void Timer::start() {
	before = std::chrono::high_resolution_clock::now();
}

void Timer::getInfo() {

	reset();

	std::cout << "Elapsed: " 	<< elapsed * dur.ratio << dur.unit;
	std::cout << "Min: " 		<< minTime * dur.ratio << dur.unit;
	std::cout << "Max: " 		<< maxTime * dur.ratio << dur.unit;
	std::cout << "Avg: " 		<< average * dur.ratio << dur.unit;
	std::cout << "Ticks: "   	<< nTimes << "\n\n";
}

void Timer::reset() {
	auto now = std::chrono::high_resolution_clock::now();

	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - before).count();

	processInfo();

	before = now;
}

double Timer::getElapsedAndReset() {
	return getElapsed(true);
}

void Timer::processInfo() {
	maxTime = std::max(maxTime, elapsed);
	minTime = std::min(minTime, elapsed);

	accum += elapsed;

	average = accum / double(++nTimes);
}

double Timer::getElapsedNoReset() {
	return getElapsed(false);
}

double Timer::getElapsed(const bool should_reset) {
	elapsed = (std::chrono::high_resolution_clock::now() - before).count();

	processInfo();

	if (should_reset)
		reset();

	return dur.ratio * elapsed;
}
