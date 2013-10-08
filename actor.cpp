#include "actor.h"

#include <algorithm>
#include <cmath>
#include <random>

#include "functions.h"
#include "simState.h"

using namespace Actors;

using std::shared_ptr;
using std::move;

using util::begin;
using util::end;

typedef std::mt19937 rand_engine;
typedef std::exponential_distribution<TimeDiffType> server_distribution;
typedef std::exponential_distribution<TimeDiffType> client_distribution;

inline TimeDiffType time(TransferSpeed speed, const Options& opts)
{
	if (speed == TransferSpeed::HIGH)
		return opts.arrHigh;
	else if (speed == TransferSpeed::LOW)
		return opts.arrLow;
	assert(false && "Unrecognized TransferSpeed");
	return 1.0f;
}

shared_ptr<rand_engine> randEngine;
shared_ptr<rand_engine> GetRandEngine(const Options& opts)
{
	if (randEngine) return randEngine;

	return (randEngine = std::make_shared<rand_engine>(opts.seed));
}

class UntransferredActor : public Actor
{
	shared_ptr<rand_engine> rand;
	server_distribution dist;

	Options opts;

public:

	UntransferredActor(shared_ptr<rand_engine> rand, 
		InvTimeDiffType lambda, const Options& opts)
		: rand(rand), dist(lambda), opts(opts)
	{
		assert(rand && "Rand cannot be null");
		assert(std::isfinite(lambda));
	}

	virtual TimeType getTime(const SimState& state) const OVERRIDE
	{
		return state.nextTransfer;
	}

	virtual void act(SimState& state) OVERRIDE
	{
		assert(!state.untransferred.empty() && "untransferred queue shouldn't be empty");
		assert(state.nextTransfer != TIME_INFINITY && "infinite time unexpected");

		auto now = state.nextTransfer;
		state.mcl = now;
		state.event = EventType::TRANSFER;

		// Get the next untransfered packet
		auto packet = state.untransferred.front();
		state.untransferred.pop_front();
		
		// Record next time for transfer
		if (state.untransferred.empty())
		{
			state.nextTransfer = TIME_INFINITY;
		}
		else
		{
			state.nextTransfer += time(state.speed, opts);
		}

		// Move untransfered packet into queue
		assert(rand && "Rand cannot be null");
		auto delay = dist(*rand);
		auto exit = now + delay;
		state.serverQueue.emplace_back(exit, delay, packet);
		std::sort(begin(state.serverQueue), end(state.serverQueue));

		state.flag = Flag::NOPRINT;
	}

};

std::shared_ptr<Actor>
Actors::MakeUntransferredActor(const Options& opts)
{
	assert(opts.meanTimeServerQueue != 0.0f);
	auto lambda = 1.0f / opts.meanTimeServerQueue;
	return std::make_shared<UntransferredActor>(
		GetRandEngine(opts), lambda, opts);
}

class ServerQueueActor : public Actor
{
	shared_ptr<rand_engine> rand;
	client_distribution dist;

	Options opts;

public:

	ServerQueueActor(shared_ptr<rand_engine> rand, 
		InvTimeDiffType lambda, const Options& opts)
		: rand(rand), dist(lambda), opts(opts)
	{
		assert(rand && "Rand cannot be null");
		assert(std::isfinite(lambda));
	}


	virtual TimeType getTime(const SimState& state) const OVERRIDE
	{
		if (state.serverQueue.empty())
			return TIME_INFINITY;
		
		auto min = std::min_element(begin(state.serverQueue), 
			end(state.serverQueue));
		return min->getExitTime();
	}

	virtual void act(SimState& state) OVERRIDE
	{
		assert(!state.serverQueue.empty() && "server queue cannot be empty");

		auto& item = state.serverQueue.front();
		auto now = item.getExitTime();
		state.mcl = now;
		state.event = EventType::SERVER;

		// Transfer packet from server to client
		state.clientQueue.emplace_back(item.getPacket());
		state.serverQueue.pop_front();

		// Update server transfer speed
		auto size = int(state.clientQueue.size());
		if (size <= opts.thresholdLow)
		{
			state.flag = Flag::UP;
			state.speed = TransferSpeed::HIGH;
		}
		else if (size >= opts.thresholdHigh)
		{
			state.flag = Flag::DOWN;
			state.speed = TransferSpeed::LOW;
		}
		else
		{
			state.flag = Flag::NOPRINT;
		}

		// Set time of next client consume
		if (state.nextConsume == TIME_INFINITY)
		{
			assert(rand && "Rand cannot be null");
			state.nextConsume = now + dist(*rand);
		}
	}

};


std::shared_ptr<Actor>
Actors::MakeServerQueueActor(const Options& opts)
{
	assert(opts.meanTimeClientQueue != 0.0f);
	auto lambda = 1.0f / opts.meanTimeClientQueue;
	return std::make_shared<ServerQueueActor>(
		GetRandEngine(opts), lambda, opts);
}


class ClientQueueActor : public Actor
{
	shared_ptr<rand_engine> rand;
	client_distribution dist;

	Options opts;

public:

	ClientQueueActor(shared_ptr<rand_engine> rand, 
		InvTimeDiffType lambda, const Options& opts)
		: rand(rand), dist(lambda), opts(opts)
	{
		assert(rand && "Rand cannot be null");
		assert(std::isfinite(lambda));
	}

	virtual TimeType getTime(const SimState& state) const OVERRIDE
	{
		return state.nextConsume;
	}

	virtual void act(SimState& state) OVERRIDE
	{
		assert(!state.clientQueue.empty() && "Client queue cannot be empty");
		assert(state.nextConsume != TIME_INFINITY && "Time cannot be infinity");

		auto now = state.nextConsume;
		state.mcl = now;
		state.event = EventType::CLIENT;

		// Consume one packet on client
		state.clientQueue.pop_front();

		// Set time for next consume
		if (state.clientQueue.empty())
		{
			state.nextConsume = TIME_INFINITY;
		}
		else
		{
			assert(rand && "rand cannot be null");
			state.nextConsume += dist(*rand);
		}

		state.flag = Flag::NOPRINT;
	}

};



std::shared_ptr<Actor>
Actors::MakeClientQueueActor(const Options& opts)
{
	assert(opts.meanTimeClientQueue != 0.0f);
	auto lambda = 1.0f / opts.meanTimeClientQueue;
	return std::make_shared<ClientQueueActor>(
		GetRandEngine(opts), lambda, opts);
}
