#include "memory.h"
#include <SDL2/SDL.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init() { return !(SDL_Init(SDL_INIT_VIDEO) < 0); }
void teardown() {
  SDL_Quit();
}

void allocateState(State *state) {
  uint64_t StableMemorySize = 512;
  state->StableMemorySize = StableMemorySize;
  state->StableMemory = malloc(StableMemorySize);
}

int main(int argc, char **argv) {
  void *handle = NULL;
  float (*cosine)(State *) = NULL;
  void (*updateAndRender)(State *, SDL_Window *, SDL_Surface *);
  char *error = NULL;
  int iters = 0;
  State state;
  allocateState(&state);
  SDL_Window *window = NULL;

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
      while (iters < 20) {
        iters++;
        system("make sdl_app.dylib");
        handle = dlopen("sdl_app.dylib", RTLD_LAZY);
        if (!handle) {
          fputs(dlerror(), stderr);
          free(&state);
          exit(1);
        }

        cosine = dlsym(handle, "my_cos");
        updateAndRender = dlsym(handle, "updateAndRender");
        if ((error = dlerror()) != NULL) {
          fputs(error, stderr);
          free(&state);
          exit(1);
        }

        printf("%f\n", (*cosine)(&state));
        updateAndRender(&state, window, screenSurface);
        dlclose(handle);
        sleep(1);
      }
    }
  }
  // Destroy window
  SDL_DestroyWindow(window);
  teardown();
}
