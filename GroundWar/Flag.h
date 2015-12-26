#pragma once
#include "Player.h"
class Flag
{
public:
	Flag(Player);
	Player owner();
private:
	Player m_owner;
};

