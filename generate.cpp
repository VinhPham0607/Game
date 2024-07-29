#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include "showFull.h"
#include "generate.h"

int *generateTable(int n, int bomb, int *locatBombX, int *locatBombY)
{
    int *a = new int[n * n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            a[i * n + j] = 0;

    srand(time(0));
    for (int i = 0; i < bomb; i++)
    {
        int random_x = rand() % (n - 1) + 0;
        int random_y = rand() % (n - 1) + 0;
        if (a[random_x * n + random_y] == 1)
        {
            i--;
        }
        else
        {
            a[random_x * n + random_y] = 1;
            *(locatBombX + i) = random_x;
            *(locatBombY + i) = random_y;
        }
    }
    return a;
}


