#include "Tank.h"

Tank::Tank(Player player) : Unit(player, 3, 3, TANK, "Tank") {}

float Tank::odds(Unit* unit)
{
	switch (unit->type()) 
	{
	case MARINES:
		return 4.0f / 6.0f;
	case ANTITANK:
		return 2.0f / 6.0f;
	case TANK:
		return 3.0f / 6.0f;
	default:
		return -1.0f;
	}
}