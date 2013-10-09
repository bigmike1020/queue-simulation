#pragma once

#include <cassert>
#include <deque>
#include <random>
#include <string>

#include "defines.h"
#include "functions.h"
#include "options.h"

class PacketFactory;

class Packet
{
	// Time spent in server and client queues
	TimeDiff server;
	TimeDiff client;
	
	Packet(TimeDiff server, TimeDiff client);
public:
	
	TimeDiff getServerTime() const {return server;}
	TimeDiff getClientTime() const {return client;}
	
	friend class PacketFactory;
};

class PacketFactory
{
	std::mt19937 rand_engine;
	util::exponential_distribution<TimeDiff> server_dist;
	util::exponential_distribution<TimeDiff> client_dist;
public:
	
	PacketFactory(const Options& opts);
	Packet operator()();
};
