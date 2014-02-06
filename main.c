// Simple game using SDL and OpenGL

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 1920
#define HEIGHT 1080
#define SPRITESIZE 32

SDL_Rect rcSrc, rcSprite;


static void repaint() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnd();
	SDL_GL_SwapBuffers();
}

static void setup_sdl() {
	const SDL_VideoInfo* video;

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialise SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// Quit SDL properly on exit
	atexit(SDL_Quit);

	// Get current video information
	video = SDL_GetVideoInfo();
	if (video == NULL) {
		fprintf(stderr, "Couldn't get video information %s\n", SDL_GetError());
		exit(1);
	}

	// Set minimim requirements for the OpenGL window
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

	// Set title
	SDL_WM_SetCaption( "Totally Bananas?", NULL );

	// Enable keyboard repeat
	SDL_EnableKeyRepeat(70, 70);

	if (SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_OPENGL | SDL_FULLSCREEN) == 0) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
}

static void setup_opengl() {
	float aspect = (float)WIDTH / (float)HEIGHT;

	// Viewport to cover entire window
	glViewport(0, 0, WIDTH, HEIGHT);

	// 2D mode for GL
	glMatrixMode(GL_PROJECTION);

	// Set clear color to gray
	glClearColor(0.5, 0.5, 0.5, 0);

	// Disable depth checking
	glDisable(GL_DEPTH_TEST);

}

static void main_loop() {
	SDL_Event event;
	SDL_Surface *screen, *sprite, *temp;
	int colorkey;

	// Load sprite
	temp = SDL_LoadBMP("girl.bmp");
	sprite = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	// Setup sprite colorkey and load RLE
	colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
	SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	// Set sprite position
	rcSprite.x = 150;
	rcSprite.y = 150;

	// Set animation frame
	rcSrc.x = 128;
	rcSrc.y = 0;
	rcSrc.w = SPRITESIZE;
	rcSrc.h = SPRITESIZE;

	while (1) {
		// Process pending events
		while (SDL_PollEvent( &event ) ) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					exit(0);
				break;
				default:
					// no default action
					break;
				}
			break;

			case SDL_QUIT:
				exit(0);
				break;
			}
		}

		// Update the screen
		repaint();

		// Wait 50ms to avoid using al the CPU time
		SDL_Delay(50);
	}
}

int main(int argc, char ** argv) {

	// Setup environment
	setup_sdl();
	setup_opengl();

	// Run main game loop
	main_loop();

	return 0;
}
