#include "options.h"

#include <cassert>
#include <cstdlib>
#include <getopt.h>
#include <unistd.h>
#include <iostream>

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

void printUsage(int argc, char *argv[])
{
	assert(argc > 0);

	std::cerr <<
"Usage: " << argv[0] << " [options]\n"
"Options:\n"
"  -p\n"
"  --packets     Number of packets that departed from the client queue. \n"
"                (default N=30) \n"
"  --arrHigh     Time to transmit a packet from the server at high rate. \n"
"                (default D_H=1) \n"
"  --arrLow      Time to transmit a packet from the server at low rate. \n"
"                (default D_L=2) \n"
"  --serverMean  Mean service time in the infinite server queue. \n"
"                (default 1/\u03BC_d=10) \n"
"  --clientMean  Mean service time in the client queue. \n"
"                (default 1/\u03BC_q=1.5) \n"
"  --thresLow    Low threshold level in the server queue. \n"
"                (default T_L=3) \n"
"  --thresHigh   High threshold level in the server queue. \n"
"                (default T_H=3) \n"
"  --seed        Pseudorandom number generator seed. (default R=45647) \n"
"\n"
"Written by Mike Senn <mpsenn@ncsu.edu>\n";
}

Options readOptions(int argc, char *argv[])
{
	Options opts{};

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
			printUsage(argc, argv);
			exit(1);
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
