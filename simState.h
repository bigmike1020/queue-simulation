#pragma once

#include <cassert>
#include <memory>
#include <deque>
#include <string>

#include "packet.h"
#include "defines.h"

enum class TransferSpeed { HIGH, LOW };
enum class EventType { TRANSFER = 1, SERVER = 2, CLIENT = 3 };
enum class Flag { UP, DOWN, NOPRINT };

class Packet;

class ServerQueueItem
{
	TimeType exit;
	TimeDiffType delay;
	std::unique_ptr<Packet> packet;

public:
	ServerQueueItem(TimeType exitTime, TimeDiffType delay, std::unique_ptr<Packet> packetPtr)
		: exit(exitTime), delay(delay), packet(std::move(packetPtr))
	{
		assert(packet && "packet null in ServerQueueItem::ServerQueueItem");
	}

	ServerQueueItem(ServerQueueItem&& rhs);
	ServerQueueItem& operator=(ServerQueueItem&& rhs);

	TimeType getExitTime() const { return exit; }

	std::unique_ptr<Packet> detach() { return std::move(packet); }

	bool operator<(const ServerQueueItem& that) const
	{
		return (exit < that.exit);
	}

	std::string toString() const;

};

struct SimState
{
	// The time and event type of the last action
	TimeType mcl;
	EventType event;

	TransferSpeed speed;
	TimeType nextTransfer;
	std::deque<std::unique_ptr<Packet> > untransferred;

	std::deque<ServerQueueItem> serverQueue;

	TimeType nextConsume;
	std::deque<std::unique_ptr<Packet> > clientQueue;

	// Set to UP or DOWN when client transfers happen
	Flag flag;

	SimState(int numPackets);

	std::string toString() const;
};

