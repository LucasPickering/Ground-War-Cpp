#pragma once
#include "Tile.h"
#include "Player.h"
#include "Constants.h"
#include "Unit.h"

class Board
{
public:
	Board();
	~Board();

	/*
	Gets a pointer to the tile at the given x and y. Returns NULL if x or y is out of bounds.
	*/
	Tile* getTile(int x, int y);

	/*
	Checks if the given unit can be moved on this turn. Criteria include:
	- Unit isn't null
	- Does the unit belong to the current player?
	- Does the current player have enough movement points to move this unit?
	*/
	bool canMove(Unit*);

	/*
	Checks if the unit on the first tile can be moved to the second. Criteria include:
	- canMove(from->unit()) passes
	- Is there a unit on the first tile?
	- Is the destination tile open?
	- Are the two tiles adjacent?
	*/
	bool validMove(Tile*, Tile*);

	/*
	 Moves the unit from the first tile to the second, if it's a valid move.
	 Returns true if the unit is moved, false otherwise.
	 */
	bool moveUnit(Tile*, Tile*);

	/*
	Can the given unit be spawned by the current player? Criteria include:
	- Unit isn't null
	- Is it owned by the current player?
	- Can the owner afford it?
	*/
	bool canSpawn(Unit*);

	/*
	Checks if the given unit can be spawned on the given tile. Criteria include:
	- Unit and tile aren't null
	- Does canSpawn(unit) pass?
	- Is the tile open?
	- Is the tile "spawnable" for the owner?
	*/
	bool canSpawnOn(Unit*, Tile*);

	/*
	Prepares to spawn a unit of the given type, if valid.
	*/
	void prepareToSpawn(Unit::UnitType type);

	/*
	Spawns m_spawningUnit on the given tile, if it's valid.
	Also subtracts the cost of the unit from the owner's gold.
	Returns true if the unit is spawned, false otherwise.
	*/
	bool spawnUnit(Tile*);

	/*
	Checks if the unit on the first tile can attack the unit on the second. Criteria include:
	- Is there a unit on the first tile?
	- Does the unit on the first tile belong to the current player?
	- Is there a unit on the second tile?
	- Does the unit on the second tile belong to the other player?
	- Are the tiles adjacent?
	*/
	bool canAttack(Tile*, Tile*);

	/*
	Moves the unit on the first tile to attack the unit on the second tile.
	Returns true if the attack happens, false otherwise.
	*/
	bool attack(Tile*, Tile*);

	/*
	Ends the current player's turn. Updates m_currentPlayer to the next player
	and resets m_movementPoints.
	*/
	void nextTurn();

	/*
	Gets an array of Tile* for the tiles adjacent to the tile at (x, y).
	The array will always be of size 6, with the tile at 0 being the tile directly
	above the given one and rotating clockwise from there. If there is no tile
	in a given direction, the pointer for that till will be nullptr. The array this
	returns is dynamically allocated and needs to be deleted manually later on.
	*/
	Tile** findAdjacents(int x, int y);

	/*
	Called when the mouse is clicked on the screen. Returns true if a tile was clicked,
	or false if the click was outside the board.
	*/
	bool onMouseClick(const int& mouseX, const int& mouseY);

	/*
	Returns true if the given tile is selected, false otherwise.
	*/
	bool isSelected(Tile* tile);

	/*
	Gets the selected tile.
	*/
	Tile* selectedTile();

	/*
	Gets the spawning unit.
	*/
	Unit* spawningUnit();

	/*
	Gets the pixel location for a tile at the given x and y indices.
	This pointer needs to be deleted later on.
	*/
	SDL_Point* tilePosition(const int& tileX, const int& tileY);

	/*
	Gets the amount of money for the given player.
	*/
	int money(Player);

	/*
	Gets the amount of movement points available for the current player.
	*/
	int movementPoints();

	/*
	Gets the player whose turn it currently is.
	*/
	Player currentPlayer();

	/*
	True if the game is over, false if it is still going.
	*/
	bool gameOver();

	/*
	Gets the player who won the game. Null if no one has won yet.
	*/
	Player winner();

private:
	int* m_money; // Array of 2 ints, one for each player
	Tile*** m_tiles; // 2D array of pointers to Tiles
	Tile* m_selectedTile = nullptr;
	Player m_currentPlayer = RED;
	int m_movementPoints = MOVEMENT_POINTS;
	Unit* m_spawningUnit = nullptr;
	Player* m_winner = nullptr;

	void loadBoard();
	Unit* unitForType(Unit::UnitType type);
	double distance(const int& x1, const int& y1, const int& x2, const int& y2);
	Tile* getTileForChar(const char& c);
	Tile* tileUnderMouse(const int& mouseX, const int& mouseY);

	/*
	Checks if given player has no units and not enough gold for units.
	*/
	void checkStalemate(Player p);

	/*
	Ends the game, with victory going to the current player (the game ends on the winner's turn).
	*/
	void endGame(Player);
};
