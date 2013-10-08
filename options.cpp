#include "options.h"

#include <cassert>

// #include <getopt.h>
// #include <unistd.h>

Options::Options()
{
	packets = 30;
	arrHigh = 1.0f;
	arrLow = 2.0f;
	meanTimeServerQueue = 1.0f; // 0.1f;
	meanTimeClientQueue = float(20.0f / 3.0f);
	thresholdLow = 3;
	thresholdHigh = 3;
	seed = 2;
}

Options readOptions(int, char *[])
{
	Options opts;

	assert(opts.packets > 0);
	assert(opts.arrHigh > 0);
	assert(opts.arrLow > 0);
	assert(opts.arrHigh <= opts.arrLow);
	assert(opts.meanTimeServerQueue > 0);
	assert(opts.meanTimeClientQueue > 0);

	assert(opts.thresholdLow > 0);
	assert(opts.thresholdHigh > 0);
	assert(opts.thresholdLow <= opts.thresholdHigh);

	return opts;

	/*
	static struct option long_options[] = {
		{ "packets", required_argument, nullptr, 'p' },
		{ "arrivalHigh", required_argument, nullptr, 'h' },
		{ "arrivalLow", required_argument, nullptr, 'l' },
		{ "serverQueueMean", required_argument, nullptr, 's' },
		{ "clientQueueMean", required_argument, nullptr, 'c' },
		{ "thresholdLow", required_argument, nullptr, 't' },
		{ "thresholdHigh", required_argument, nullptr, 'u' },
		{ "verbose", no_argument, nullptr, 'v' },
		{ 0, 0, nullptr, 0 }
	};

	Options res = Options();

	while (true) {
		int option_index = 0;

		int c = getopt_long(argc, argv, "p:h:l:s:c:t:u:v",
			long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			break;
		case 'p':
			res.packets = atoi(optarg);
			assert(res.packets > 0 && "Packets argument must be > 0");
			break;
		case 'l':
			res.arrHigh = atoi(optarg);
			assert(res.arrHigh > 0 && "Arrival time arguments must be > 0");
			break;
		case 'h':
			res.arrLow = atoi(optarg);
			assert(res.arrLow > 0 && "Arrival time arguments must be > 0");
			break;
		}
	}

	return res; */
}
