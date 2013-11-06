#pragma once

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

	Packet(TimeDiff transfer, TimeDiff server, TimeDiff client);

public:
	
	TimeDiff getTransferTime() const {return transfer;}
	TimeDiff getServerTime() const {return server;}
	TimeDiff getClientTime() const {return client;}

	Time start;
	Time finish;
	
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
