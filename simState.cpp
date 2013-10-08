#include "simState.h"

#include <algorithm>
#include <sstream>

#include "functions.h"
#include "types.h"

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

ServerQueueItem::ServerQueueItem(ServerQueueItem&& rhs)
	: exit(rhs.exit), delay(rhs.delay), packet(move(rhs.packet))
{}

ServerQueueItem& ServerQueueItem::operator=(ServerQueueItem&& rhs)
{
	exit = rhs.exit;
	delay = rhs.delay;
	packet = move(rhs.packet);
	return *this;
}

std::string ServerQueueItem::toString() const
{
	auto ss = util::sstream();
	ss << exit << "(" << delay << ")";
	return ss.str();
}

SimState::SimState(int numPackets)
	: mcl(0.0f)
	, event(EventType::TRANSFER)
	, speed(TransferSpeed::HIGH)
	, nextTransfer(TIME_INFINITY)
	, nextConsume(TIME_INFINITY)
	, flag(Flag::NOPRINT)
{
	for (int i = 1; i <= numPackets; ++i)
	{
		untransferred.emplace_back(util::make_unique<Packet>(i));
	}

	nextTransfer = TIME_BEGIN;
}

std::string SimState::toString() const
{
	auto ss = util::sstream();

	ss << "[" << mcl << " " << str(event) << "] [" << str(speed) << " ";
	
	if (nextTransfer != TIME_INFINITY) ss << int(nextTransfer);
	else ss << "-";
		
	ss << "] [";
	
	auto ss2 = util::sstream();
	for (const auto& item : serverQueue)
	{
		ss2 << item.toString() << " ";
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

