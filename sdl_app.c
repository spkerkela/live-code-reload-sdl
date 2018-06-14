#include "constants.h"
#include "memory.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void updateAndRender(State *mem, SDL_Window *window, SDL_Surface *screenSurface,
                     SDL_Surface *image) {
  SDL_Surface *optimizedSurface = NULL;
  optimizedSurface = SDL_ConvertSurface(image, screenSurface->format, 0);
  if (optimizedSurface == NULL) {
    printf("Unable to optimize image! SDL Error: %s\n", SDL_GetError());
  }
  SDL_Rect stretchRect;
  stretchRect.x = 0;
  stretchRect.y = 0;
  stretchRect.w = SCREEN_WIDTH;
  stretchRect.h = SCREEN_HEIGHT;

  // SDL_BlitSurface(image, NULL, screenSurface, NULL);
  SDL_BlitScaled(image, NULL, screenSurface, &stretchRect);

  // Update the surface
  SDL_UpdateWindowSurface(window);
}
