#include <cxxtest/TestSuite.h>
#include "Board.h"
#include "Marines.h"
#include "AntiTank.h"
#include "Tank.h"

class GroundWarTestSuite : public CxxTest::TestSuite
{
public:
	Board board;
	Unit* redMarine;
	Unit* blueMarine;
	Unit* redAntiTank;
	Unit* blueAntiTank;
	Unit* redTank;
	Unit* blueTank;
	Flag* redFlag;
	Flag* blueFlag;

	void setUp()
	{
		// Construct all the units
		redMarine = new Marines(RED);
		blueMarine = new Marines(BLUE);
		redAntiTank = new AntiTank(RED);
		blueAntiTank = new AntiTank(BLUE);
		redTank = new Tank(RED);
		blueTank = new Tank(BLUE);
		redFlag = new Flag(RED);
		blueFlag = new Flag(BLUE);
	}

	void testGame()
	{
		// Test getTile
		TS_ASSERT_EQUALS(board.getTile(0, 0), nullptr);
		TS_ASSERT_EQUALS(board.getTile(7, 7)->type(), Tile::TILE);

		// Test canMove on red's turn
		TS_ASSERT(!board.canMove(nullptr));
		TS_ASSERT(board.canMove(redMarine));
		TS_ASSERT(!board.canMove(blueMarine));
		TS_ASSERT(board.canMove(redAntiTank));
		TS_ASSERT(!board.canMove(blueAntiTank));
		TS_ASSERT(board.canMove(redTank));
		TS_ASSERT(!board.canMove(blueTank));

		// Test canSpawn on red's turn
		TS_ASSERT(!board.canSpawn(nullptr));
		TS_ASSERT(board.canSpawn(redMarine));
		TS_ASSERT(!board.canSpawn(blueMarine));
		TS_ASSERT(board.canSpawn(redAntiTank));
		TS_ASSERT(!board.canSpawn(blueAntiTank));
		TS_ASSERT(board.canSpawn(redTank));
		TS_ASSERT(!board.canSpawn(blueTank));

		// Test canSpawnOn
		TS_ASSERT(!board.canSpawnOn(nullptr, nullptr)); // Red base
		TS_ASSERT(board.canSpawnOn(redMarine, board.getTile(0, 7))); // Red base
		TS_ASSERT(board.canSpawnOn(redMarine, board.getTile(1, 8))); // Red base
		TS_ASSERT(board.canSpawnOn(redAntiTank, board.getTile(2, 8))); // Red base
		TS_ASSERT(board.canSpawnOn(redTank, board.getTile(3, 9))); // Red base
		TS_ASSERT(!board.canSpawnOn(redMarine, board.getTile(0, 0))); // Null tile
		TS_ASSERT(!board.canSpawnOn(redMarine, board.getTile(0, 2))); // Regular tile
		TS_ASSERT(!board.canSpawnOn(redMarine, board.getTile(6, 4))); // Gold tile
		TS_ASSERT(!board.canSpawnOn(redMarine, board.getTile(4, 0))); // Unowned spawn tile
		TS_ASSERT(!board.canSpawnOn(redMarine, board.getTile(4, 1))); // Unowned spawnable tile
		TS_ASSERT(!board.canSpawnOn(redMarine, board.getTile(3, 5))); // Mountain tile
		TS_ASSERT(!board.canSpawnOn(redMarine, board.getTile(9, 0))); // Blue base

		TS_ASSERT_EQUALS(board.spawningUnit(), nullptr);
		board.prepareToSpawn(Unit::MARINES);
		TS_ASSERT_EQUALS(board.spawningUnit()->type(), Unit::MARINES);
		TS_ASSERT_DIFFERS(board.spawningUnit()->type(), Unit::ANTITANK);
		TS_ASSERT_DIFFERS(board.spawningUnit()->type(), Unit::TANK);

		TS_ASSERT(!board.spawnUnit(board.getTile(6, 4))); // Gold tile
		TS_ASSERT(board.spawnUnit(board.getTile(0, 7)));
		TS_ASSERT_EQUALS(board.getTile(0, 7)->unit()->type(), Unit::MARINES);
		TS_ASSERT_EQUALS(board.spawningUnit(), nullptr);
	}

