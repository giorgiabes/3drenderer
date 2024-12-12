#include <SDL2/SDL.h>
#include <stdio.h>

void initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
  }
}

int main(void) {
  initialize_window();

  return 0;
}
