#include "MountainTile.h"

MountainTile::MountainTile() : Tile(MOUNTAIN, 0x666666, 0x434343) {}

bool MountainTile::openForMovement()
{
	return false;
}