#include <cstdlib>
#include <cmath>
#include <time.h>
#include "Board.h"
#include "MountainTile.h"
#include "GoldTile.h"
#include "BaseTile.h"
#include "SpawnTile.h"
#include "SpawnableTile.h"
#include "Marines.h"
#include "Antitank.h"
#include "Tank.h"

Board::Board()
{
	m_tiles = new Tile**[BOARD_WIDTH];
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		m_tiles[x] = new Tile*[BOARD_HEIGHT];
	}
	loadBoard();
	for (int x = 0; x < BOARD_WIDTH; ++x)
	{
		for (int y = 0; y < BOARD_HEIGHT; ++y)
		{
			if (m_tiles[x][y])
			{
				// Set the adjacents for each tile
				// All tiles have to be initialized before this is done
				m_tiles[x][y]->setAdjacents(findAdjacents(x, y));
			}
		}
	}

	m_money = new int[2];
	m_money[0] = START_MONEY;
	m_money[1] = START_MONEY;

	srand(time(nullptr)); // Initialize seed for RNG
}


Board::~Board()
{
	for (int x = 0; x < BOARD_WIDTH; ++x)
	{
		for (int y = 0; y < BOARD_HEIGHT; ++y)
		{
			delete m_tiles[x][y];
		}
		delete[] m_tiles[x];
	}
	delete[] m_tiles;
	delete[] m_money;
	delete m_winner;
}

Tile* Board::getTile(int x, int y)
{
	if (0 <= x && x < BOARD_WIDTH && 0 <= y && y < BOARD_HEIGHT)
	{
		return m_tiles[x][y];
	}
	return nullptr;
}

bool Board::canMove(Unit* unit)
{
	return unit && unit->owner() == m_currentPlayer && m_movementPoints >= unit->movementCost();
}

bool Board::validMove(Tile* from, Tile* to)
{
	return canMove(from->unit()) && to->openForMovement() && from->isAdjacent(to);
}

bool Board::moveUnit(Tile* from, Tile* to)
{
	if (validMove(from, to))
	{
		Unit* unit = from->unit();
		m_movementPoints -= unit->movementCost();
		to->setUnit(unit);
		from->setUnit(nullptr);
		m_selectedTile = nullptr;

		// Check for victory
		if (to->type() == Tile::BASE && unit->flag() && ((BaseTile*)to)->spawnableFor(unit->owner()))
		{
			endGame(unit->owner());
		}

		return true;
	}
	return false;
}

bool Board::canSpawn(Unit* unit)
{
	return unit && unit->owner() == m_currentPlayer && m_money[m_currentPlayer] >= unit->goldCost();
}

bool Board::canSpawnOn(Unit* unit, Tile* tile)
{
	return canSpawn(unit) && tile && tile->openForMovement() && tile->spawnableFor(m_currentPlayer);
}

void Board::prepareToSpawn(Unit::UnitType type)
{
	// If the unit that is already spawning was re-selected, unselect it.
	if (m_spawningUnit && type == m_spawningUnit->type())
	{
		delete m_spawningUnit;
		m_spawningUnit = nullptr;
		return;
	}
	Unit* unit = unitForType(type);
	if (canSpawn(unit))
	{
		// If another unit was already spawning, delete it.
		if (m_spawningUnit)
		{
			delete m_spawningUnit;
		}

		m_spawningUnit = unit;
		m_selectedTile = nullptr;
	}
	else
	{
		delete unit;
	}
}

bool Board::spawnUnit(Tile* tile)
{
	if (canSpawnOn(m_spawningUnit, tile))
	{
		m_money[m_currentPlayer] -= m_spawningUnit->goldCost();
		tile->setUnit(m_spawningUnit);
		m_spawningUnit = nullptr;
		return true;
	}
	return false;
}

bool Board::canAttack(Tile* from, Tile* to)
{
	bool adjacent = from->isAdjacent(to);
	return from->unit() && from->unit()->owner() == m_currentPlayer && to->unit() &&
		to->unit()->owner() != m_currentPlayer && from->isAdjacent(to);
}

bool Board::attack(Tile* from, Tile* to)
{
	if (canAttack(from, to))
	{
		float odds = from->unit()->odds(to->unit());
		if ((float)rand() / (float)RAND_MAX < odds) // Attacker wins
		{
			to->killUnit();
			moveUnit(from, to);
			++m_money[m_currentPlayer];
			checkStalemate(Player(1 - m_currentPlayer));
		}
		else // Defender wins
		{
			from->killUnit();
			m_selectedTile = nullptr;
			++m_money[1 - m_currentPlayer];
			checkStalemate(m_currentPlayer);
		}
		return true;
	}
	return false;
}

void Board::nextTurn()
{
	if (m_movementPoints < MOVEMENT_POINTS) // Only allow next turn if a move has been made or no possible moves
	{
		// Check all tiles
		for (int x = 0; x < BOARD_WIDTH; x++) {
			for (int y = 0; y < BOARD_HEIGHT; y++) {
				// For gold tiles, accumulate gold for the occupier
				if (m_tiles[x][y] && m_tiles[x][y]->type() == Tile::GOLD && m_tiles[x][y]->unit()) {
					++m_money[m_tiles[x][y]->unit()->owner()];
				}
			}
		}

		m_currentPlayer = Player(1 - m_currentPlayer);
		m_movementPoints = MOVEMENT_POINTS;
		m_selectedTile = nullptr;
		m_spawningUnit = nullptr;
	}
}

