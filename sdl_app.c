#include "memory.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void updateAndRender(State *mem, SDL_Window *window,
                     SDL_Surface *screenSurface) {

  // Get window surface
  printf("rendering\n");
  screenSurface = SDL_GetWindowSurface(window);

  // Fill the surface white
  SDL_FillRect(screenSurface, NULL,
               SDL_MapRGB(screenSurface->format, 0x00, 0x12, 0x2F));

  // Update the surface
  SDL_UpdateWindowSurface(window);
}

float my_cos(State *mem) {
  printf("entering\n");

  printf("%p\n", mem);
  printf("%d\n", mem->StableMemorySize);
  printf("Timestamp: %d\n", (int)time(NULL));
  int i;
  for (i = 0; i < 2; i++) {
    printf("Iteration: %d\n", i + 1);
  }

  printf("returning\n");
  return 51213.0f;
}
