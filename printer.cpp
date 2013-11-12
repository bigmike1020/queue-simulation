#include "printer.h"

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iterator>
#include <numeric>
#include <vector>

using util::begin;
using util::end;

const char ITEM_FORMAT[] = "%6.2f(%6.2f) ";

const char* str(TransferSpeed speed)
{
	return (speed == TransferSpeed::HIGH) ? "H" : "L";
}

const char* str(Flag flag)
{
	if(flag == Flag::UP)
		return " up ";
	else if (flag == Flag::DOWN)
		return "down";
	return "  - ";
}

void StateHistory::emplace_back(const SimState& state)
{
	history.emplace_back(state);
}

void printServerQueue(std::deque<ServerQueueItem>& serverQueue)
{
	char serverStr[33] = {'\0'};
	if(!serverQueue.empty())
	{
		auto packet = serverQueue.front();
		serverQueue.pop_front();

		int chars = snprintf(serverStr, 33, ITEM_FORMAT, 
			packet.getExitTime(), packet.getDelay());

		if(!serverQueue.empty())
		{
			packet = serverQueue.front();
			serverQueue.pop_front();
			snprintf(serverStr+chars, 33-chars, ITEM_FORMAT, 
				packet.getExitTime(), packet.getDelay());
		}
	}
	
	printf("|%33s|", serverStr);
}

void StateHistory::print(const Options& opts)
{
	if(opts.format == PrintFormat::PRETTY)
	{
		prettyPrint();
	}
	else
	{
		boringPrint();
	}
}

void StateHistory::prettyPrint()
{
	printf( 
"    Actions  |   Server    |      Infinite Server Queue      |"
"  Client Queue   \n"
"   MCL |Event|Trans.| CL-p |       CL-isq per waiting        |"
"CL-cq | Νο. |flag\n"
"       | No  |Speed |      |         packet (delay)          |"
"      |queue|    \n"
	);
	
	for(auto it = begin(history); it != end(history); ++it)
	{
		printf(
"------- ----- ------ ---- ----------------------------------- "
"------ ----- ----\n"
		);

		if(it->mcl == TIME_INFINITY)
		{
			printf("    -  |");
		}
		else
		{
			printf(" %6.2f|", it->mcl);
		}

		printf("%3d  |%4s  |", int(it->event), str(it->speed));

		if(it->nextTransfer == TIME_INFINITY)
		{
			printf("   -  ");
		}
		else
		{
			printf("%6.2f", it->nextTransfer);
		}

		std::deque<ServerQueueItem> serverQueue = it->serverQueue;
		printServerQueue(serverQueue);

		if(it->nextConsume == TIME_INFINITY)
		{
			printf("   -  |   0 |");
		}
		else
		{
			printf("%6.2f|%4zu |", it->nextConsume, it->clientQueue.size());
		}

		printf("%4s\n", str(it->flag));

		while(!serverQueue.empty())
		{
			printf("                           ");
			printServerQueue(serverQueue);
			printf("\n");
		}
	}
}


void StateHistory::boringPrint()
{
	printf( 
"MCL, Event No, Trans. speed, CL-p, "
"CL-cq, Νο, flag, CL-isq per waiting packet (delay)\n"
	);
	
	for(auto it = begin(history); it != end(history); ++it)
	{
		if(it->mcl == TIME_INFINITY)
		{
			printf("      -, ");
		}
		else
		{
			printf("%7.3f, ", it->mcl);
		}

		printf("%1d, %1s, ", int(it->event), str(it->speed));

		if(it->nextTransfer == TIME_INFINITY)
		{
			printf("      -,");
		}
		else
		{
			printf("%7.3f,", it->nextTransfer);
		}

		if(it->nextConsume == TIME_INFINITY)
		{
			printf("       -,  0,");
		}
		else
		{
			printf(" %7.3f, %2zu,", it->nextConsume, it->clientQueue.size());
		}

		printf("%4s, ", str(it->flag));

		auto& queue = it->serverQueue;
		for(auto it2 = begin(queue); it2 != end(queue); ++it2)
		{
			printf(" %7.3f(%7.3f)", it2->getExitTime(), it2->getDelay());
		}

		printf("\n");
	}
}

