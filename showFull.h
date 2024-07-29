#ifndef SHOWFULL_H
#define SHOWFULL_H

void showStartMenu(SDL_Renderer *gRenderer, SDL_Texture *gStartMenu);
void showBombTable(SDL_Renderer *gRenderer, SDL_Texture *gTextureBox, SDL_Texture *gTextureGalaxy, SDL_Texture *gTextureMar,
                   int *arr, int n, int *locatBombX, int *locatBombY, int bomb);
#endif // SHOWFULL_H
