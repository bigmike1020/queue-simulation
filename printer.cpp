#include "printer.h"

#include <cstdlib>
#include <cstdio>

#include <iterator>
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

void printPacketStats(const SimState& state, const Options& opts)
{
	printf("I have %lu packets.\n", state.finishedPackets.size());
}
