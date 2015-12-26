#include "BaseTile.h"

BaseTile::BaseTile(Player owner) : Tile(BASE, owner == RED ? 0x8f7e3b : 0x4f7e7b, owner == RED ? 0xff0000 : 0x0000ff)
{
	m_owner = owner;
}

bool BaseTile::spawnableFor(Player player)
{
	return player == m_owner;
}