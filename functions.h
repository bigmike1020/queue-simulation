#pragma once

#include <cmath>
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
	
	template<typename Number>
	class exponential_distribution
	{
			Number lambda;
	public:
			exponential_distribution(Number lambda) : lambda(lambda)
			{
				assert(std::isfinite(lambda));
				assert(lambda > 0.0f);
			}

			template<typename R>
			float operator()(R&& rand) const{
					Number size = rand.max() - rand.min();
					Number num = (rand() - rand.min()) / size;
					return -log(num) / lambda;
			}
	};


} // util::

