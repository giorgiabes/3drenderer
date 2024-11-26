#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

// Global variables to manage the application state
bool is_running = false; // Flag to check if the program is running

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

// Function to allocate memory and initialize the color buffer and texture
void setup(void) {
        // Allocate memory for the color buffer
        color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

        // Create an SDL texture for the color buffer
        color_buffer_texture = SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_ARGB8888, // Use ARGB pixel format
                SDL_TEXTUREACCESS_STREAMING, // Allow streaming updates
                window_width,
                window_height
        );
}

// Function to handle user input and update the application state
void process_input(void) {
        SDL_Event event;
        SDL_PollEvent(&event);

        // Handle different types of SDL events
        switch (event.type) {
                case SDL_QUIT: { // Quit event
                        is_running = false;
                        break;
                }
                case SDL_KEYDOWN: { // Key press event
                        if (event.key.keysym.sym == SDLK_ESCAPE) { // Check for Escape key
                                is_running = false;
                                break;
                        }
                }
        }
}

// Placeholder function for updating game logic (to be implemented later)
void update(void) {
        // TODO:
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

// Function to handle the rendering of the current frame
void render(void) {
        // Set the background color to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw a grid on the color buffer
        draw_grid();

        // Render the color buffer and clear it for the next frame
        render_color_buffer();
        clear_color_buffer(0xFF000000); // Clear with black color

        // Present the renderer to update the window
        SDL_RenderPresent(renderer);
}

// Function to release resources and close the SDL window
void destroy_window(void) {
        free(color_buffer); // Free allocated memory for the color buffer
        SDL_DestroyRenderer(renderer); // Destroy the renderer
        SDL_DestroyWindow(window); // Destroy the window
        SDL_Quit(); // Quit SDL subsystems
}

// Main function
int main(void) {
        // Initialize the window and start the application
        is_running = initialize_window();
        setup();

        // Main loop: process input, update logic, and render frames
        while (is_running) {
                process_input();
                update();
                render();
        }

        // Clean up resources before exiting
        destroy_window();

        return 0;
}

