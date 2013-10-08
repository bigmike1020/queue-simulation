#pragma once

#include <memory>
#include <sstream>
#include <utility>

#define RETURN(x) -> decltype(x) {return x;}

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
	
	template<typename T> auto begin(T& t) RETURN(t.begin())
	template<typename T> auto end(T& t) RETURN(t.end())

} // util::

