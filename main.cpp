#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <windows.h>

#include "generate.h"

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int n = 4;

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Loads individual image as texture
SDL_Texture *loadTexture(std::string path);

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Current displayed texture
SDL_Texture *gStartMenu = NULL;
SDL_Texture *gTextureBox = NULL;
SDL_Texture *gTextureMar = NULL;
SDL_Texture *gTextureGalaxy = NULL;
SDL_Texture *gTextureCongrats = NULL;
SDL_Texture *gTextureLevelup = NULL;
SDL_Texture *gTextureGameOver = NULL;
Mix_Chunk *gScratch = NULL;
Mix_Chunk *gGameOver = NULL;
Mix_Chunk *gWinner = NULL;

bool init()
{
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		// Create window
		gWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{

	// Loading success flag
	bool success = true;

	gScratch = Mix_LoadWAV("");
	if (gScratch == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	gGameOver = Mix_LoadWAV("");
	if (gGameOver == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	gWinner = Mix_LoadWAV("");
	if (gScratch == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	// Load texture
	gStartMenu = loadTexture("");
	if (gStartMenu == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}
	gTextureBox = loadTexture("");
	if (gTextureBox == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}
	gTextureMar = loadTexture("");
	if (gTextureMar == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}
	gTextureGalaxy = loadTexture("");
	if (gTextureGalaxy == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}
	gTextureCongrats = loadTexture("");
	if (gTextureCongrats == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}
	gTextureLevelup = loadTexture("");
	if (gTextureLevelup == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}
	gTextureGameOver = loadTexture("");
	if (gTextureGameOver == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	// Nothing to load
	return success;
}

void close()
{
	// Free loaded image
	SDL_DestroyTexture(gStartMenu);
	gStartMenu = NULL;
	SDL_DestroyTexture(gTextureBox);
	gTextureBox = NULL;
	SDL_DestroyTexture(gTextureMar);
	gTextureMar = NULL;
	SDL_DestroyTexture(gTextureGalaxy);
	gTextureGalaxy = NULL;
	SDL_DestroyTexture(gTextureCongrats);
	gTextureCongrats = NULL;
	SDL_DestroyTexture(gTextureGameOver);
	gTextureGameOver = NULL;

	Mix_FreeChunk(gScratch);
	gScratch = NULL;
	Mix_FreeChunk(gGameOver);
	gGameOver = NULL;
	Mix_FreeChunk(gWinner);
	gWinner = NULL;

	// Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	// Quit SDL subsystems
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

SDL_Texture *loadTexture(std::string path)
{
	// The final texture
	SDL_Texture *newTexture = NULL;

	// Load image at specified path
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		// Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int main(int argc, char *args[])
{
	const int numberOfBomb = 2;
	int bombLocatX[numberOfBomb], bombLocatY[numberOfBomb];
	int *table;
	int tmp = 2;
	close();

	return 0;
}



