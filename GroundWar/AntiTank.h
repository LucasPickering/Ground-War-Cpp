#pragma once
#include "Unit.h"
class AntiTank :
	public Unit
{
public:
	AntiTank(Player);
	float odds(Unit*);
};

