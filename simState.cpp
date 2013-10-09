#include "simState.h"

#include <algorithm>
#include <sstream>

#include "functions.h"

std::string str(TransferSpeed speed)
{
	return (speed == TransferSpeed::HIGH) ? "H" : "L";
}

std::string str(Flag flag)
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

std::string ServerQueueItem::toString() const
{
	std::ostringstream ss;
	ss.precision(4);
	ss << std::showpoint;
	ss << exit << "(" << delay << ")";
	return ss.str();
}

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

std::string SimState::toString() const
{
	std::ostringstream ss;
	ss.precision(4);
	ss << std::showpoint;

	ss << "[" << mcl << " " << str(event) << "] [" << str(speed) << " ";
	
	if (nextTransfer != TIME_INFINITY) ss << int(nextTransfer);
	else ss << "-";
		
	ss << "] [";
	
	std::ostringstream ss2;
	ss2.precision(4);
	ss2 << std::showpoint;
	for (auto it=util::begin(serverQueue); it != util::end(serverQueue); ++it)
	{
		ss2 << it->toString() << " ";
	}

	auto str2 = ss2.str();
	static auto maxSize = str2.size();
	str2.resize(maxSize = std::max(str2.size(), maxSize));

	ss << str2 << "] [";
	
	if (nextConsume != TIME_INFINITY) ss << nextConsume;
	else ss << "-";
	
	ss << " " << clientQueue.size() << " " << str(flag) << "]";

	return ss.str();
}