	// Tile tests
	void testOpenForMovement()
	{
		TS_ASSERT(!board.getTile(4, 4)->openForMovement()); // Mountain tile

		Tile* tile = board.getTile(4, 5); // Regular tile
		TS_ASSERT(tile->openForMovement());
		tile->setUnit(new Marines(BLUE));
		TS_ASSERT(!tile->openForMovement());
	}

	void testIsAdjacent()
	{
		Tile* tile = board.getTile(7, 7);
		TS_ASSERT(!tile->isAdjacent(nullptr));

		TS_ASSERT(tile->isAdjacent(board.getTile(6, 6)));
		TS_ASSERT(tile->isAdjacent(board.getTile(7, 6)));
		TS_ASSERT(tile->isAdjacent(board.getTile(8, 6)));
		TS_ASSERT(tile->isAdjacent(board.getTile(8, 7)));
		TS_ASSERT(tile->isAdjacent(board.getTile(7, 8)));
		TS_ASSERT(tile->isAdjacent(board.getTile(6, 7)));

		TS_ASSERT(!tile->isAdjacent(board.getTile(3, 3)));
		TS_ASSERT(!tile->isAdjacent(board.getTile(7, 5)));
		TS_ASSERT(!tile->isAdjacent(board.getTile(6, 5)));
		TS_ASSERT(!tile->isAdjacent(board.getTile(6, 8)));
	}

	void testSpawnableFor()
	{
		// Regular tile
		TS_ASSERT(!board.getTile(0, 2)->spawnableFor(RED));
		TS_ASSERT(!board.getTile(0, 2)->spawnableFor(BLUE));

		// Gold tile
		TS_ASSERT(!board.getTile(6, 4)->spawnableFor(RED));
		TS_ASSERT(!board.getTile(6, 4)->spawnableFor(BLUE));

		// Unowned spawn tile
		TS_ASSERT(!board.getTile(4, 0)->spawnableFor(RED));
		TS_ASSERT(!board.getTile(4, 0)->spawnableFor(BLUE));

		// Unowned spawnable tile
		TS_ASSERT(!board.getTile(4, 1)->spawnableFor(RED));
		TS_ASSERT(!board.getTile(4, 1)->spawnableFor(BLUE));

		// Red base tile
		TS_ASSERT(board.getTile(0, 7)->spawnableFor(RED));
		TS_ASSERT(!board.getTile(0, 7)->spawnableFor(BLUE));

		// Blue base tile
		TS_ASSERT(!board.getTile(9, 0)->spawnableFor(RED));
		TS_ASSERT(board.getTile(9, 0)->spawnableFor(BLUE));
	}

	void testKillUnit()
	{
		Tile* tile = board.getTile(5, 5);
		TS_ASSERT_EQUALS(tile->unit(), nullptr);
		tile->setUnit(new Marines(RED));
		TS_ASSERT(tile->unit());
		tile->killUnit();
		TS_ASSERT_EQUALS(tile->unit(), nullptr);
	}

	void testSetUnit()
	{
		Tile* tile = board.getTile(5, 5);
		TS_ASSERT_EQUALS(tile->unit(), nullptr);
		Unit* unit = new Marines(RED);
		tile->setUnit(unit);
		TS_ASSERT_EQUALS(unit, tile->unit());
	}

	void testSpawnFlag()
	{
		Tile* tile = board.getTile(5, 5);
		TS_ASSERT_EQUALS(tile->flag(), nullptr);
		tile->spawnFlag(RED);
		TS_ASSERT(tile->flag());
		TS_ASSERT_EQUALS(tile->flag()->owner(), RED);
	}

	void testFlagMovement() 
	{
		Tile* tile = board.getTile(3, 7);
		Tile* tile2 = board.getTile(2, 7);
		tile->spawnFlag(BLUE);
		Unit* marine = new Marines(RED);
		tile->setUnit(marine);
		board.moveUnit(tile, tile2);
		TS_ASSERT(tile2->flag());

	}

