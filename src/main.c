
// This is the main program file, that is compiled to get the stable executable

#include <stdio.h>
#include <unistd.h>
#include "raylib.h"
#include "lib/raymath.h"
#include "lib/physac.h"
#include "lib/defines.c"

// Defines the global variables, structs and enums
#include "globals.h"
#include "main.h"


//Settings player 
Player player = { (Vector2){100, SCREEN_HEIGHT/2}, 6};


int main(int argc, char *argv[]) {

	// Switching ON/OFF the debug mode (./solin or ./solin debug)
	if(argc == 2 && strcmp(argv[1], "debug") == 0) { DEBUG = true; }
	else { DEBUG = false; }

	// Defining variables values
	bool quit = false;
	screenWidth = SCREEN_WIDTH;
	screenHeight = SCREEN_HEIGHT;

	// Launching game start-up requirements
	if(DEBUG) { getdir(); }

	PrintDebug("Launching window!");
	InitWindow(screenWidth, screenHeight, GAME_NAME);

	PrintDebug("Init audio device!");
	InitAudioDevice();

	PrintDebug("Init physics!");
	InitPhysics();

	PrintDebug("Load resources!");
	LoadResources();

	PrintDebug("Setting FPS!");
	SetTargetFPS(60);

	// Set default screen
	game.gameScreen = MENU;   

	PrintDebug("Game window is starting!");

	// Main game loop (Detect window close button or ESC key)
	while (!WindowShouldClose() && !quit) {

		UpdateScreen();
	}

	PrintDebug("Unloading resources!");
	UnloadResources();

	PrintDebug("Closing audio device!");
	CloseAudioDevice();

	PrintDebug("Unloading physics!");
	ClosePhysics();
	
	//Close window and OpenGL context
	PrintDebug("Game window is closing!");
	CloseWindow();

	return 0;
}


// Loading all the ressources (TODO organize it into a struct, defined in globals.h?)
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
	Player* player_adress = &player;
	// Play the song (loaded previously)
	//PlayMusicStream(soincSong);

	// Draw the corresponding screen
	switch(game.gameScreen) {
		case MENU: {
			MenuDraw();
		} break;
		case LEVEL_1: {
			ResetPhysics();
			LevelOneDraw(player_adress);
			
		} break;
		case LEVEL_2: {
			ResetPhysics();
			LevelTwoDraw(player_adress);
			
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

void PrintDebug(char *str) {
	if(DEBUG) { printf("[DEBUG] %s\n", str); }
}
