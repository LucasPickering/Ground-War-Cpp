#pragma once
#include <string>
#include <map>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Board.h"

typedef std::map <const char*, SDL_Texture*> TexMap;

class Renderer
{
public:
	~Renderer();
	int init();
	void draw(Board* board);

private:
	std::string texPath;
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	TexMap textures;

	/*
	Prints an SDL error the to given ostream. The given error message is appended
	to the front of the error.
	*/
	void logSDLError(std::ostream& os, const std::string& msg);

	/*
	Loads a PNG image into the renderer and stores it in the textures map.
	*/
	SDL_Texture* loadTexture(const char* texName);

	/*
	Draws a texture at the given x and y, scaled to the given width.
	*/
	void renderTexture(const char* texName, int x, int y);

	/*
	Draws the given tile at the given x and y pixel coordinates. Highlights the tile with
	the given highlight mode. Highlight modes are defined in Constants.h.
	*/
	void drawTile(Tile* tile, int x, int y, int highlight);

	/*
	Draws a victory screen with the given winner.
	*/
	void drawVictory(Player);

	SDL_Texture* createText(const std::string& text, SDL_Color color);
	void writeText(const std::string& text, int x, int y);
	void writeText(const std::string& text, int x, int y, SDL_Color color);
};

