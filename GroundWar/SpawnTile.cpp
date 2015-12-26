#include "SpawnTile.h"

SpawnTile::SpawnTile() : Tile(SPAWN, 0xa26991, 0xff00ff) {}

int SpawnTile::getBackgroundColor()
{
	if (unit())
	{
		if (unit()->owner() == RED)
		{
			return 0x8f7e3b; // Red background
		}
		return 0x4f7e7b; // Blue background
	}
	return Tile::getBackgroundColor(); // Default background
}

int SpawnTile::getOutlineColor()
{
	if (unit())
	{
		if (unit()->owner() == RED)
		{
			return 0xff0000; // Red outline
		}
		return 0x0000ff; // Blue outline
	}
	return Tile::getOutlineColor(); // Default outline
}
