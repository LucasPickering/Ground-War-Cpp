#include "SpawnableTile.h"

SpawnableTile::SpawnableTile() : Tile(SPAWNABLE, 0x6aa84f, 0xff00ff) {}

void SpawnableTile::setAdjacents(Tile** adjacents)
{
	Tile::setAdjacents(adjacents);
	for (int i = 0; i < 6; ++i)
	{
		if (adjacents[i] && adjacents[i]->type() == SPAWN)
		{
			m_spawnTile = (SpawnTile*)adjacents[i];
			return;
		}
	}
}

int SpawnableTile::getOutlineColor()
{
	if (spawnableFor(RED))
	{
		return 0xff0000; // Red outline
	}
	if (spawnableFor(BLUE))
	{
		return 0x0000ff; // Blue outline
	}
	return Tile::getOutlineColor(); // Default outline
}

bool SpawnableTile::spawnableFor(Player player)
{
	return m_spawnTile->unit() && m_spawnTile->unit()->owner() == player;
}