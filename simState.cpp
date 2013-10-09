#include "simState.h"

#include <algorithm>
#include <sstream>

#include "functions.h"

SimState::SimState(const Options& opts)
	: mcl(0.0f)
	, event(EventType::TRANSFER)
	, speed(TransferSpeed::HIGH)
	, nextTransfer(TIME_INFINITY)
	, nextConsume(TIME_INFINITY)
	, flag(Flag::NOPRINT)
{
	PacketFactory factory(opts);

	for (int i = 0; i <= opts.packets; ++i)
	{
		untransferred.emplace_back(factory());
	}
	
	nextTransfer = TIME_BEGIN;
}
