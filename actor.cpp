#include "actor.h"

#include <algorithm>

#include "functions.h"
#include "simState.h"

using namespace Actors;

using std::shared_ptr;
using std::move;

using util::begin;
using util::end;

static TimeDiff time(TransferSpeed speed, const Options& opts)
{
	if (speed == TransferSpeed::HIGH)
		return opts.arrHigh;
	else if (speed == TransferSpeed::LOW)
		return opts.arrLow;
	assert(false && "Unrecognized TransferSpeed");
	return 1.0f;
}

class UntransferredActor : public Actor
{
	PacketFactory factory;
	Options opts;

public:

	UntransferredActor(const Options& opts) : factory(opts), opts(opts)
	{}

	virtual Time getTime(const SimState& state) const OVERRIDE
	{
		return state.nextTransfer;
	}

	virtual void act(SimState& state) OVERRIDE
	{
		assert(state.untransferredCount > 0
			&& "untransferred queue shouldn't be empty");
		assert(state.nextTransfer != TIME_INFINITY 
			&& "infinite time unexpected");

		auto now = state.nextTransfer;
		state.mcl = now;
		state.event = EventType::TRANSFER;

		// Get the next untransfered packet
		auto packet = factory(state.speed, opts);
		--state.untransferredCount;

		// Record next time for transfer
		if (state.untransferredCount == 0)
		{
			state.nextTransfer = TIME_INFINITY;
		}
		else
		{
			state.nextTransfer += time(state.speed, opts);
		}

		// Move untransfered packet into queue
		auto delay = packet.getServerTime();
		auto exit = now + delay;
		state.serverQueue.emplace_back(exit, delay, packet);
		std::sort(begin(state.serverQueue), end(state.serverQueue));

		state.flag = Flag::NOPRINT;
	}

};

std::shared_ptr<Actor>
Actors::MakeUntransferredActor(const Options& opts)
{
	return std::make_shared<UntransferredActor>(opts);
}

class ServerQueueActor : public Actor
{
	Options opts;

public:

	ServerQueueActor(const Options& opts) : opts(opts)
	{}

	virtual Time getTime(const SimState& state) const OVERRIDE
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
		auto packet = item.getPacket();
		state.clientQueue.emplace_back(packet);
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
			assert(state.clientQueue.size() == 1);
			state.nextConsume = now + packet.getClientTime();
		}
		else assert(state.clientQueue.size() > 1);
	}

};


std::shared_ptr<Actor>
Actors::MakeServerQueueActor(const Options& opts)
{
	return std::make_shared<ServerQueueActor>(opts);
}


class ClientQueueActor : public Actor
{
	Options opts;

public:

	ClientQueueActor(const Options& opts) : opts(opts)
	{}

	virtual Time getTime(const SimState& state) const OVERRIDE
	{
		return state.nextConsume;
	}

	virtual void act(SimState& state) OVERRIDE
	{
		assert(!state.clientQueue.empty() && "Client queue cannot be empty");
		assert(state.nextConsume != TIME_INFINITY 
			&& "Time cannot be infinity");

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
			auto& packet = state.clientQueue.front();
			state.nextConsume += packet.getClientTime();
		}

		state.flag = Flag::NOPRINT;
	}

};

std::shared_ptr<Actor>
Actors::MakeClientQueueActor(const Options& opts)
{
	return std::make_shared<ClientQueueActor>(opts);
}
