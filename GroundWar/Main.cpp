#include "Main.h"

int main(int, char**) {
	board = new Board();
	renderer = new Renderer();

	if (renderer->init() != 0)
	{
		cleanup();
		return 1;
	}

	bool runGame = true;
	SDL_Event event;

	while (runGame)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				runGame = false;
			}
			else if (!board->gameOver()) // If the game isn't over, look for KB/M input
			{
				switch (event.type)
				{
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						board->onMouseClick(event.button.x, event.button.y);
					}
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_m:
						board->prepareToSpawn(Unit::MARINES);
						break;
					case SDLK_a:
						board->prepareToSpawn(Unit::ANTITANK);
						break;
					case SDLK_t:
						board->prepareToSpawn(Unit::TANK);
						break;
					case SDLK_SPACE:
						board->nextTurn();
						break;
					}
					break;
				}
			}
		}

		renderer->draw(board);
	}

	cleanup();
	return 0;
}

void cleanup()
{
	delete board;
	delete renderer;
}