/**
 * 01-clear-screen
 *
**/

// Although using SDL3, this code does not use the SDL callback functions.
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdint.h>

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

// RSRE = RARM SDL Rasterizer Enginer

typedef uint8_t RSRE_uint8_vec4[4];

void RSRE_clear(SDL_Surface  *surface, RSRE_uint8_vec4 color) {
  SDL_Color sdl_color = {
    .r = (Uint8) color[0],
    .g = (Uint8) color[1],
    .b = (Uint8) color[2],
    .a = (Uint8) color[3]
  };
  
  for (int i = 0; i < surface->w * surface->h; i++) {
    ((SDL_Color *) (surface->pixels))[i] = sdl_color;
  }
}

void RSRE_draw(SDL_Surface *dst, SDL_Surface *src) {
  // See the docs: https://wiki.libsdl.org/SDL3/SDL_Rect
  SDL_Rect srcrect = {
    .x = 0,
    .y = 0,
    .w = src->w,
    .h = src->h
  };

  SDL_Rect dstrect = {
    .x = 0,
    .y = 0,
    .w = dst->w,
    .h = dst->h
  };

  if (!SDL_BlitSurface(src, &srcrect, dst, &dstrect)) {
    SDL_Log("Error drawing to the window surface: %s.\n", SDL_GetError());
  }
}

int main(int argc, char *argv[]) {
  // Read the docs: https://wiki.libsdl.org/SDL3/SDL_SetAppMetadata
  SDL_SetAppMetadata("01 Clear Screen", "1.0", "pro.rodolfo.tutorial.rasterizer.clearscreen");

  // Read the docs: https://wiki.libsdl.org/SDL3/SDL_Init
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Error initializing SDL: %s.\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Read the docs: https://wiki.libsdl.org/SDL3/SDL_CreateWindow
  SDL_Window *window = SDL_CreateWindow(
    "Clear Screen",
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    0
  );
  if (window == NULL) {
    SDL_Log("Error creating the screen: %s.", SDL_GetError());
    SDL_Quit();
    return SDL_APP_FAILURE;
  }

  // Read the docs: https://wiki.libsdl.org/SDL3/SDL_CreateSurface
  SDL_Surface *surface = SDL_CreateSurface(
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_PIXELFORMAT_RGBA32
  );
  if (surface == NULL) {
    SDL_Log("Error creating drawing surface: %s.\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return SDL_APP_FAILURE;
  }

  // Clearing color.
  RSRE_uint8_vec4 color = {125, 0, 0, 255};
  RSRE_clear(surface, color);
  
  // Draw.
  RSRE_draw(SDL_GetWindowSurface(window), surface);

  // Read the docs: https://wiki.libsdl.org/SDL3/SDL_UpdateWindowSurface
  SDL_UpdateWindowSurface(window);

  // Read the docs: https://wiki.libsdl.org/SDL3/SDL_SetSurfaceBlendMode
  if (SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE)) {
    SDL_Log("Error setting surface blend mode: %s.\n", SDL_GetError());
  }

  // Loop.
  SDL_Event event;
  bool running = 1;

  while (running) {
    // Read the docs: https://wiki.libsdl.org/SDL3/SDL_WaitEvent
    if (SDL_WaitEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          running = 0;
          break;
      }
    } else {
      SDL_Log("Error retrieving next event: %s.", SDL_GetError());
      running = 0;
    }
  }

  // Clean up.
  SDL_DestroyWindow(window);
  SDL_Quit();

  return SDL_APP_SUCCESS;
}
