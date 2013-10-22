#pragma once

#include <cassert>

#include "defines.h"
#include "functions.h"
#include "options.h"

enum class TransferSpeed { HIGH, LOW };

class PacketFactory;

class Packet
{
	// Time spent in server and client queues
	TimeDiff transfer;
	TimeDiff server;
	TimeDiff client;
	
	Packet(TimeDiff transfer, TimeDiff server, TimeDiff client)
		: transfer(transfer), server(server), client(client)
	{
		assert(transfer > 0 && "Time spent in queue must be > 0");
		assert(server > 0 && "Time spent in queue must be > 0");
		assert(client > 0 && "Time spent in queue must be > 0");
	}

public:
	
	TimeDiff getTransferTime() const {return transfer;}
	TimeDiff getServerTime() const {return server;}
	TimeDiff getClientTime() const {return client;}
	
	friend class PacketFactory;
};

class PacketFactory
{
	util::rand_engine rand_engine;
	util::exponential_distribution<TimeDiff> transfer_dist_fast;
	util::exponential_distribution<TimeDiff> transfer_dist_slow;
	util::exponential_distribution<TimeDiff> server_dist;
	util::exponential_distribution<TimeDiff> client_dist;
public:
	
	PacketFactory(const Options& opts);

	Packet operator()(TransferSpeed speed, const Options& opts);

};
