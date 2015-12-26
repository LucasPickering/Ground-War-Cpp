#pragma once

#include "Tile.h"

class MountainTile :
	public Tile
{
public:
	MountainTile();
	bool openForMovement();
};

