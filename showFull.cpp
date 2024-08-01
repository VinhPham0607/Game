#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

#include "showFull.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void showStartMenu(SDL_Renderer *gRenderer, SDL_Texture *gStartMenu)
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    // show full table
    SDL_RenderCopy(gRenderer, gStartMenu, NULL, NULL);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(500);
}

void showFullTable(SDL_Renderer *gRenderer, SDL_Texture *gTextureBox, SDL_Texture *gTextureGalaxy,
                   int *arr, int n)
{
    // Clear screen
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    // show full table
    SDL_RenderCopy(gRenderer, gTextureGalaxy, NULL, NULL);
    SDL_RenderPresent(gRenderer);
    SDL_Rect viewport;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            viewport.x = 140 + i * 140;
            viewport.y = 40 + j * 140;
            viewport.w = 100;
            viewport.h = 100;
            SDL_RenderSetViewport(gRenderer, &viewport);
            // Render texture to screen
            SDL_RenderCopy(gRenderer, gTextureBox, NULL, NULL);
        }
    }
    viewport.x = 0;
    viewport.y = 0;
    viewport.w = SCREEN_WIDTH;
    viewport.h = SCREEN_HEIGHT;
    SDL_RenderSetViewport(gRenderer, &viewport);
    // Update screen
    SDL_RenderPresent(gRenderer);
    SDL_Delay(300);
}

void showBombTable(SDL_Renderer *gRenderer, SDL_Texture *gTextureBox, SDL_Texture *gTextureGalaxy, SDL_Texture *gTextureMar,
                   int *arr, int n, int *locatBombX, int *locatBombY, int bomb)
{
    // code render

    // Clear screen
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    // show full table
    SDL_RenderCopy(gRenderer, gTextureGalaxy, NULL, NULL);
    SDL_RenderPresent(gRenderer);
    SDL_Rect viewport;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            viewport.x = 140 + i * 140;
            viewport.y = 40 + j * 140;
            viewport.w = 100;
            viewport.h = 100;
            SDL_RenderSetViewport(gRenderer, &viewport);
            if ((j == *(locatBombX + 0) && i == *(locatBombY + 0)) || (j == *(locatBombX + 1) && i == *(locatBombY + 1)))
            {
                SDL_RenderCopy(gRenderer, gTextureMar, NULL, NULL);
            }
            // Render texture to screen
            else
                SDL_RenderCopy(gRenderer, gTextureBox, NULL, NULL);
        }
    }
    viewport.x = 0;
    viewport.y = 0;
    viewport.w = SCREEN_WIDTH;
    viewport.h = SCREEN_HEIGHT;
    SDL_RenderSetViewport(gRenderer, &viewport);
    // Update screen
    SDL_RenderPresent(gRenderer);
    SDL_Delay(1000);
}

void showWinner(SDL_Renderer *gRenderer, SDL_Texture *gTextureCongrats)
{
    const int startw = 700;
    const int starth = 365;
    for (int i = 0; i < 10; i++)
    {
        SDL_Rect viewport;
        viewport.x = (SCREEN_WIDTH - startw / (10 - i)) / 2;
        viewport.y = (SCREEN_HEIGHT - starth / (10 - i)) / 2;
        viewport.w = startw / (10 - i);
        viewport.h = starth / (10 - i);
        SDL_RenderSetViewport(gRenderer, &viewport);
        SDL_RenderCopy(gRenderer, gTextureCongrats, NULL, NULL);
        viewport.x = 0;
        viewport.y = 0;
        viewport.w = SCREEN_WIDTH;
        viewport.h = SCREEN_HEIGHT;
        SDL_RenderSetViewport(gRenderer, &viewport);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(50);
    }
}
