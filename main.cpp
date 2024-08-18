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
#include <SDL2/SDL_ttf.h> // 18.8

#include "showFull.h"
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

// CountDown
TTF_Font *gFont = NULL;


// CountDonw

bool init()
{
    // Initialization flag
    bool success = true;

    // CountDown
    if (TTF_Init() == -1)
    {
        printf("SDL TTF could not initialize! SDL Error: %s\n", TTF_GetError());
        success = false;
    }
    else
    {
        gFont = TTF_OpenFont("font/Zebulon.otf",28);
        if (gFont == NULL)
        {
            printf("Failed to load font. SDL Error: %s\n", TTF_GetError());
            success = false;
        }
    }
    // CountDown

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
        gWindow = SDL_CreateWindow("Finding Star", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

    gScratch = Mix_LoadWAV("assets/background.mp3");
    if (gScratch == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    gGameOver = Mix_LoadWAV("assets/game-over.wav");
    if (gGameOver == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    gWinner = Mix_LoadWAV("assets/winners.wav");
    if (gScratch == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    // Load texture
    gStartMenu = loadTexture("assets/start-menu.png");
    if (gStartMenu == NULL)
    {
        printf("Failed to load texture image!\n");
        success = false;
    }
    gTextureBox = loadTexture("assets/box.png");
    if (gTextureBox == NULL)
    {
        printf("Failed to load texture image!\n");
        success = false;
    }
    gTextureMar = loadTexture("assets/star.png");
    if (gTextureMar == NULL)
    {
        printf("Failed to load texture image!\n");
        success = false;
    }
    gTextureGalaxy = loadTexture("assets/background.png");
    if (gTextureGalaxy == NULL)
    {
        printf("Failed to load texture image!\n");
        success = false;
    }
    gTextureCongrats = loadTexture("assets/winners.png");
    if (gTextureCongrats == NULL)
    {
        printf("Failed to load texture image!\n");
        success = false;
    }
    gTextureLevelup = loadTexture("assets/level-up.png");
    if (gTextureLevelup == NULL)
    {
        printf("Failed to load texture image!\n");
        success = false;
    }
    gTextureGameOver = loadTexture("assets/game-over.png");
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

// CounDonw
void renderCoundDown(SDL_Renderer *renderer, TTF_Font *font, int secondsLeft, bool display)
{
    int textWidth = 0, textHeight = 0;
    std::string countdownText = std::to_string(secondsLeft) + " (s)";

    SDL_Surface *tempSurface = TTF_RenderText_Solid(font, countdownText.c_str(), {0, 0, 0, 255});
    if (display == true)tempSurface = TTF_RenderText_Solid(font, countdownText.c_str(), {255, 255, 255, 0});
    if (tempSurface != nullptr)
    {
        textWidth = tempSurface->w;
        textHeight = tempSurface->h;
        SDL_FreeSurface(tempSurface);
    }
    else
    {
        std::cerr << "Unable to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Rect rect = {SCREEN_WIDTH - textWidth - 670, 15, textWidth + 15, textHeight + 10};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, countdownText.c_str(), {0, 0, 0, 255});
    if (display == true)textSurface = TTF_RenderText_Solid(font, countdownText.c_str(), {255, 255, 255, 0});
    if (textSurface != nullptr)
    {
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect renderQuad = {rect.x + 5, rect.y + 5, textWidth, textHeight};
        SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
    else
    {
        std::cerr << "Unable to create text surface: " << TTF_GetError() << std::endl;
    }

    SDL_RenderPresent(renderer);
}

// CounDonw



int main(int argc, char *args[])
{
    const int numberOfBomb = 2;
    int bombLocatX[numberOfBomb], bombLocatY[numberOfBomb];
    int *table;
    int tmp = 2;

    // CountDown
    int startTime = 0;
    int elapsedTime = 0;

    // CountDonw

    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        // Load media
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            bool quit = false;
            bool startGame = false;
            bool gameover = false;
            SDL_Event e;

            while (!quit)
            {
                startGame = false;
                gameover = false;
                int result = 0;
                int oldresult = result;
                int timewait = 40;
                int timeswitch = 8;

                // Display start menu
                while (!startGame && !quit)
                {
                    showStartMenu(gRenderer, gStartMenu);
                    while (SDL_PollEvent(&e) != 0)
                    {
                        switch (e.type)
                        {
                        case SDL_QUIT:
                            startGame = true;
                            quit = true;
                            gameover = true;
                            break;

                        // Mouse button pressed
                        case SDL_MOUSEBUTTONDOWN:
                            if (e.button.button == SDL_BUTTON_LEFT)
                            {
                                if (e.motion.x > 240 && e.motion.x < 365 && e.motion.y > 325 && e.motion.y < 435)
                                {
                                    startGame = true;
                                }
                                if (e.motion.x > 423 && e.motion.x < 545 && e.motion.y > 325 && e.motion.y < 435)
                                {
                                    startGame = true;
                                    quit = true;
                                    gameover = true;
                                }
                            }
                            break;
                        }
                    }
                }

                if (!quit)
                {
                    Mix_PlayChannel(-1, gScratch, 100);

                    while (!gameover)
                    {
                        startTime = SDL_GetTicks(); // CountDonw

                        table = generateTable(n, numberOfBomb, bombLocatX, bombLocatY);
                        showBombTable(gRenderer, gTextureBox, gTextureGalaxy, gTextureMar, table, n, bombLocatX, bombLocatY, numberOfBomb);
                        showFullTable(gRenderer, gTextureBox, gTextureGalaxy, table, n);

                        switchBomb(table, n, bombLocatX, bombLocatY, numberOfBomb, gRenderer, gTextureBox, gTextureGalaxy, timewait, timeswitch);
                        timewait -= 4;
                        timeswitch += 3;

                        for (int i = 0; i < numberOfBomb; i++)
                        {
                            printf("bomb locat: %i and %i\n", *(bombLocatX + i), *(bombLocatY + i));
                        }
                        int timeOver = 10;
                        while (!quit && !gameover)
                        {

                            while ((timeOver > 0 && SDL_PollEvent(&e) == 0 && SDL_PollEvent(&e) == 0)
                                    || (SDL_PollEvent(&e) != 0 && e.type != SDL_MOUSEBUTTONDOWN && e.button.button != SDL_BUTTON_LEFT))
                            {
                                renderCoundDown(gRenderer, gFont, timeOver, false);
                                SDL_RenderPresent(gRenderer);
                                SDL_Delay(1000);
                                renderCoundDown(gRenderer, gFont, timeOver, true);
                                timeOver--;

                            }
                                if (timeOver == 0)
                                {
                                    Mix_HaltChannel(-1);
                                    showBombTable(gRenderer, gTextureBox, gTextureGalaxy, gTextureMar, table, n, bombLocatX, bombLocatY, numberOfBomb);
                                    SDL_Delay(500);
                                    Mix_PlayChannel(-1, gGameOver, 0);
                                    SDL_RenderCopy(gRenderer, gTextureGameOver, NULL, NULL);
                                    SDL_RenderPresent(gRenderer);
                                    gameover = true;
                                    SDL_Delay(7500);
                                    break;
                                }
                            while (SDL_PollEvent(&e) != 0)
                            {
                                switch (e.type)
                                {
                                case SDL_QUIT:
                                    quit = true;
                                    gameover = true;
                                    break;

                                case SDL_MOUSEBUTTONDOWN:
                                    if (e.button.button == SDL_BUTTON_LEFT)
                                    {
                                        oldresult = result;
                                        for (int l = 0; l < numberOfBomb; l++)
                                        {
                                            int a = *(bombLocatX + l), b = *(bombLocatY + l);

                                            if (e.motion.x > 140 + b * 140 && e.motion.x < 240 + b * 140 && e.motion.y > 40 + a * 140 && e.motion.y < 140 + a * 140)
                                            {
                                                result += 1;
                                                tmp = l;
                                            }
                                        }
                                        if (result > oldresult)
                                        {
                                            printf("Correct\n");
                                            printf("result %i\n time %i\n", result, timewait);
                                            if (timewait <= 20 && result == 2)
                                            {
                                                showResult(gRenderer, gTextureBox, gTextureGalaxy, gTextureMar, gTextureLevelup, table, n, bombLocatX, bombLocatY, numberOfBomb, result, tmp, timewait);
                                                Mix_HaltChannel(-1);
                                                Mix_PlayChannel(-1, gWinner, 0);
                                                showWinner(gRenderer, gTextureCongrats);
                                                SDL_Delay(5000);
                                                gameover = true;
                                            }
                                            else
                                            {
                                                showResult(gRenderer, gTextureBox, gTextureGalaxy, gTextureMar, gTextureLevelup, table, n, bombLocatX, bombLocatY, numberOfBomb, result, tmp, timewait);
                                            }
                                        }
                                        else if (result == 0 || (result == oldresult && result == 1))
                                        {
                                            Mix_HaltChannel(-1);
                                            showBombTable(gRenderer, gTextureBox, gTextureGalaxy, gTextureMar, table, n, bombLocatX, bombLocatY, numberOfBomb);
                                            SDL_Delay(500);
                                            Mix_PlayChannel(-1, gGameOver, 0);
                                            SDL_RenderCopy(gRenderer, gTextureGameOver, NULL, NULL);
                                            SDL_RenderPresent(gRenderer);
                                            gameover = true;
                                            SDL_Delay(7500);
                                        }
                                    }
                                    break;
                                }
                            }
                            if (result == 2)
                            {
                                SDL_Delay(500);
                                result = 0;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    close();

    return 0;
}



