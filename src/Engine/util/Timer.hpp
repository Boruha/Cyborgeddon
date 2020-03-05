#pragma once

#include <chrono>
#include <string>

struct Duration {
	constexpr Duration(const double rat, const std::string_view u)
		: ratio(rat), unit(u) { }

	const double ratio          {       1 };
	const std::string_view unit {" ns\n"};
};

namespace duration {
	constexpr Duration nanoseconds 	    {1.000000000, " ns\n"};
	constexpr Duration microseconds 	{0.001000000, " µs\n"};
	constexpr Duration milliseconds 	{0.000001000, " ms\n"};
	constexpr Duration seconds			{0.000000001, "  s\n"};
}

struct Timer {

	explicit Timer(Duration = duration::milliseconds);

	void start ();
	void reset ();

	double getElapsedAndReset();
	double getElapsedNoReset();

	void getInfo();

private:

	void processInfo();

	double getElapsed(bool);

	std::chrono::time_point<std::chrono::high_resolution_clock> before;

    double 	elapsed	{ 	0 };
    double 	maxTime { std::numeric_limits<double>::min() };
    double 	minTime { std::numeric_limits<double>::max() };
    double 	average { 0.0 };
    double 	accum 	{   0 };

    unsigned long 	nTimes 	{   0 };

    const Duration  dur { duration::milliseconds };
};