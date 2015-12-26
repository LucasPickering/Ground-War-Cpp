#include "Tile.h"

Tile::Tile() : Tile(TILE) {}

Tile::Tile(TileType type) : Tile(type, 0x6aa84f, 0x434343) {}

Tile::Tile(TileType type, int backgroundColor, int outlineColor)
{
	m_type = type;
	m_backgroundColor = backgroundColor;
	m_outlineColor = outlineColor;
}

Tile::~Tile()
{
	delete[] m_adjacentTiles;
	delete m_flag;
	delete m_unit;
}

bool Tile::openForMovement()
{
	return m_unit == nullptr;
}

void Tile::setAdjacents(Tile** adjacents)
{
	m_adjacentTiles = adjacents; // This needs to be deleted later on (in destructor)
}

bool Tile::isAdjacent(Tile* tile)
{
	if (!tile)
	{
		return false;
	}
	for (int i = 0; i < 6; i++)
	{
		if (tile == m_adjacentTiles[i]) // *Should* check if they point to the same object
		{
			return true;
		}
	}
	return false;
}

bool Tile::spawnableFor(Player player)
{
	return false;
}

void Tile::killUnit()
{
	// If the unit was carrying a flag, put it on this tile.
	if (m_unit->flag())
	{
		m_flag = m_unit->flag();
		m_unit->setFlag(nullptr);
	}
	delete m_unit;
	m_unit = nullptr;
}

Unit* Tile::unit()
{
	return m_unit;
}

void Tile::setUnit(Unit* unit)
{
	m_unit = unit;

	// If the unit can carry the flag on this tile, give it to the unit.
	// canTakeFlag does the null check for m_flag.
	if (m_unit && m_unit->canTakeFlag(m_flag))
	{
		m_unit->setFlag(m_flag);
		m_flag = nullptr;
	}
}

Flag* Tile::flag()
{
	if (unit() && unit()->flag())
	{
		return unit()->flag();
	}
	return m_flag;
}

void Tile::spawnFlag(Player owner)
{
	m_flag = new Flag(owner);
}

Tile::TileType Tile::type()
{
	return m_type;
}

int Tile::getBackgroundColor()
{
	return m_backgroundColor;
}

int Tile::getOutlineColor()
{
	return m_outlineColor;
}