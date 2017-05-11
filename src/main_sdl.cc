/*
Copyright (c) 2012-2017 Matthew Brennan Jones <matthew.brennan.jones@gmail.com>
A NES emulator in WebAssembly. Based on vNES.
Licensed under GPLV3 or later
Hosted at: https://github.com/workhorsy/nes_wasm
*/
#ifdef SDL

#include "SaltyNES.h"
#include <iostream>

using namespace std;


void onLoaded(void* userData, void* buffer, int size) {
	std::cout << "!!! onLoaded" << std::endl;
	std::cout << "!!! size" << size << std::endl;

	// Stop the idle loop that is waiting for this download
	emscripten_cancel_main_loop();

	// Run the emulator
	vNES vnes;
	//vnes.init(argv[1]);
	vnes.init_data((uint8_t*)buffer, size);
	vnes.pre_run_setup(nullptr);
	vnes.run();

	// Clanup the SDL resources then exit
	SDL_Quit();
}

void onFailed(void* userData) {
	std::cout << "!!! game download failed" << std::endl;
}

void idle() {
	std::cout << "idle ..." << std::endl;
}

int main(int argc, char* argv[]) {
	printf("%s\n", "");
	printf("%s\n", "SaltyNES is a browser based NES emulator derived from vNES.");
	printf("%s\n", "SaltyNES (C) 2012-2017 Matthew Brennan Jones <matthew.brennan.jones@gmail.com>");
	printf("%s\n", "vNES 2.14 (C) 2006-2011 Jamie Sanders thatsanderskid.com");
	printf("%s\n", "Use of this program subject to GNU GPL, Version 3.");
	printf("%s\n", "");
/*
	// Make sure there is a rom file name
	if(argc < 2) {
		fprintf(stderr, "No rom file argument provided. Exiting ...\n");
		return -1;
	}
*/
	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Could not initialize SDL: %s\n",
			SDL_GetError());
		return -1;
	}

	// Grab a SDL surface from the screen
	Globals::sdl_screen = SDL_SetVideoMode(256, 240, 32, SDL_SWSURFACE|SDL_ANYFORMAT);
	if(!Globals::sdl_screen) {
		fprintf(stderr, "Couldn't create a surface: %s\n",
			SDL_GetError());
		return -1;
	}

	// Start downloading the game file
	emscripten_async_wget_data("game.nes", nullptr, onLoaded, onFailed);

	// Wait here until the game is downloaded
	emscripten_set_main_loop(idle, 1, true);

	return 0;
}

#endif
