#pragma once
#include "Tile.h"
class BaseTile :
	public Tile
{
public:
	BaseTile(Player);
	bool spawnableFor(Player);

private:
	Player m_owner;
};

