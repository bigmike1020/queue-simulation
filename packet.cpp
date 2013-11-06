#include "packet.h"

#include <cassert>

#include "defines.h"

Packet::Packet(TimeDiff transfer, TimeDiff server, TimeDiff client)
	: transfer(transfer), server(server), client(client), 
	start(TIME_ZERO), finish(TIME_ZERO)
{
	assert(transfer > 0 && "Time spent in queue must be > 0");
	assert(server > 0 && "Time spent in queue must be > 0");
	assert(client > 0 && "Time spent in queue must be > 0");
}

PacketFactory::PacketFactory(const Options& opts)
	: rand_engine(opts.seed)
	, transfer_dist_fast(1.0f / opts.arrHigh)
	, transfer_dist_slow(1.0f / opts.arrLow)
	, server_dist(1.0f / opts.meanTimeServerQueue)
	, client_dist(1.0f / opts.meanTimeClientQueue)
{
	assert(opts.arrHigh <= opts.arrLow 
		&& "Fast arrival rate must be less than slow arrival rate");
}

Packet PacketFactory::operator()(TransferSpeed speed, const Options& opts)
{
	auto transfer = (speed == TransferSpeed::HIGH) 
		? transfer_dist_fast(rand_engine) : transfer_dist_slow(rand_engine);
	auto server = server_dist(rand_engine);
	auto client = client_dist(rand_engine);

	Packet p(transfer, server, client);
	return p;
}
