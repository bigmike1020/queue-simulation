#include "functions.h"

#include <iostream>

using namespace util;

Logger::~Logger()
{
	std::cout << ss.str() << std::endl;
}
