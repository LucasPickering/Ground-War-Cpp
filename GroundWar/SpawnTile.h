#pragma once

#include "Tile.h"

class SpawnTile : 
	public Tile
{
public:
	SpawnTile();
	int getBackgroundColor();
	int getOutlineColor();
};

