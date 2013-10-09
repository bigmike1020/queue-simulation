#pragma once
#include "defines.h"

struct Options
{
	int packets;
	TimeDiff arrHigh;
	TimeDiff arrLow;
	TimeDiff meanTimeServerQueue;
	TimeDiff meanTimeClientQueue;
	int thresholdLow;
	int thresholdHigh;
	unsigned int seed;

	Options();
};

Options readOptions(int argc, char *argv[]);

