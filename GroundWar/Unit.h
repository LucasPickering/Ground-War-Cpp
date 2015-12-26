#pragma once
#include "Player.h"
#include "Flag.h"

class Unit
{
public:
	enum UnitType
	{
		MARINES, ANTITANK, TANK
	};

	Unit(Player owner, int goldCost, int movementCost, UnitType type, const char* name);
	virtual ~Unit();

	Player owner();
	int goldCost();
	int movementCost();
	UnitType type();
	Flag* flag();
	void setFlag(Flag*);
	const char* name();
	virtual bool canTakeFlag(Flag*);

	/*
	Returns a float [0, 1] representing the odds of this unit winning in combat against the given unit.
	0 is always a loss and 1 is always a win.
	*/
	virtual float odds(Unit*)  = 0;
private:
	Player m_owner;
	int m_goldCost;
	int m_movementCost;
	UnitType m_type;
	Flag* m_flag = nullptr;
	const char* m_name;
};

