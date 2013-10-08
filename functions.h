#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

namespace util{

	// emulate variadics for 5 args
	template<class T, class A0>
	std::unique_ptr<T> make_unique(A0&& a0){
		return std::unique_ptr<T>(new T(std::forward<A0>(a0)));
	}

	template<class T, class A0, class A1>
	std::unique_ptr<T> make_unique(A0&& a0, A1&& a1){
		return std::unique_ptr<T>(new T(std::forward<A0>(a0),
			std::forward<A1>(a1)));
	}

	template<class T, class A0, class A1, class A2>
	std::unique_ptr<T> make_unique(A0&& a0, A1&& a1, A2&& a2){
		return std::unique_ptr<T>(new T(std::forward<A0>(a0),
			std::forward<A1>(a1), std::forward<A2>(a2)));
	}

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

	Logger write();

	class sstream
	{
		std::ostringstream ss;
	public:
		sstream()
		{
			ss.precision(4);
			ss << std::showpoint;
		}

		template<typename T>
		sstream& operator<<(const T& t)
		{
			ss << t;
			return *this;
		}

		std::string str() const { return ss.str()
	}

} // util::

