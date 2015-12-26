#include "AntiTank.h"

AntiTank::AntiTank(Player player) : Unit(player, 2, 4, ANTITANK, "Antitank") {}

float AntiTank::odds(Unit* unit)
{
	switch (unit->type())
	{
	case MARINES:
		return 2.0f / 6.0f;
	case ANTITANK:
		return 3.0f / 6.0f;
	case TANK:
		return 4.0f / 6.0f;
	default:
		return -1.0f;
	}
}