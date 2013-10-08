#include "functions.h"

using namespace util;

Logger::~Logger()
{
	const std::string& str = ss.str();
	std::cout << str << std::endl;
}

Logger util::write()
{
	return Logger();
}

std::ostringstream util::sstream()
{
	std::ostringstream ss;
	ss.precision(4);
	ss << std::showpoint;
	return ss;
}

