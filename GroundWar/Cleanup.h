#pragma once

#include <utility>
#include <SDL.h>

template<typename T, typename... Args>
void cleanup(T *t, Args&&... args)
{
	cleanup(t);
	cleanup(std::forward<Args>(args)...);
}

template<>
void cleanup<SDL_Window>(SDL_Window* win)
{
	if (win)
	{
		SDL_DestroyWindow(win);
	}
}

template<>
void cleanup<SDL_Renderer>(SDL_Renderer* ren)
{
	if (ren)
	{
		SDL_DestroyRenderer(ren);
	}
}

template<>
void cleanup<SDL_Texture>(SDL_Texture* tex)
{
	if (tex)
	{
		SDL_DestroyTexture(tex);
	}
}

template<>
void cleanup<SDL_Surface>(SDL_Surface* surf)
{
	if (surf)
	{
		SDL_FreeSurface(surf);
	}
}

template<>
void cleanup<TTF_Font>(TTF_Font* font)
{
	if (font)
	{
		TTF_CloseFont(font);
	}
}