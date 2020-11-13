#include <stdio.h>
#include <raylib.h>
#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#define PHYSAC_DEBUG

#include "../lib/physac.h"
#include "../lib/defines.h"
#include "../globals.h"


//Initialize and Default settings 
//Needs to be defined in the main, but seems we can't extern a struct in C
//Player player = { (Vector2){20,300},10,true,true, 5};

bool victory = false;
void LevelTwoDraw(Player *player_Struct) {

 	SetConfigFlags(FLAG_MSAA_4X_HINT);
	//int player_health = player_Struct->health_point;

	// Create floor and walls rectangle physics body
	PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){ 100, screenHeight/2 }, 50, 60, 10);
	PhysicsBody floorLeft = CreatePhysicsBodyRectangle((Vector2){ 190, 350 }, 445, 170, 10);
    PhysicsBody platform = CreatePhysicsBodyRectangle((Vector2){ 535, 340 }, 100, 70, 10);
    PhysicsBody floorRight = CreatePhysicsBodyRectangle((Vector2){ 740, 360 }, 150, 150, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ 0, screenHeight/2 }, 10, screenHeight, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth + 5, screenHeight/2 }, 10, screenHeight, 10);
    Rectangle trou = { 415, 700, 245, 10};
    Rectangle wall_right = { 800, 200, 10, 200};

	// Disable dynamics to floor and walls physics bodies
    floorLeft->enabled = false;
    floorRight->enabled=false;
    platform->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;
	body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
	
	while (!WindowShouldClose() || victory) {   // Detect window close button, ESC key or victory
	
		RunPhysicsStep();
		//Rectangle rect_solin = { body -> position.x - 30, body -> position.y - 30, 60, 60 };
		
		//LevelTwoRead(player_Struct,body,trou,wall_right, rect_solin);

		BeginDrawing();
		ClearBackground(BLACK);

		DrawFPS(screenWidth - 90, screenHeight - 30);

		DrawTextureEx(background_lvl2, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
        DrawTextureEx(imgPlayer, (Vector2){ body -> position.x - 40, body -> position.y - 30}, 0.0f, 0.15f, WHITE);
		DrawTextureEx(solin_head, (Vector2){10, 20}, 0.0f, 0.25f, WHITE);
		DrawText(TextFormat("%f", body->position.y), 10, 85, 30, WHITE);
		DrawText(TextFormat("%f", body->position.x), 10, 55, 30, RED);
		
		//DrawText(TextFormat("%d", player_Struct->health_point), 10, 55, 30, RED);
		DrawText(TextFormat("%d", player_Struct->health_point), 90, 35, 30, WHITE);
		
		EndDrawing();
	}
	
}




