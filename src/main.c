
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

// Initialize player
Player player = { (Vector2){100, SCREEN_HEIGHT/2}, 3, 0 };

int main(int argc, char *argv[]) {

	// Switching ON/OFF the debug mode (./solin or ./solin debug)
	if(argc == 2 && strcmp(argv[1], "debug") == 0) { DEBUG = true; }
	else { DEBUG = false; }

	// Defining variables values
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
	//while (!WindowShouldClose() || !quit) {
	while (!WindowShouldClose()) {

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

	solinReverse = LoadTexture("res/solin_reverse.png");
	solin_head = LoadTexture("res/solin_head.png");
	solinPlayer = LoadTexture("res/solin.png");
	solinDead = LoadTexture("res/solin_dead.png");
	backgroundMenu = LoadTexture("res/menu/background.png");
	backgroundGameover = LoadTexture("res/gameover/background.png");
	backgroundSuccess = LoadTexture("res/success.png");
	background_lvl1 = LoadTexture("res/Level1/ECRAN1V.png");
	background_lvl2 = LoadTexture("res/Level2/ECRAN2V.png");
	background_lvl3 = LoadTexture("res/Level3/ECRAN3V.png");
	lean1_available = LoadTexture("res/Level2/lean.png");
	lean2_available = LoadTexture("res/Level2/lean.png");
	lean3_available = LoadTexture("res/Level2/lean.png");
	lean1_taken = LoadTexture("res/Level2/empty.png");
	lean2_taken = LoadTexture("res/Level2/empty.png");
	lean3_taken = LoadTexture("res/Level2/empty.png");
	inventory_lean = LoadTexture("res/Level2/lean.png");
	spikes = LoadTexture("res/Level2/spikes.png");
	tremplin = LoadTexture("res/Level2/tremplin.png");
	teleportation_NOK = LoadTexture("res/Level2/tp_desac.png");
	teleportation_OK = LoadTexture("res/Level2/tp.png");

	gobleanDead = LoadTexture("res/Level3/goblean_dead.png");
	gobleanAlive = LoadTexture("res/Level3/goblean.png");
	mainG_empty = LoadTexture("res/Level3/empty.png");
	mainD_empty = LoadTexture("res/Level3/empty.png");
	mainG = LoadTexture("res/Level3/main_gauche.png");
	mainD = LoadTexture("res/Level3/main_droite.png");
	car_destroyed = LoadTexture("res/Level3/empty.png");
	car_new = LoadTexture("res/Level3/carV2.png");
	lean_destroyed = LoadTexture("res/Level3/empty.png");
	lean_launched = LoadTexture("res/Level3/lean.png");

	soincSong = LoadMusicStream("res/menu/smash.mp3");
}

// Unloading all the ressources (With the future struct, automatize Unload* function to dynamically unload ressources)
// Load toutes les ressources avant
void UnloadResources() {
	UnloadTexture(backgroundMenu);
	UnloadTexture(background_lvl1);
	UnloadTexture(background_lvl2);
	UnloadTexture(background_lvl3);
	UnloadTexture(solinReverse);
	UnloadTexture(solinPlayer);
	UnloadTexture(solinDead);
	UnloadTexture(solin_head);

	UnloadTexture(gobleanDead);
	UnloadTexture(gobleanAlive);
	UnloadTexture(mainG_empty);
	UnloadTexture(mainD_empty);
	UnloadTexture(mainG);
	UnloadTexture(mainD);
	UnloadTexture(car_destroyed);
	UnloadTexture(car_new);
	UnloadTexture(lean_destroyed);
	UnloadTexture(lean_launched);

	StopMusicStream(soincSong);
}

// Function managing the screen
void UpdateScreen() {
	// Play the song (loaded previously)
	//PlayMusicStream(soincSong);

	// Draw the corresponding screen
	switch(game.gameScreen) {
		case MENU: {
			ResetPhysics();
			MenuDraw();
		} break;
		case LEVEL_1: {
			ResetPhysics();
			LevelOneDraw(&player);
		} break;
		case LEVEL_2: {
			ResetPhysics();
			LevelTwoDraw(&player);
		} break;
		case LEVEL_3: {
			ResetPhysics();
			//LevelThreeDraw(player_adress);
		} break;
		case GAME_OVER: {
			ResetPhysics();
			GameoverDraw();
		} break;
		case SUCCESS: {
			ResetPhysics();
			SuccessDraw();
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
