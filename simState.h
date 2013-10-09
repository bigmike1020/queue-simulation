#pragma once

#include <cassert>
#include <memory>
#include <deque>
#include <string>

#include "options.h"
#include "packet.h"
#include "defines.h"

enum class TransferSpeed { HIGH, LOW };
enum class EventType { TRANSFER = 1, SERVER = 2, CLIENT = 3 };
enum class Flag { UP, DOWN, NOPRINT };

class Packet;

class ServerQueueItem
{
	Time exit;
	TimeDiff delay;
	Packet packet;

public:
	ServerQueueItem(Time exitTime, TimeDiff delay, Packet& packet)
		: exit(exitTime), delay(delay), packet(packet)
	{}

	Time getExitTime() const { return exit; }
	Time getDelay() const { return delay; }

	const Packet& getPacket() const { return packet; }

	bool operator<(const ServerQueueItem& that) const
	{
		return (exit < that.exit);
	}
};

struct SimState
{
	// The time and event type of the last action
	Time mcl;
	EventType event;

	TransferSpeed speed;
	Time nextTransfer;
	std::deque<Packet> untransferred;

	std::deque<ServerQueueItem> serverQueue;

	Time nextConsume;
	std::deque<Packet> clientQueue;

	// Set to UP or DOWN when client transfers happen
	Flag flag;

	SimState(const Options& opts);
};

