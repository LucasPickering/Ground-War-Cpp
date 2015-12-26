#pragma once
#include "Unit.h"
#include "Flag.h"

class Tile
{
public:
	enum TileType
	{
		TILE, BASE, MOUNTAIN, GOLD, SPAWN, SPAWNABLE
	};

	Tile();
	Tile(TileType);
	Tile(TileType, int backgroundColor, int outlineColor);
	virtual ~Tile();

	/*
	Checks if this Tile is available to be moved onto. For most tiles,
	this just checks if unit is nullptr.
	*/
	virtual bool openForMovement();

	/*
	Sets the array of tiles adjacent to this one. Should be called once,
	shortly after construction. The array passed in here should be deleted
	with the object.
	*/
	virtual void setAdjacents(Tile**);

	/*
	Checks if the given Tile is adjacent to this one.
	*/
	bool isAdjacent(Tile*);

	/*
	Checks if the given Player can spawn units on this tile.
	*/
	virtual bool spawnableFor(Player);

	/*
	Kills and deletes the unit on this tile. Does NOT subtract any money from the owner.
	*/
	void killUnit();

	Unit* unit();
	void setUnit(Unit*);

	/*
	Gets the flag on this tile. If tile doesn't have a flag, but the unit on it does,
	returns that flag.
	*/
	Flag* flag();
	void spawnFlag(Player);
	TileType type();

	virtual int getBackgroundColor();
	virtual int getOutlineColor();
	
private:
	Tile** m_adjacentTiles;
	Unit* m_unit = nullptr;
	Flag* m_flag = nullptr;
	TileType m_type;
	int m_backgroundColor;
	int m_outlineColor;
};

