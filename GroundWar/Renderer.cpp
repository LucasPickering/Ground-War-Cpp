#include "Renderer.h"
#include <iostream>
#include <cstdio>
#include <SDL_image.h>
#include "Constants.h"
#include "Cleanup.h"

Renderer::~Renderer()
{
	// Clean up renderer, window, and font
	cleanup(renderer, window, font);

	// Clean up textures
	for (TexMap::iterator iter = textures.begin(); iter != textures.end(); ++iter)
	{
		SDL_DestroyTexture(iter->second);
	}

	// Quit everything
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

int Renderer::init()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	// Initialize SDL_image
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}

	// Initialize the texture path
	char *basePath = SDL_GetBasePath();
	if (basePath)
	{
		texPath = basePath;
		SDL_free(basePath);
		// Cut out the last folder in the dir and replace it with "Textures"
		texPath = texPath.substr(0, texPath.rfind("\\", texPath.length() - 2)) + "\\Textures\\%s.png";
	}
	else {
		logSDLError(std::cout, "Get Resource Path");
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	// Initialize window
	window = SDL_CreateWindow("Ground War", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		logSDLError(std::cout, "SDL_CreateWindow");
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	// Initialize renderer
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		cleanup(window);
		logSDLError(std::cout, "SDL_CreateRenderer");
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	// Load some textures
	loadTexture(TILE_BG);
	loadTexture(TILE_OUTLINE);
	loadTexture(FLAG_TEX);
	loadTexture("Marines");
	loadTexture("Antitank");
	loadTexture("Tank");
	loadTexture(VICTORY_TEX);

	// Initialize TTF
	if (TTF_Init() != 0)
	{
		cleanup(window, renderer);
		logSDLError(std::cout, "TTF_Init");
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	// Initialize font
	font = TTF_OpenFont("C:\\Windows\\Fonts\\Arial.ttf", 20);
	if (font == nullptr)
	{
		cleanup(window, renderer);
		logSDLError(std::cout, "TTF_OpenFont");
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	return 0;
}

void Renderer::draw(Board* board)
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);

	// Draw the board
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			Tile* tile = board->getTile(x, y);
			if (tile)
			{
				SDL_Point* p = board->tilePosition(x, y);
				int mode = 0;

				// Set overlay mode
				if (board->isSelected(tile))
				{
					mode = SELECTED; // Tile is selected. Draw with the appropriate highlight.
				}
				else if (tile->isAdjacent(board->selectedTile()))
				{
					if (tile->openForMovement())
					{
						mode = MOVABLE; // Tile can be moved to. Draw with the appropriate highlight.
					}
					else if (board->canAttack(board->selectedTile(), tile))
					{
						mode = ATTACKABLE; // Tile can be attack. Draw with the appropriate highlight.
					}
				}
				drawTile(tile, p->x, p->y, mode);
				delete p;
			}
		}
	}

	// Write gold info on the screen
	char s[20];
	sprintf(s, "Gold: %d", board->money(RED));
	writeText(s, RED_INFO.x, RED_INFO.y, RED_COLOR);
	sprintf(s, "Gold: %d", board->money(BLUE));
	writeText(s, BLUE_INFO.x, BLUE_INFO.y, BLUE_COLOR);

	// Writing movement point info
	sprintf(s, "Movement points: %d", board->movementPoints());
	writeText(s, NEUTRAL_INFO.x, NEUTRAL_INFO.y,
		board->currentPlayer() == RED ? RED_COLOR : BLUE_COLOR);

	// Write spawning unit info
	if (board->spawningUnit())
	{
		sprintf(s, "Spawning %s", board->spawningUnit()->name());
		writeText(s, NEUTRAL_INFO.x, NEUTRAL_INFO.y + 20,
			board->currentPlayer() == RED ? RED_COLOR : BLUE_COLOR);
	}
	if (board->gameOver()) // If the game is over, draw a victory screen
	{
		drawVictory(board->winner());
	}

	SDL_RenderPresent(renderer);
	SDL_Delay(10); // 100 FPS
}

void Renderer::logSDLError(std::ostream &os, const std::string &msg)
{
	os << msg << " Error: " << SDL_GetError() << std::endl;
}

SDL_Texture* Renderer::loadTexture(const char* texName)
{
	char s[256];
	sprintf(s, texPath.c_str(), texName);
	SDL_Texture *texture = IMG_LoadTexture(renderer, s);
	if (texture == nullptr)
	{
		logSDLError(std::cout, "Load Texture");
	}
	return textures[texName] = texture;
}

void Renderer::renderTexture(const char* texName, int x, int y)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;

	// Load the texture from the map
	SDL_Texture* texture = textures[texName];
	SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);

	SDL_RenderCopy(renderer, texture, nullptr, &dst);
}

void Renderer::drawTile(Tile* tile, int x, int y, int highlight)
{
	if (tile)
	{
		// Draw tile background
		int color = tile->getBackgroundColor();
		if (highlight & SELECTED)
		{
			color = 0xffffff;
		}
		SDL_SetTextureColorMod(textures[TILE_BG], color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff);
		renderTexture(TILE_BG, x, y);

		// Draw tile foreground (outline)
		color = tile->getOutlineColor();
		if (highlight & MOVABLE)
		{
			color = 0xffffff;
		}
		else if (highlight & ATTACKABLE)
		{
			color = 0xff9933;
		}
		SDL_SetTextureColorMod(textures[TILE_OUTLINE], color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff);
		renderTexture(TILE_OUTLINE, x, y);

		// If there's a unit on the tile, draw it
		if (Unit* unit = tile->unit())
		{
			const SDL_Color color = unit->owner() == RED ? RED_COLOR : BLUE_COLOR;
			SDL_SetTextureColorMod(textures[unit->name()], color.r, color.g, color.b);
			renderTexture(unit->name(), x, y);
		}

		// If there's a flag on the tile or the unit on the tile, draw it
		if (Flag* flag = tile->flag())
		{
			const SDL_Color color = flag->owner() == RED ? RED_COLOR : BLUE_COLOR;
			SDL_SetTextureColorMod(textures[FLAG_TEX], color.r, color.g, color.b);
			renderTexture(FLAG_TEX, x, y);
		}
	}
}

void Renderer::drawVictory(Player winner)
{
	SDL_Color color = winner == RED ? RED_COLOR : BLUE_COLOR;
	SDL_SetTextureColorMod(textures[VICTORY_TEX], color.r, color.g, color.b);
	renderTexture(VICTORY_TEX, VICTORY_POS.x, VICTORY_POS.y);
}

/**
* Render the given text, in the given color, as a texture.
*/
SDL_Texture* Renderer::createText(const std::string &text, SDL_Color color)
{
	// Render to a surface first
	SDL_Surface *surf = TTF_RenderText_Blended(font, text.c_str(), color);
	if (surf == nullptr)
	{
		logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}

	// Render the surface to a texture
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		logSDLError(std::cout, "CreateTexture");
	}

	SDL_FreeSurface(surf); // Clean up surface
	return texture;
}

void Renderer::writeText(const std::string& text, int x, int y)
{
	writeText(text, x, y, { 0, 0, 0 });
}

void Renderer::writeText(const std::string& text, int x, int y, SDL_Color color)
{
	SDL_Texture* texture = createText(text, color);
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, texture, nullptr, &dst);
	SDL_DestroyTexture(texture);
}