	void testRedEndGame()
	{
		Tile* tile = board.getTile(2, 7);
		Tile* tile2 = board.getTile(1, 8);
		tile->spawnFlag(BLUE);
		Unit* marine = new Marines(RED);
		tile->setUnit(marine);
		board.moveUnit(tile, tile2);
		TS_ASSERT(board.gameOver());
		TS_ASSERT_EQUALS(board.winner(), RED);
	}
	void testBlueEndGame()
	{
		Tile* tile = board.getTile(10, 1);
		Tile* tile2 = board.getTile(11, 1);
		Tile* tmpTile = board.getTile(9, 0);
		Tile* tmpTile2 = board.getTile(8, 0);
		tile->spawnFlag(RED);
		tmpTile->setUnit(new Marines(BLUE));
		board.moveUnit(tmpTile, tmpTile2);
		board.nextTurn();
		Unit* marine = new Marines(BLUE);
		tile->setUnit(marine);
		board.moveUnit(tile, tile2);
		TS_ASSERT(board.gameOver());
		TS_ASSERT_EQUALS(board.winner(), BLUE);
	}
	void TestAttack()
	{
		Tile* tile = board.getTile(5, 4);
		Tile* tile2 = board.getTile(5, 5);
		Unit* red = new Marines(RED);
		Unit* blue = new Marines(BLUE);
		tile->setUnit(red);
		tile2->setUnit(blue);
		board.attack(tile, tile2);
		TS_ASSERT_DIFFERS (tile2->unit(), redTank);
		TS_ASSERT_DIFFERS(tile2->unit(), redAntiTank);
		TS_ASSERT_DIFFERS(tile2->unit(), blueTank);
		TS_ASSERT_DIFFERS(tile2->unit(), blueAntiTank);
	}
		
	void testTileType()
	{
		TS_ASSERT_EQUALS(board.getTile(5, 5)->type(), Tile::TILE);
		TS_ASSERT_EQUALS(board.getTile(0, 7)->type(), Tile::BASE);
		TS_ASSERT_EQUALS(board.getTile(9, 0)->type(), Tile::BASE);
		TS_ASSERT_EQUALS(board.getTile(5, 3)->type(), Tile::MOUNTAIN);
		TS_ASSERT_EQUALS(board.getTile(6, 4)->type(), Tile::GOLD);
		TS_ASSERT_EQUALS(board.getTile(8, 8)->type(), Tile::SPAWN);
		TS_ASSERT_EQUALS(board.getTile(8, 7)->type(), Tile::SPAWNABLE);
	}

	// Unit tests (haha get it)
	void testBlueUnits() {
		TS_ASSERT_EQUALS(blueTank->owner(), BLUE);
		TS_ASSERT_EQUALS(blueAntiTank->owner(), BLUE);
		TS_ASSERT_EQUALS(blueMarine->owner(), BLUE);

		TS_ASSERT_EQUALS(blueTank->goldCost(), 3);
		TS_ASSERT_EQUALS(blueAntiTank->goldCost(), 2);
		TS_ASSERT_EQUALS(blueMarine->goldCost(), 1);

		TS_ASSERT_EQUALS(blueTank->movementCost(), 3);
		TS_ASSERT_EQUALS(blueAntiTank->movementCost(), 4);
		TS_ASSERT_EQUALS(blueMarine->movementCost(), 6);

		TS_ASSERT_EQUALS(blueTank->type(), Unit::TANK);
		TS_ASSERT_EQUALS(blueAntiTank->type(), Unit::ANTITANK);
		TS_ASSERT_EQUALS(blueMarine->type(), Unit::MARINES);

		TS_ASSERT(!blueTank->canTakeFlag(redFlag));
		TS_ASSERT(!blueTank->canTakeFlag(blueFlag));
		TS_ASSERT(!blueAntiTank->canTakeFlag(redFlag));
		TS_ASSERT(!blueAntiTank->canTakeFlag(blueFlag));
		TS_ASSERT(blueMarine->canTakeFlag(redFlag));
		TS_ASSERT(!blueMarine->canTakeFlag(blueFlag));

		TS_ASSERT_EQUALS(blueTank->odds(redTank), 1.0f / 2.0f);
		TS_ASSERT_EQUALS(blueTank->odds(redAntiTank), 1.0f / 3.0f);
		TS_ASSERT_EQUALS(blueTank->odds(redMarine), 2.0f / 3.0f);
		TS_ASSERT_EQUALS(blueAntiTank->odds(redTank), 2.0f / 3.0f);
		TS_ASSERT_EQUALS(blueAntiTank->odds(redAntiTank), 1.0f / 2.0f);
		TS_ASSERT_EQUALS(blueAntiTank->odds(redMarine), 1.0f / 3.0f);
		TS_ASSERT_EQUALS(blueMarine->odds(redTank), 1.0f / 3.0f);
		TS_ASSERT_EQUALS(blueMarine->odds(redAntiTank), 2.0f / 3.0f);
		TS_ASSERT_EQUALS(blueMarine->odds(redMarine), 1.0f / 2.0f);
	}

