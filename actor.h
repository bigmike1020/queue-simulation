#pragma once

#include <memory>

#include "options.h"
#include "defines.h"

struct SimState;

namespace Actors
{
	class Actor
	{

	public:

		virtual bool finished(const SimState& state) const
		{
			return getTime(state) == TIME_INFINITY;
		}

		virtual Time getTime(const SimState& state) const = 0;
		virtual void act(SimState& state) = 0;
		
		virtual void print() {}

		virtual ~Actor() {}
	};

	std::shared_ptr<Actor> MakeUntransferredActor(const Options& opts);
	std::shared_ptr<Actor> MakeServerQueueActor(const Options& opts);
	std::shared_ptr<Actor> MakeClientQueueActor(const Options& opts);

}

