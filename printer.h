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

	size_t size() const { return history.size(); }
	
	void emplace_back(const SimState& state);
	void boringPrint();
	void prettyPrint();
	void print(const Options& opts);
};

void printPacketStats(SimState& state, Options opts);
