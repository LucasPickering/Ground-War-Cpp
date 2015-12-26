#pragma once
#include "Unit.h"
class Tank :
	public Unit
{
public:
	Tank(Player player);
	float odds(Unit*);
};

