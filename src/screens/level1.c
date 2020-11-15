

#include <stdio.h>
#include <raylib.h>

#ifndef PHYSAC_IMPLEMENTATION
#define PHYSAC_IMPLEMENTATION
#endif

#ifndef PHYSAC_NO_THREADS
#define PHYSAC_NO_THREADS
#endif

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../screens/level1.h"

float vitesse = VELOCITY*0.4;
bool boule = true;
bool right = true;
bool victory = false;
int fall = 0;

// Return true si tombé dans le trou
bool Fallen_Hole(Rectangle *trou, Rectangle *rect_solin) {
	return CheckCollisionRecs(*rect_solin, *trou);
}

//Return true if level ended
bool End_Level(Rectangle *wall_right, Rectangle *rect_solin) {
	return CheckCollisionRecs(*rect_solin, *wall_right); 
}


void Check_Event(Player *player_Struct,PhysicsBody *body, Rectangle *trou, Rectangle *wall_right, Rectangle *rect_solin) {
	
	if ( Fallen_Hole(trou, rect_solin) ) {
		(*body)->position.x = 80;
		(*body)->position.y = screenHeight/2;
		fall++;
		player_Struct->health_point -= 1 ;

		// Check if player is dead
		if(player_Struct->health_point <= 0) {
			// Reset 3 lives and show Game over screen
			player_Struct->health_point = 3;
			victory = true;
			game.gameScreen = GAME_OVER;
		}
	}
	else if ( End_Level(wall_right, rect_solin) ) {
		victory = true;
		game.gameScreen = LEVEL_2;
	}
}

void LevelOneRead(Player *player_Struct,PhysicsBody *body, Rectangle *trou, Rectangle *wall_right, Rectangle *rect_solin)
{
	// Horizontal movement input
        if (IsKeyDown(KEY_RIGHT)) {
		(*body)->velocity.x = vitesse;
		if (boule && !right){
			imgPlayer = soincPlayer;
		}
		right = true;
        }        
        else if (IsKeyDown(KEY_LEFT)) {
		(*body)->velocity.x = -vitesse;
		if (boule && right) {
			imgPlayer = soincReverse;
		}
		right = false;
        }

	// Vertical movement input checking if player physics body is grounded
        //if (IsKeyDown(KEY_UP) && (*body)->isGrounded) {
	if (IsKeyDown(KEY_UP)) {
		(*body)->velocity.y = -VELOCITY*1.2;
	}

	Check_Event(player_Struct, body, trou, wall_right, rect_solin);
}

void LevelOneDraw(Player *player_Struct) {

	PrintDebug("Drawing Level 1");

 	SetConfigFlags(FLAG_MSAA_4X_HINT);

	// Re/set default player image
	imgPlayer = soincPlayer;

	//Re/set the victory switch (otherwise when gameover + retry = infinite loop)
	victory = false;

	// Create floor and walls rectangle physics body
	PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){ 100, screenHeight/2 }, 50, 60, 10);
	PhysicsBody floorLeft = CreatePhysicsBodyRectangle((Vector2){ 190, 350 }, 445, 170, 10);
	PhysicsBody platform = CreatePhysicsBodyRectangle((Vector2){ 535, 340 }, 100, 70, 10);
	PhysicsBody floorRight = CreatePhysicsBodyRectangle((Vector2){ 740, 360 }, 150, 150, 10);
	PhysicsBody wall_left = CreatePhysicsBodyRectangle((Vector2){ 0, screenHeight/2 }, 10, screenHeight, 10);
  	
	Rectangle trou = { 415, 700, 245, 10};
	Rectangle wall_right = { 800, 200, 10, 200};

	// Disable dynamics to floor and walls physics bodies
	floorLeft->enabled = false;
	floorRight->enabled=false;
	platform->enabled = false;
	wall_left->enabled = false;
	//wall_right->enabled = false;
	body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
	
	// Detect window close button, ESC key or victory
	while (!victory && !WindowShouldClose() ) {
		
		RunPhysicsStep();

		//printf("[DEBUG] FPS : %d\n", GetFPS());

		Rectangle rect_solin = { body -> position.x - 30, body -> position.y - 30, 60, 60 };
		
		LevelOneRead(player_Struct, &body, &trou, &wall_right, &rect_solin);
		
		BeginDrawing();
		ClearBackground(BLACK);

		DrawFPS(screenWidth - 90, screenHeight - 30);

		DrawTextureEx(background_lvl1, (Vector2){0, 0}, 0.0f, 0.85f, WHITE); 
        	DrawTextureEx(imgPlayer, (Vector2){ body -> position.x - 40, body -> position.y - 30}, 0.0f, 0.15f, WHITE);
		DrawTextureEx(solin_head, (Vector2){10, 20}, 0.0f, 0.25f, WHITE);
		DrawText(TextFormat("%f", body->position.x), 10, 85, 30, WHITE);
		DrawText(TextFormat("%f", body->position.y), 10, 55, 30, RED);
		//DrawText(TextFormat("%d", player_Struct->health_point), 10, 55, 30, RED);
		DrawText(TextFormat("%d", player_Struct->health_point), 90, 35, 30, WHITE);
		
		EndDrawing();
	}

	// Destroy physics bodies
	for (int i = 0; i < GetPhysicsBodiesCount(); i++) {
        	PhysicsBody body = GetPhysicsBody(i);
		DestroyPhysicsBody(body);
	}
}






