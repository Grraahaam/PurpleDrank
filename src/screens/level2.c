#include <stdio.h>
#include <raylib.h>
/*
#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#define PHYSAC_DEBUG
*/
#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../screens/level2.h"



void LevelTwoDraw(Player *player_Struct) {

 	SetConfigFlags(FLAG_MSAA_4X_HINT);
	
	while (!WindowShouldClose() ) {   // Detect window close button, ESC key or victory
	
		RunPhysicsStep();
		
		BeginDrawing();
		ClearBackground(BLACK);

		DrawFPS(screenWidth - 90, screenHeight - 30);

		DrawTextureEx(background_lvl2, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
       
		DrawText(TextFormat("%d", player_Struct->health_point), 90, 35, 30, WHITE);
		
		EndDrawing();
		
	}
}




