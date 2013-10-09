#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

#include "actor.h"
#include "functions.h"
#include "simState.h"

using util::begin;
using util::end;
using util::Logger;

using Actors::Actor;

std::vector<std::shared_ptr<Actor> > MakeActorList(const Options& opts)
{
	return {
		Actors::MakeUntransferredActor(opts),
		Actors::MakeServerQueueActor(opts), 
		Actors::MakeClientQueueActor(opts)
	};
}

template<typename StatePtr>
class Comparator
{
	StatePtr state;

public:

	Comparator(StatePtr state) : state(state)
	{}

	template<typename Ptr>
	bool operator()(const Ptr& lhs, const Ptr& rhs) const
	{
		assert(state && "State null in Actor comparator");
		return (lhs->getTime(*state) < rhs->getTime(*state));
	}
};


int main(int argc, char *argv[])
{
	Options opts = readOptions(argc, argv);

	SimState state(opts);
	Logger() << state.toString();

	Comparator<SimState*> comp(&state);
	auto actors = MakeActorList(opts);

	std::deque<SimState> stateHistory = {state};
	
	std::sort(begin(actors), end(actors), comp);
	std::shared_ptr<Actor> nextActor;
	while (!(nextActor = actors.front())->finished(state))
	{
		auto now = nextActor->getTime(state);
		assert(0 < now);
		assert(now < TIME_INFINITY);
		
		nextActor->act(state);
		stateHistory.emplace_back(state);

		Logger() << state.toString();

		std::sort(begin(actors), end(actors), comp);
	}

	return 0;
}
