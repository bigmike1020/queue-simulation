#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

#include "actor.h"
#include "functions.h"
#include "options.h"
#include "simState.h"
#include "types.h"

using std::begin;
using std::end;

using util::write;
using Actors::Actor;

std::vector<std::shared_ptr<Actor> > MakeActorList(const Options& opts)
{
	std::shared_ptr<Actor> 
		untransferedActor = Actors::MakeUntransferredActor(opts),
		serverQueueActor = Actors::MakeServerQueueActor(opts),
		clientQueueActor = Actors::MakeClientQueueActor(opts);

	std::vector<std::shared_ptr<Actor> > actors = {
		untransferedActor, serverQueueActor, clientQueueActor
	};

	return actors;
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

	SimState state(opts.packets);
	Logger() << state.toString();

	Comparator<SimState*> comp(&state);
	auto actors = MakeActorList(opts);

	std::sort(begin(actors), end(actors), comp);
	while (!actors.front()->finished(state))
	{
		auto& nextActor = actors.front();
		nextActor->act(state);

		Logger() << state.toString();

		std::sort(begin(actors), end(actors), comp);
		nextActor = actors.front();
	}

	return 0;
}
