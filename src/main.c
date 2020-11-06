
// This is the main program file, that is compiled to get the stable executable

// Include Raylib library
#include "raylib.h"

// Include the game libraries
#include "globals.h" // Defines the variable types, structs and enums
#include "main.h"
#include "screens/menu.h"

int main() {

	// Initialization
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "PurpleDrank");
	InitAudioDevice();
	LoadRessources();

	// Main game loop (Detect window close button or ESC key)
	while (!WindowShouldClose()) {

		UpdateScreen();

	}

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
	background_lvl1 = LoadTexture("res/lvl1/1.png");
    	midground_lvl1 = LoadTexture("res/lvl1/2.png");
	foreground_lvl1 = LoadTexture("res/lvl1/3.png");
	soincPlayer = LoadTexture("res/soinc.png");
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
	PlayMusicStream(soincSong);

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
