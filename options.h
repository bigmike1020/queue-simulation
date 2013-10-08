#pragma once
#include "defines.h"

struct Options
{
	int packets;
	TimeDiffType arrHigh;
	TimeDiffType arrLow;
	TimeDiffType meanTimeServerQueue;
	TimeDiffType meanTimeClientQueue;
	int thresholdLow;
	int thresholdHigh;
	unsigned int seed;

	Options();
};

Options readOptions(int argc, char *argv[]);