struct Bounds
{
	TimeDiff min;
	TimeDiff lower;
	TimeDiff median;
	TimeDiff higher;
	TimeDiff max;
};

TimeDiff higherSum(TimeDiff init, Bounds& b)
{
	return init + b.higher;
}

TimeDiff higherProduct(Bounds& b1, Bounds& b2)
{
	return b1.higher * b2.higher;
}

struct SubtractAndSquare
{
	TimeDiff mean;
	explicit SubtractAndSquare(TimeDiff mean) : mean(mean) {}
	TimeDiff operator()(TimeDiff init, Bounds& b)
	{
		TimeDiff diff = (mean - b.higher);
		return init + (diff * diff);
	}
};

void printStats(std::deque<Bounds>& intervals, Options& opts)
{
	auto& b = intervals;
	
	TimeDiff sum = std::accumulate(begin(b), end(b), 0.0, higherSum);
	TimeDiff mean = sum / b.size();

	TimeDiff sq_diff = std::accumulate(begin(b), end(b), 0.0, 
		SubtractAndSquare{mean});
	TimeDiff stdev = std::sqrt(sq_diff / (b.size() - 1));
	
	printf("Mean=%lf, Std Deviation=%lf\n", mean, stdev);
	
	auto range = 1.96 * stdev / std::sqrt(intervals.size());
	auto lowerConf = mean - range;
	auto higherConf = mean + range;
	
	if(range < (0.10 * mean))
	{
		printf("Error %lf is small; batch size sufficient.\n", range);
	}
	else
	{
		printf("Error %lf is large; increase batch size.\n", range);
	}
	
	printf("95.0%% confidence interval (%lf, %lf)\n", lowerConf, higherConf);
	
	for(auto it = begin(b); it != end(b); ++it)
	{
		printf("%lf\n", it->higher);
	}

}

void printPacketStats(SimState& state, Options opts)
{
	auto& packets = state.finishedPackets;
	
	// First marks the beginning of the current batch
	printf("Total %lu packets. Skipping first %d packets.\n", 
		packets.size(), opts.warmupSize);
	
	auto first = begin(packets);
	for(int i=0; i < opts.warmupSize; ++i)
	{
		++first;
		assert(first != end(packets));
	}

	std::deque<Bounds> intervals;
	std::deque<TimeDiff> endToEnds;
	
	for(auto curr = first; curr != end(packets); )
	{
		std::deque<TimeDiff> delays;
		
		for(int i=0; i < opts.batchSize; ++i)
		{
			auto delay = curr->finish - curr->start;
			delays.emplace_back(delay);
			if(endToEnds.size() < 30000) endToEnds.emplace_back(delay);
			
			++curr;
			if(curr == end(packets)) break;
		}
		
		if(int(delays.size()) != opts.batchSize) break;
		
		std::sort(begin(delays), end(delays));

		auto lowBound = int(floor(opts.batchSize*(1-opts.percentile)));
		auto highBound = int(ceil(opts.batchSize * opts.percentile));
		highBound = std::min(highBound, int(delays.size()-1));
		
		Bounds bound;
		bound.min = delays.at(0);
		bound.lower = delays.at(lowBound);
		bound.median = delays.at(delays.size()/2);
		bound.higher = delays.at(highBound);
		bound.max = delays.at(delays.size()-1);
		intervals.emplace_back(bound);

		if(curr == end(packets)) break;
	}

	auto sum = std::accumulate(begin(endToEnds), end(endToEnds), 0.0);
	printf("Mean end-to-end delay %lf.\n", sum/endToEnds.size());
	
	printf("Collected %lu batches of %d packets.\n", 
		intervals.size(), opts.batchSize);
	printf("Displaying %4.1lf%% percentiles.\n", opts.percentile * 100.0);
	printf("Simulation ended at %1.0lf time.\n", packets.rbegin()->finish);
	
	printStats(intervals, opts);
	
}
