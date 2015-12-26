#pragma once
#include "Unit.h"
class Marines :
	public Unit
{
public:
	Marines(Player);
	float odds(Unit*);
	bool canTakeFlag(Flag*);
};

