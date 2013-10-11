#pragma once

#include <deque>

#include "options.h"
#include "simState.h"

class StateHistory
{
	std::deque<SimState> history;

	int mclSize;
	int clpSize;
	int clcqSize;
	int clientQueueSize;

public:

	void emplace_back(const SimState& state);
	void boringPrint();
	void prettyPrint();
	void print(const Options& opts);
};
