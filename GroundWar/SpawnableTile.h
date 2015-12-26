#pragma once
#include "SpawnTile.h"

class SpawnableTile :
	public Tile
{
public:
	SpawnableTile();
	void setAdjacents(Tile**);
	int getOutlineColor();
	bool spawnableFor(Player);

private:
	SpawnTile* m_spawnTile;
};

