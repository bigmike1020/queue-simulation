#include "options.h"

#include <cassert>
#include <cstdlib>
#include <getopt.h>
#include <unistd.h>

#include "defines.h"

Options::Options()
{
	packets = 30;
	arrHigh = 1.0f;
	arrLow = 2.0f;
	meanTimeServerQueue = 10.0f;
	meanTimeClientQueue = 1.5f;
	thresholdLow = 3;
	thresholdHigh = 3;
	seed = 45647;
}

Options readOptions(int argc, char *argv[])
{
	Options opts{};

	assert(opts.packets > 0);
	assert(opts.arrHigh > 0);
	assert(opts.arrLow > 0);
	assert(opts.arrHigh <= opts.arrLow);
	assert(opts.meanTimeServerQueue > 0);
	assert(opts.meanTimeClientQueue > 0);

	assert(opts.thresholdLow > 0);
	assert(opts.thresholdHigh > 0);
	assert(opts.thresholdLow <= opts.thresholdHigh);

	static struct option long_options[] = {
		{ "packets", required_argument, nullptr, 'p' },
		{ "arrHigh", required_argument, nullptr, 'h' },
		{ "arrLow", required_argument, nullptr, 'l' },
		{ "serverMean", required_argument, nullptr, 's' },
		{ "clientMean", required_argument, nullptr, 'c' },
		{ "thresLow", required_argument, nullptr, 't' },
		{ "thresHigh", required_argument, nullptr, 'u' },
		{ "seed", required_argument, nullptr, 'r' },
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
		case 'h':
			res.arrHigh = atof(optarg);
			assert(res.arrHigh > 0 && "Arrival time arguments must be > 0");
			break;
		case 'l':
			res.arrLow = atof(optarg);
			assert(res.arrLow > 0 && "Arrival time arguments must be > 0");
			break;
		case 's':
			res.meanTimeServerQueue = atof(optarg);
			assert(res.meanTimeServerQueue > 0 && "Queue times must be > 0");
			break;
		case 'c':
			res.meanTimeClientQueue = atof(optarg);
			assert(res.meanTimeClientQueue > 0 && "Queue times must be > 0");
			break;
		case 't':
			res.thresholdLow = atoi(optarg);
			assert(res.thresholdLow >= 0 && "Low threshold must be >= 0");
			break;
		case 'u':
			res.thresholdHigh = atoi(optarg);
			assert(res.thresholdHigh >= 0 && "High threshold must be >= 0");
			break;
		case 'r':
			res.seed = atoi(optarg);
			break;
		case '?':
		default:
			assert(false && "Unrecognized command line parameter");
			break;
		}
	}
	
	assert(opts.packets > 0);
	assert(opts.arrHigh > 0);
	assert(opts.arrLow > 0);
	assert(opts.arrHigh <= opts.arrLow);
	assert(opts.meanTimeServerQueue > 0);
	assert(opts.meanTimeClientQueue > 0);

	assert(opts.thresholdLow > 0);
	assert(opts.thresholdHigh > 0);
	assert(opts.thresholdLow <= opts.thresholdHigh);

	return res;
}
