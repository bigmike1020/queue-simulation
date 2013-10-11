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
	
	Packet(TimeDiff server, TimeDiff client)
		: server(server), client(client)
	{
		assert(server > 0 && "Time spent in queue must be > 0");
		assert(client > 0 && "Time spent in queue must be > 0");
	}

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
	
	PacketFactory(const Options& opts)
		: rand_engine(opts.seed)
		, server_dist(1.0f / opts.meanTimeServerQueue)
		, client_dist(1.0f / opts.meanTimeClientQueue)
	{}

	Packet operator()()
	{
		auto server = server_dist(rand_engine);
		auto client = client_dist(rand_engine);
		
		Packet p(server, client);
		return p;
	}

};
