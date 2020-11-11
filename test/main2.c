
// This is the main program file, that is compiled to get the stable executable

// Include Raylib library
#include "raylib.h"

#include "../lib/raylib/physac.h"
// Include the game libraries
#include "globals.h" // Defines the variable types, structs and enums
#include "screens/menu_screen.h"

// Function declarations
void UpdateScreen();
void LoadRessources();
void UnloadRessources();
void RunLevel();



int main() {

	InitPhysics();
	// Initialization
	const int screenWidth = 800;
	const int screenHeight = 450;
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(screenWidth, screenHeight, "PurpleDrank");
	InitAudioDevice();
	LoadRessources();  

	SetTargetFPS(60);  

	// Main game loop (Detect window close button or ESC key)
	while (!WindowShouldClose()) {

		//GetCurrentLevel();
		RunLevel();

	}

	// De-Initialization
	ClosePhysics();       // Unitialize physics
	UnloadRessources();
	CloseAudioDevice();
	CloseWindow(); //Close window and OpenGL context
	

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

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

	game.gameScreen = LEVEL_1;

	// Draw the corresponding screen
	/*switch(game.gameScreen) {
		case MENU: {
			//MenuDraw();
		} break;
	}
	*/
}

void RunLevel() {
	
	const int screenWidth = 800;
	const int screenHeight = 450;
	Image image = LoadImage("../../res/solin.png");
    Image image3 = LoadImage("../../res/solin_reverse.png");
    Image back = LoadImage("../../res/Level1/ECRAN1V.png");
    Image solin_head = LoadImage("../../res/solin_head.png");
    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){ 80, screenHeight/2 }, 50, 60, 1);
   // body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts

	Rectangle trou = { 415, 700, 245, 10};
    Rectangle wall_right = { 800, 200, 10, 200};
    Texture2D solin = LoadTextureFromImage(image);
    Texture2D background = LoadTextureFromImage(back);
    Texture2D lives = LoadTextureFromImage(solin_head);

	

	  //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawFPS(screenWidth - 90, screenHeight - 30);
            
          //  DrawTextureEx(background, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
            
           // DrawTextureEx(solin, (Vector2){ body -> position.x - 40, body -> position.y - 30}, 0.0f, 0.15f, WHITE);
            
            DrawTextureEx(lives, (Vector2){10, 20}, 0.0f, 0.25f, WHITE);
            DrawText(TextFormat("%d", 5), 90, 25, 30, WHITE);

	EndDrawing();
}


