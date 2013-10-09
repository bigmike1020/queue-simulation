#include "packet.h"

#include <deque>
#include <random>

static std::mt19937 rand_engine;

Packet::Packet(TimeDiff server, TimeDiff client)
	: server(server), client(client)
{
	assert(server > 0 && "Time spent in queue must be > 0");
	assert(client > 0 && "Time spent in queue must be > 0");
}

PacketFactory::PacketFactory(const Options& opts)
	: server_dist(1.0f / opts.meanTimeServerQueue)
	, client_dist(1.0f / opts.meanTimeClientQueue)
{}

Packet PacketFactory::operator()()
{
	auto server = server_dist(rand_engine);
	auto client = client_dist(rand_engine);
	
	Packet p(server, client);
	allPackets.push_back(p);

	return p;
}

std::string PacketFactory::stats() const
{
	return "";
}
