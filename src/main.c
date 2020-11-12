
// This is the main program file, that is compiled to get the stable executable

#include <stdio.h>
#include <unistd.h>
#include "lib/defines.h"

// Defines the global variables, structs and enums
#include "globals.h"

// Include the game libraries
#include "main.h"
#include "screens/menu.h"
#include "screens/level1.h"
#include "screens/level2.h"

int main() {

	// Defining global variables values
	bool quit = false;
	screenWidth = SCREEN_WIDTH;
	screenHeight = SCREEN_HEIGHT;

	getdir();

	puts("[DEBUG] Launching window!");

	InitWindow(screenWidth, screenHeight, "PurpleDrank");

	puts("[DEBUG] Init audio device!");

	InitAudioDevice();

	puts("[DEBUG] Load resources!");

	// Initialize physics and default physics bodies
    InitPhysics();
	
	LoadResources();

	// Set startup screen
	game.gameScreen = MENU;
	

	puts("[DEBUG] Game window is starting!");
	SetTargetFPS(60);   

	// Main game loop (Detect window close button or ESC key)
	while (!WindowShouldClose() && !quit) {

		UpdateScreen();
	}

	puts("[DEBUG] Game window is closing!");

	// De-Initialization
	UnloadResources();
	CloseAudioDevice();

	// Unitialize physics
	ClosePhysics();
	//Close window and OpenGL context	
	CloseWindow();

	return 0;
}

// Loading all the ressources (TODO organize it into a struct?)
void LoadResources() {
	// Textures MUST be loaded after Window initialization (OpenGL context is required)
	
	Image img_soincPlayer = LoadImage("res/solin.png");
    Image img_Reverse = LoadImage("res/solin_reverse.png");
    Image img_solin_head = LoadImage("res/solin_head.png");

	soincReverse = LoadTextureFromImage(img_Reverse);
   	solin_head = LoadTextureFromImage(img_solin_head);
	soincPlayer = LoadTextureFromImage(img_soincPlayer);
	backgroundMenu = LoadTexture("res/menu/background.png");
	background_lvl1 = LoadTexture("res/Level1/ECRAN1V.png");
	background_lvl2 = LoadTexture("res/Level2/ECRAN2V.png");
	background_lvl3 = LoadTexture("res/Level3/ECRAN3V.png");
	soincSong = LoadMusicStream("res/menu/smash.mp3");
}

// Unloading all the ressources (With the future struct, automatize Unload* function to dynamically unload ressources)
// Load toutes les ressources avant 
void UnloadResources() {
	UnloadTexture(backgroundMenu);
	UnloadTexture(background_lvl1);
	UnloadTexture(background_lvl2);
	UnloadTexture(background_lvl3);
	UnloadTexture(soincReverse);
	UnloadTexture(soincPlayer);
	UnloadTexture(solin_head);
	StopMusicStream(soincSong);
	UnloadImage(img_soincPlayer);
	UnloadImage(img_soincPlayer);
    UnloadImage(img_solin_head);
}

// Function managing the screen
void UpdateScreen() {
	
	// Play the song (loaded previously)
	//PlayMusicStream(soincSong);

	// Draw the corresponding screen
	switch(game.gameScreen) {
		case MENU: {
			MenuDraw();
		} break;
		case LEVEL_1: {
			LevelOneDraw();
		} break;
		case LEVEL_2: {
			//ResetPhysics();
			//LevelTwoDraw();
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
