#ifndef SHOWFULL_H
#define SHOWFULL_H

void showStartMenu(SDL_Renderer *gRenderer, SDL_Texture *gStartMenu);
void showFullTable(SDL_Renderer *gRenderer, SDL_Texture *gTextureBox, SDL_Texture *gTextureGalaxy, int *arr, int n);
void showBombTable(SDL_Renderer *gRenderer, SDL_Texture *gTextureBox, SDL_Texture *gTextureGalaxy, SDL_Texture *gTextureMar,
                   int *arr, int n, int *locatBombX, int *locatBombY, int bomb);
void showWinner(SDL_Renderer *gRenderer, SDL_Texture *gTextureCongrats);
#endif // SHOWFULL_H
