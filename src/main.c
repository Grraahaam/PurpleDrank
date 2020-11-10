
// This is the main program file, that is compiled to get the stable executable

#include <stdio.h>
#include <unistd.h>
#include "stdbool.h" // Allow bool variable 

// Include Raylib library
#include "raylib.h"
#include "../lib/raylib/raymath.h"
#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS

#include "../lib/raylib/physac.h" 
#include "globals.h" // Defines the variable types, structs and enums
#include "structure.h"

// Include the game libraries
#include "main.h"
#include "screens/menu.h"
#include "screens/level1.h"

bool quit = false;
bool starting = true;

int main() {

	getdir();

	puts("[DEBUG] Launching window!");

	InitWindow(screenWidth, screenHeight, "PurpleDrank");

	puts("[DEBUG] Init audio device!");

	InitAudioDevice();

	puts("[DEBUG] Load resources!");

	// Initialize physics and default physics bodies
    InitPhysics();
	LoadRessources();
	

	puts("[DEBUG] Game window is starting!");
	RunPhysicsStep();

	// Main game loop (Detect window close button or ESC key)
	while (!WindowShouldClose() && !quit) {
		
		UpdateScreen();

	}

	puts("[DEBUG] Game window is closing!");

	// De-Initialization
	ClosePhysics();       // Unitialize physics
	UnloadRessources();
	CloseAudioDevice();
	CloseWindow(); //Close window and OpenGL context

	return 0;
}

// Loading all the ressources (TODO organize it into a struct?)
void LoadResources() {
	// Textures MUST be loaded after Window initialization (OpenGL context is required)
	backgroundMenu = LoadTexture("res/menu/background.png");
	background_lvl1 = LoadTexture("res/Level1/ECRAN1V.png");
	background_lvl2 = LoadTexture("res/Level2/ECRAN2V.png");
	background_lvl3 = LoadTexture("res/Level3/ECRAN3V.png");
	soincReverse = LoadTexture("res/solin_reverse.png");
    solin_head = LoadTexture("res/solin_head.png");
	soincPlayer = LoadTexture("res/solin.png");
	soincSong = LoadMusicStream("res/menu/smash.mp3");

	//trying to load a PhysicsBody, then using in level1.c
	body = CreatePhysicsBodyRectangle((Vector2){ 80, screenHeight/2 }, 50, 60, 1);
	
}

// Unloading all the ressources (With the future struct, automatize Unload* function to dynamically unload ressources)
// Load toutes les ressources avant 
void UnloadRessources() {
	UnloadTexture(backgroundMenu);
	UnloadTexture(background_lvl1);
	UnloadTexture(background_lvl2);
	UnloadTexture(background_lvl3);
	UnloadTexture(soincReverse);
	UnloadTexture(soincPlayer);
	UnloadTexture(solin_head);
	StopMusicStream(soincSong);
}

// Function managing the screen
void UpdateScreen() {
	
	// Play the song (loaded previously)
	//PlayMusicStream(soincSong);
	//game.gameScreen = MENU;
	if (starting) {
		game.gameScreen = MENU;
		starting = false;
	}
	// Draw the corresponding screen
	switch(game.gameScreen) {
		case MENU: {
			MenuDraw();
		} break;
		case LEVEL_1: {
			LevelOneDraw();
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
