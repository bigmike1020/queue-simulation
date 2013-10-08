#pragma once

#include <memory>
#include <sstream>
#include <utility>

namespace util{

	struct Logger
	{
		std::ostringstream ss;

		template<typename T>
		Logger& operator<<(const T& t)
		{
			ss << t;
			return *this;
		}

		~Logger();
	};

} // util::

