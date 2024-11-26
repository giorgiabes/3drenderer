#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

/* Global variable to control the main game loop.
 * It indicates whether the program shoudl keep running.*/
bool is_running = false;

/* Pointers for SDL window and renderer.
 *	window: Represents the application renderer.
 *	renderer: Used for drawing graphics.*/
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

/* Global variables for graphics:
 *	color_buffer: Holds the pixel data for the screen.
 *	color_buffer_texture: SDL texture for rendering the color_buffer.*/
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

/* Default window dimensions, which can later be adjusted dynamically.*/
int window_width = 800;
int window_height = 600;

/* Initializes SDL, creates a window, and sets up fullscreen mode.*/
bool initialize_window(void) {
	/* Initializes SDL subsystems. If it fails, prints an error and
	 * returns false.*/
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initialaizing SDL.\n");
		return false;
	}
	/* Queries the screen resolution and updates the window dimensions
	 * to fullscreen value.*/
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width = display_mode.w;
	window_height = display_mode.h;

	/* Creates a borderless SDL window centered on the screen.
	 * Prints an error if it fails and returns false.*/
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

	/* Creates an SDL renderer to draw graphics in the window.
	 * Prints an error if it fails and returns false.*/
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}
	/* Sets the window to fullscreen.*/
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

/* Prepares memory and resources for rendering.*/
void setup(void) {
	/* Allocates memory for the color_buffer to store pixel colors
	 * for the screen.*/
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

	/* Creates a texture for rendering the color_buffer on the sreen.*/
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
}

/* Handles user input events (keyboard and window events).*/
void process_input(void) {
	/* Checks if any SDL event (e.g. quit, keypress) has
	 * occorred.*/
	SDL_Event event;
	SDL_PollEvent(&event);

	/* Stops the program if:*/
	switch (event.type) {
		/* The user closes the window.*/
		case SDL_QUIT: {
			is_running = false;
			break;
		}
		/* The user presses the ESC key.*/
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				is_running = false;
				break;
			}
		}
	}
}

void update(void) {
	// TODO:
}

void draw_grid(void) {
	// TODO:
	// Draw a background grid that fills the entire window.
	// Lines should be rendered at every raw/col multiple of 10.
	//  __ __ __ __ __ __ __ __
	// |__|__|__|__|__|__|__|__|
	// |__|__|__|__|__|__|__|__|
	// |__|__|__|__|__|__|__|__|
	// |__|__|__|__|__|__|__|__|
	// |__|__|__|__|__|__|__|__|
	// |__|__|__|__|__|__|__|__|
	// |__|__|__|__|__|__|__|__|
	// |__|__|__|__|__|__|__|__|
}

/* Renders the color_buffer to the screen.*/
void render_color_buffer(void) {
	/* Updates the texture (color_buffer_texture) whith pixel
	 * data from color_buffer.*/
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(window_width * sizeof(uint32_t))
	);
	/* Copies the thexture onto the renderer.*/
	SDL_RenderCopy(
		renderer,
		color_buffer_texture,
		NULL,
		NULL
	);
}

/* Clears the color buffer with a specific color.*/
void clear_color_buffer(uint32_t color) {
	/* Loops through all pixels in the color_buffer
	 * and sets each pixel to the given color.*/
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

/* Handles rendering logic for each frame.*/
void render(void) {
	/* Sets the renderer's color to red and clears the screen.*/
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	/* Draws the grid.*/
	draw_grid();

	/* Renders the color_buffer.*/
	render_color_buffer();
	/* Clears the buffer with a yellow color*/
	clear_color_buffer(0xFFFFFF00);
	/* and presents the final image to the screen.*/
	SDL_RenderPresent(renderer);
}

/* Cleans up resources and quits SDL.*/
void destroy_window(void) {
	/* Frees allocated memory, destroys the SDL window and renderer (in
	 * reverse order) and shuts down SDL subsystems.*/
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

/* The program's entry point.*/
int main(void) {
	/* Initializes the window and sets up the environment.*/
	is_running = initialize_window();
	setup();

	/* Main game loop:*/
	while (is_running) {
		process_input(); // Handles user inputs.
		update(); // Updates the application state.
		render(); // Draws the frame.
	}

	/* Cleans up resources and exits.*/
	destroy_window();

	/* return 0 indicates successfull execution.*/
	return 0;
}