void Board::checkStalemate(Player p) {
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			if (m_tiles[x][y] && m_tiles[x][y]->unit() && m_tiles[x][y]->unit()->owner() == p) { // See if player has any units
				return; // Player has one unit. No stalemate for them.
			}
		}
	}

	// No units and no money (lowest cost is 1)
	if (m_money[p] == 0)
	{
		endGame(Player(1 - p)); // Declare victory for the other player
	}
}

void Board::endGame(Player winner)
{
	m_winner = new Player(winner);
}

Tile** Board::findAdjacents(int x, int y)
{
	Tile** adjacents = new Tile*[6];
	int adjX, adjY;
	for (int i = 0; i < 6; ++i)
	{
		switch (i)
		{
		case 0:
			adjX = x;
			adjY = y - 1;
			break;
		case 1:
			adjX = x + 1;
			adjY = y - (x % 2 == 0 ? 0 : 1);
			break;
		case 2:
			adjX = x + 1;
			adjY = y + (x % 2 == 0 ? 1 : 0);
			break;
		case 3:
			adjX = x;
			adjY = y + 1;
			break;
		case 4:
			adjX = x - 1;
			adjY = y + (x % 2 == 0 ? 1 : 0);
			break;
		case 5:
			adjX = x - 1;
			adjY = y - (x % 2 == 0 ? 0 : 1);
			break;
		}

		if (0 <= adjX && adjX < BOARD_WIDTH && 0 <= adjY && adjY < BOARD_HEIGHT)
		{
			// If adjX and adjY are in bounds, set adjacents[i] to m_tiles[adjX][adjY]
			adjacents[i] = m_tiles[adjX][adjY];
		}
		else
		{
			// If either adjX or adjY is out of bounds, set adjacents[i] to null
			adjacents[i] = nullptr;
		}
	}
	return adjacents;
}

bool Board::onMouseClick(const int& mouseX, const int& mouseY)
{
	Tile* tile = tileUnderMouse(mouseX, mouseY);
	if (tile)
	{
		if (m_spawningUnit) // If a unit is getting ready to be spawned
		{
			spawnUnit(tile); // Spawn it here (spawnUnit does the necessary checks)
		}
		else if (m_selectedTile) // If a tile is selected
		{
			if (tile == m_selectedTile) // If selected tile was clicked
			{
				m_selectedTile = nullptr; // Unselect it
			}
			else if (m_selectedTile->isAdjacent(tile)) // If the clicked tile is adjacent to the selected one
			{
				if (!moveUnit(m_selectedTile, tile)) // Try to move the unit. If we can't, ...
				{
					attack(m_selectedTile, tile); // ... then try to attack.
				}
			}
		}
		else if (tile->unit() && canMove(tile->unit()))
		{
			m_selectedTile = tile;
		}
		return true;
	}
	return false;
}

Tile* Board::tileUnderMouse(const int& mouseX, const int& mouseY)
{
	for (int tileX = 0; tileX < BOARD_WIDTH; ++tileX)
	{
		for (int tileY = 0; tileY < BOARD_HEIGHT; ++tileY)
		{
			SDL_Point* center = tilePosition(tileX, tileY);
			double dist = distance(mouseX, mouseY, center->x + TILE_WIDTH / 2, center->y + TILE_HEIGHT / 2);
			delete center;
			if (dist <= TILE_HEIGHT / 2)
			{
				return m_tiles[tileX][tileY];
			}
		}
	}
	return nullptr;
}

double Board::distance(const int& x1, const int& y1, const int& x2, const int& y2)
{
	const int xDist = x1 - x2;
	const int yDist = y1 - y2;
	return sqrt(xDist * xDist + yDist * yDist);
}

void Board::loadBoard()
{
	for (int x = 0; x < BOARD_WIDTH; ++x)
	{
		for (int y = 0; y < BOARD_HEIGHT; ++y)
		{
			m_tiles[x][y] = getTileForChar(BOARD[y][x]);
		}
	}
}

Tile* Board::getTileForChar(const char& c)
{
	Tile* tile = nullptr;
	switch (c)
	{
	case 'T':
		return new Tile();
	case 'M':
		return new MountainTile();
	case 'G':
		return new GoldTile();
	case 'R':
		return new BaseTile(RED);
	case 'V':
		tile = new BaseTile(RED);
		tile->spawnFlag(RED);
		return tile;
	case 'B':
		return new BaseTile(BLUE);
	case 'W':
		tile = new BaseTile(BLUE);
		tile->spawnFlag(BLUE);
		return tile;
	case 'S':
		return new SpawnTile();
	case 'L':
		return new SpawnableTile();
	}
	return nullptr;
}

bool Board::isSelected(Tile* tile)
{
	return tile == m_selectedTile;
}

Tile* Board::selectedTile()
{
	return m_selectedTile;
}

Unit* Board::spawningUnit()
{
	return m_spawningUnit;
}

SDL_Point* Board::tilePosition(const int& tileX, const int& tileY)
{
	return new SDL_Point{ BOARD_POS.x + TILE_WIDTH * 3 / 4 * tileX,
		BOARD_POS.y + tileY * TILE_HEIGHT + (tileX % 2 == 1 ? 0 : TILE_HEIGHT / 2) };
}

Unit* Board::unitForType(Unit::UnitType type)
{
	switch (type)
	{
	case Unit::MARINES:
		return new Marines(m_currentPlayer);
	case Unit::ANTITANK:
		return new AntiTank(m_currentPlayer);
	case Unit::TANK:
		return new Tank(m_currentPlayer);
	}
	return nullptr;
}

int Board::money(Player player)
{
	return m_money[player];
}

int Board::movementPoints()
{
	return m_movementPoints;
}

Player Board::currentPlayer()
{
	return m_currentPlayer;
}

bool Board::gameOver()
{
	return m_winner != nullptr;
}

Player Board::winner()
{
	return *m_winner;
}