	void testRedUnits() {
		TS_ASSERT_EQUALS(redTank->owner(), RED);
		TS_ASSERT_EQUALS(redAntiTank->owner(), RED);
		TS_ASSERT_EQUALS(redMarine->owner(), RED);

		TS_ASSERT_EQUALS(redTank->goldCost(), 3);
		TS_ASSERT_EQUALS(redAntiTank->goldCost(), 2);
		TS_ASSERT_EQUALS(redMarine->goldCost(), 1);

		TS_ASSERT_EQUALS(redTank->movementCost(), 3);
		TS_ASSERT_EQUALS(redAntiTank->movementCost(), 4);
		TS_ASSERT_EQUALS(redMarine->movementCost(), 6);

		TS_ASSERT_EQUALS(redTank->type(), Unit::TANK);
		TS_ASSERT_EQUALS(redAntiTank->type(), Unit::ANTITANK);
		TS_ASSERT_EQUALS(redMarine->type(), Unit::MARINES);
	
		TS_ASSERT_EQUALS(redTank->name(), "Tank");
		TS_ASSERT_EQUALS(redAntiTank->name(), "Antitank");
		TS_ASSERT_EQUALS(redMarine->name(), "Marines");

		TS_ASSERT(!redTank->canTakeFlag(redFlag));
		TS_ASSERT(!redTank->canTakeFlag(blueFlag));
		TS_ASSERT(!redAntiTank->canTakeFlag(redFlag));
		TS_ASSERT(!redAntiTank->canTakeFlag(blueFlag));
		TS_ASSERT(!redMarine->canTakeFlag(redFlag));
		TS_ASSERT(redMarine->canTakeFlag(blueFlag));

		TS_ASSERT_EQUALS(redTank->odds(blueTank), 1.0f / 2.0f);
		TS_ASSERT_EQUALS(redTank->odds(blueAntiTank), 1.0f / 3.0f);
		TS_ASSERT_EQUALS(redTank->odds(blueMarine), 2.0f / 3.0f);
		TS_ASSERT_EQUALS(redAntiTank->odds(blueTank), 2.0f / 3.0f);
		TS_ASSERT_EQUALS(redAntiTank->odds(blueAntiTank), 1.0f / 2.0f);
		TS_ASSERT_EQUALS(redAntiTank->odds(blueMarine), 1.0f / 3.0f);
		TS_ASSERT_EQUALS(redMarine->odds(blueTank), 1.0f / 3.0f);
		TS_ASSERT_EQUALS(redMarine->odds(blueAntiTank), 2.0f / 3.0f);
		TS_ASSERT_EQUALS(redMarine->odds(blueMarine), 1.0f / 2.0f);
	}

	// Flag tests (no joke here)
	void testFlag()
	{
		TS_ASSERT_EQUALS(redFlag->owner(), RED);
		TS_ASSERT_DIFFERS(redFlag->owner(), BLUE);

		TS_ASSERT_EQUALS(blueFlag->owner(), BLUE);
		TS_ASSERT_DIFFERS(blueFlag->owner(), RED);
	}

	void tearDown()
	{
		// Delete all the units
		delete redMarine;
		delete blueMarine;
		delete redAntiTank;
		delete blueAntiTank;
		delete redTank;
		delete blueTank;
		delete redFlag;
		delete blueFlag;
	}
};
