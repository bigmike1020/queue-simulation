#pragma once

#include <cassert>

#include "defines.h"

class Packet
{
	int num;

public:
	Packet(int num) : num(num)
	{
		assert(num > 0 && "Num must be greater than 0");
	}

};

