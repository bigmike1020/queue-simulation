#include "printer.h"

#include <cstdlib>
#include <cstdio>

#define FP_FORMAT "%#4.3f"
#define INT_FORMAT "%d"

using util::begin;
using util::end;

const char* str(TransferSpeed speed)
{
	return (speed == TransferSpeed::HIGH) ? "H" : "L";
}

const char* str(Flag flag)
{
	if(flag == Flag::UP)
		return "up";
	else if (flag == Flag::DOWN)
		return "down";
	return "";
}

std::string str(EventType event)
{
	std::ostringstream ss;
	ss << int(event);
	return ss.str();
}

void StateHistory::emplace_back(const SimState& state)
{
	history.emplace_back(state);
	/*
	int size = snprintf(nullptr, 0, FP_FORMAT, state.mcl);
	mclSize = std::max(mclSize, size);

	size = snprintf(nullptr, 0, FP_FORMAT, state.nextTransfer);
	clpSize = std::max(clpSize, size);

	size = snprintf(nullptr, 0, FP_FORMAT, state.nextConsume);
	clcqSize = std::max(clpcqSize, size);

	size = snprintf(nullptr, 0, FP_FORMAT, state.clientQueue.size());
	clientQueueSize = std::max(clientQueueSize, size);
	*/
}

void StateHistory::print()
{
	printf( 
"  Actions |  Server   |      Infinite Server Queue       |  Client Queue  \n"
" MCL|Event|Trans.|CL-p|       CL-isq per waiting         |CL-cq| Νο. |flag\n"
"    | No  |Speed |    |         packet (delay)           |     |queue|    \n"
	);
	
	for(auto it = begin(history); it != end(history); ++it)
	{
		printf("%4f|%5d|%6s|%4f|", it->mcl, int(it->event), str(it->speed),
			it->nextTransfer);

		auto& serverQueue = it->serverQueue;
		for(auto it2 = begin(serverQueue); it2 != end(serverQueue); ++it2)
		{
			printf("%4f(%4f) ", it2->getExitTime(), it2->getDelay());
		}
		
		if(it->nextConsume == TIME_INFINITY)
		{
			printf("|  -  |    0|");
		}
		else
		{
			printf("|%5f|%5zu|", it->nextConsume, it->clientQueue.size());
		}
		
		printf("%4s\n", str(it->flag));
		
	}
}
