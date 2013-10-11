#pragma once
#include "defines.h"

enum class PrintFormat { PRETTY, BORING };

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

	PrintFormat format;

	Options();
};

Options readOptions(int argc, char *argv[]);

