#pragma once

#include <deque>

#include "options.h"
#include "simState.h"

class StateHistory
{
	Options opts;

	std::deque<SimState> history;

public:

	StateHistory(const Options& opts) : opts(opts)
	{}

	void emplace_back(const SimState& state);
	void boringPrint();
	void prettyPrint();
	void print(const Options& opts);

	void statPrint();
};
