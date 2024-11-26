#include "display.h"

// SDL objects for the window and renderer
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
// Pointer to a buffer storing pixel colors and its associated SDL texture
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
// Dimensions of the application window
int window_width = 800;
int window_height = 600;

// Function to initialize the SDL window and renderer
bool initialize_window(void) {
        // Initialize all SDL subsystems
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                fprintf(stderr, "Error initializing SDL.\n");
                return false;
        }

        // Get the current display mode to set window dimensions
        SDL_DisplayMode display_mode;
        SDL_GetCurrentDisplayMode(0, &display_mode);
        window_width = display_mode.w;
        window_height = display_mode.h;

        // Create an SDL window in the center of the screen, borderless
        window = SDL_CreateWindow(
                NULL,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                window_width,
                window_height,
                SDL_WINDOW_BORDERLESS
        );
        if (!window) {
                fprintf(stderr, "Error creating SDL window.\n");
                return false;
        }

        // Create an SDL renderer for the window
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (!renderer) {
                fprintf(stderr, "Error creating SDL renderer.\n");
                return false;
        }

        // Set the window to fullscreen mode
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

        return true;
}

// Function to draw a grid on the color buffer
void draw_grid() {
        for (int y = 0; y < window_height; y += 50) { // Increment y by 50 pixels
                for (int x = 0; x < window_width; x += 50) { // Increment x by 50 pixels
                        // Set grid point color to yellow (ARGB format)
                        color_buffer[(window_width * y) + x] = 0xFFFFFF00;
                }
        }
}

// Function to draw a rectangle on the color buffer
void draw_rect(int x, int y, int width, int height, uint32_t color) {
        // Iterate over the width of the rectangle
        for (int i = 0; i < width; i++) {
                // Iterate over the height of the rectangle
                for (int j = 0; j < height; j++) {
                        // Calculate the current pixel's x and y coordinates
                        int current_x = x + i;
                        int current_y = y + j;

                        // Set the color of the current pixel in the color buffer
                        // Ensure coordinates are within the buffer bounds to avoid overflow
                        color_buffer[(window_width * current_y) + current_x] = color;
                }
        }
}

// Function to render the color buffer to the screen
void render_color_buffer(void) {
        // Update the SDL texture with the color buffer data
        SDL_UpdateTexture(
                color_buffer_texture,
                NULL,
                color_buffer,
                (int)(window_width * sizeof(uint32_t))
        );

        // Copy the texture to the renderer
        SDL_RenderCopy(
                renderer,
                color_buffer_texture,
                NULL,
                NULL
        );
}

// Function to clear the color buffer with a specific color
void clear_color_buffer(uint32_t color) {
        for (int y = 0; y < window_height; y++) {
                for (int x = 0; x < window_width; x++) {
                        color_buffer[(window_width * y) + x] = color;
                }
        }
}

// Function to release resources and close the SDL window
void destroy_window(void) {
        free(color_buffer); // Free allocated memory for the color buffer
        SDL_DestroyRenderer(renderer); // Destroy the renderer
        SDL_DestroyWindow(window); // Destroy the window
        SDL_Quit(); // Quit SDL subsystems
}
