#include "constants.h"
#include "memory.h"
#include <SDL2/SDL.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool init() { return !(SDL_Init(SDL_INIT_VIDEO) < 0); }
SDL_Surface *loadMedia(SDL_Surface *surface) {

  char *assetPath = "assets/bmp/farzaan-kassam-539463-unsplash.bmp";
  surface = SDL_LoadBMP(assetPath);
  if (surface == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n", assetPath,
           SDL_GetError());
    return NULL;
  }

  printf("%p\n", surface);
  return surface;
}
void teardown() { SDL_Quit(); }

void allocateState(State *state) {
  uint64_t StableMemorySize = 512;
  state->StableMemorySize = StableMemorySize;
  state->StableMemory = malloc(StableMemorySize);
}

int main(int argc, char **argv) {
  void *handle = NULL;
  void (*updateAndRender)(State *, SDL_Window *, SDL_Surface *, SDL_Surface *);
  char *error = NULL;
  int iters = 0;
  State state;
  allocateState(&state);
  SDL_Window *window = NULL;
  SDL_Surface *image = NULL;
  SDL_Surface *screenSurface = NULL;
  if (!init()) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  } else {
    window = SDL_CreateWindow("Loader Example", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {

      screenSurface = SDL_GetWindowSurface(window);
      image = loadMedia(image);
      if (image == NULL) {
        free(&state);
        exit(1);
      }
      while (iters < 20) {
        iters++;
        system("make sdl_app.dylib");
        handle = dlopen("sdl_app.dylib", RTLD_LAZY);
        if (!handle) {
          fputs(dlerror(), stderr);
          free(&state);
          exit(1);
        }

        updateAndRender = dlsym(handle, "updateAndRender");
        if ((error = dlerror()) != NULL) {
          fputs(error, stderr);
          free(&state);
          exit(1);
        }

        updateAndRender(&state, window, screenSurface, image);
        dlclose(handle);
        sleep(1);
      }
    }
  }
  // Destroy window
  SDL_DestroyWindow(window);
  teardown();
}
