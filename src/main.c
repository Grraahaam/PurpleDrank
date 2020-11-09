
// This is the main program file, that is compiled to get the stable executable

#include <stdio.h>
#include <unistd.h>

// Include Raylib library
#include "raylib.h"

// Include the game libraries
#include "globals.h" // Defines the variable types, structs and enums
#include "main.h"
#include "screens/menu.h"

bool quit = false;

int main() {

	getdir();

	puts("[DEBUG] Launching window!");

	InitWindow(screenWidth, screenHeight, "PurpleDrank");

	puts("[DEBUG] Init audio device!");

	InitAudioDevice();

	puts("[DEBUG] Load resources!");

	LoadRessources();

	puts("[DEBUG] Game window is starting!");

	// Main game loop (Detect window close button or ESC key)
	while (!WindowShouldClose() && !quit) {

		UpdateScreen();

	}

	puts("[DEBUG] Game window is closing!");

	// De-Initialization
	UnloadRessources();
	CloseAudioDevice();
	CloseWindow(); //Close window and OpenGL context

	return 0;
}

// Loading all the ressources (TODO organize it into a struct?)
void LoadRessources() {
	// Textures MUST be loaded after Window initialization (OpenGL context is required)
	background = LoadTexture("res/menu/background.png");
	background_lvl1 = LoadTexture("res/lvl1/background.png");
    	midground_lvl1 = LoadTexture("res/lvl1/midground.png");
	foreground_lvl1 = LoadTexture("res/lvl1/foreground.png");
	soincPlayer = LoadTexture("res/solin.png");
	soincSong = LoadMusicStream("res/menu/smash.mp3");
}

// Unloading all the ressources (With the future struct, automatize Unload* function to dynamically unload ressources)
void UnloadRessources() {
	UnloadTexture(background);
	UnloadTexture(background_lvl1);
    	UnloadTexture(midground_lvl1);
	UnloadTexture(foreground_lvl1);
	UnloadTexture(soincPlayer);
	StopMusicStream(soincSong);
}

// Function managing the screen
void UpdateScreen() {

	// Play the song (loaded previously)
	//PlayMusicStream(soincSong);

	game.gameScreen = MENU;

	// Draw the corresponding screen
	switch(game.gameScreen) {
		case MENU: {
			MenuDraw();
		} break;
		default : {
			// Default action, if screen not handled
		} break;
	}
}

int getdir() {
   char cwd[255];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("[DEBUG] Current working directory : %s\n", cwd);
   } else {
       perror("getcwd() error");
       return 1;
   }
   return 0;
}
