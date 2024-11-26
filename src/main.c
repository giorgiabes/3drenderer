#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "display.h"

// Global variables to manage the application state
bool is_running = false; // Flag to check if the program is running



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

// Function to handle the rendering of the current frame
void render(void) {
        // Set the background color to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw a grid on the color buffer
        draw_grid();
	
	// Draw a rectangle on the color buffer
	draw_rect(300, 200, 300, 150, 0xFFFF00FF);

        // Render the color buffer and clear it for the next frame
        render_color_buffer();
        clear_color_buffer(0xFF000000); // Clear with black color

        // Present the renderer to update the window
        SDL_RenderPresent(renderer);
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

