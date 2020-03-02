#pragma once

#include <chrono>
#include <string>

struct Duration {
	long double ratio {       1 };
	std::string unit  {" ns\n"};
};

namespace duration {
	inline Duration nanoseconds 	{1.000000000, " ns\n"};
	inline Duration microseconds 	{0.001000000, " µs\n"};
	inline Duration milliseconds 	{0.000001000, " ms\n"};
	inline Duration seconds			{0.000000001, "  s\n"};
}

struct Timer {

	Timer();

	void start ();
	void reset ();

	double getElapsed(const Duration& = duration::milliseconds);

	void getInfo (const Duration& = duration::milliseconds);

private:

	void processInfo();

	std::chrono::time_point<std::chrono::high_resolution_clock> before;

		 double 	elapsed	{ 	0 };
		 double 	maxTime { std::numeric_limits<double>::min() };
		 double 	minTime { std::numeric_limits<double>::max() };
		 double 	average { 0.0 };
	long double 	accum 	{   0 };
	unsigned long 	nTimes 	{   0 };
};