#pragma once
#include <SDL.h>

static const int SCREEN_WIDTH = 1200; // Screen width, in pixels
static const int SCREEN_HEIGHT = 700; // Screen height, in pixels
static const SDL_Point BOARD_POS = { 175, 0 }; // position of the top-left of the board, in pixels
static const SDL_Point RED_INFO = { 10, 200 };
static const SDL_Point BLUE_INFO = { 1110, RED_INFO.y };
static const SDL_Point NEUTRAL_INFO = { 10, 10 };
static const SDL_Point VICTORY_POS = { 350, 250 };
static const SDL_Color RED_COLOR = { 0xff, 0x00, 0x00 };
static const SDL_Color BLUE_COLOR = { 0x00, 0x00, 0xff };
static const char* TILE_BG = "TileBackground";
static const char* TILE_OUTLINE = "TileOutline";
static const char* FLAG_TEX = "Flag";
static const char* VICTORY_TEX = "Victory";

static const int TILE_RADIUS = 40; // Radius of each tile, in pixels
static const int TILE_WIDTH = TILE_RADIUS * 2; // Vertex-to-vertex width of each tile, in pixels
static const int TILE_HEIGHT = (int)((double)TILE_RADIUS * 1.73205/*sqrt(3)*/); // Top-to-bottom height of each tile, in pixels

static const int BOARD_WIDTH = 13;
static const int BOARD_HEIGHT = 10;
static const int START_MONEY = 10;
static const int MOVEMENT_POINTS = 12;

// Highlight modes
static const int SELECTED = 1;
static const int MOVABLE = 2;
static const int ATTACKABLE = 4;

static const char* BOARD[] = {  "----SLTTTBW--",
								"--TLLLTTTTTBB",
								"TTTTTTMTTTTTT",
								"TTTTMMTTTTTTT",
								"TTTTMTGTMMTTT",
								"TTTMTTTTMTTTT",
								"TTTTTTMMTTTTT",
								"RTTTTTTTLTTT-",
								"-RVTTTTLSL---",
								"---R-T-L-----" };