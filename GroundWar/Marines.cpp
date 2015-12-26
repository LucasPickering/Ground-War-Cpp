#include "Marines.h"

Marines::Marines(Player player) : Unit(player, 1, 6, MARINES, "Marines") {}

float Marines::odds(Unit* unit)
{
	switch (unit->type())
	{
	case MARINES:
		return 3.0f / 6.0f;
	case ANTITANK:
		return 4.0f / 6.0f;
	case TANK:
		return 2.0f / 6.0f;
	default:
		return -1.0f;
	}
}

bool Marines::canTakeFlag(Flag* flag)
{
	return flag && owner() != flag->owner();
}
