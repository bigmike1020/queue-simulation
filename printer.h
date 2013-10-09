#pragma once

#include <deque>

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
	void print();
};
