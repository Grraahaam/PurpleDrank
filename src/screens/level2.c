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
#include "../screens/level2.h"

float vitesse = VELOCITY*0.4;
bool boule = true;
bool right = true;
bool victory = false;
bool dead = false;


bool LevelTwoEnd(Rectangle *wall_right, Rectangle *rect_solin) { //Return true if leveld ended
	return CheckCollisionRecs(*rect_solin, *wall_right); 
}

void LevelTwoCheck_Event(Player *player_Struct, Rectangle *wall_right, Rectangle *rect_solin) {
	
	if ( LevelTwoEnd(wall_right, rect_solin) ) {
        
		victory = true;	
	}
    // Check if player is dead
	if(player_Struct->health_point <= 0) {
		// Reset 3 lives and show Game over screen
		player_Struct->health_point = 3;
		dead = true;
		game.gameScreen = GAME_OVER;
	}
}

void LevelTwoRead(Player *player_Struct, PhysicsBody *body, Rectangle *wall_right, Rectangle *rect_solin) {
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
		if (boule && right){
            imgPlayer = soincReverse;
        }
        right = false;
    }

	// Vertical movement input checking if player physics body is grounded
    if (IsKeyDown(KEY_UP) && (*body)->isGrounded) {
		(*body)->velocity.y = -VELOCITY*3;
	} 
	LevelTwoCheck_Event(player_Struct, wall_right, rect_solin);
}

void LevelTwoDraw(Player *player_Struct) {

    PrintDebug("Drawing Level 2");
	
 	SetConfigFlags(FLAG_MSAA_4X_HINT);

     // Re/set default player image
	imgPlayer = soincPlayer;

	//Re/set the victory switch (otherwise when gameover + retry = infinite loop)
	victory = false;
	dead = false;

	// Create floor and walls rectangle physics body
    PhysicsBody floor1 = CreatePhysicsBodyRectangle((Vector2){ 35, 360 }, 70, 190, 10);
    PhysicsBody floor2 = CreatePhysicsBodyRectangle((Vector2){ 115, 405 }, 90, 50, 10);
    PhysicsBody floor3 = CreatePhysicsBodyRectangle((Vector2){ 218, 390 }, 116, 70, 10);
    PhysicsBody floor4 = CreatePhysicsBodyRectangle((Vector2){ 341, 402 }, 130, 50, 10);
    PhysicsBody floor5 = CreatePhysicsBodyPolygon((Vector2){ 462, 420 }, 70.0f, 5, 10); 
    PhysicsBody floor6 = CreatePhysicsBodyRectangle((Vector2){ 520, 388.5 }, 70, 70, 10); 
    PhysicsBody floor7 = CreatePhysicsBodyPolygon((Vector2){ 552, 427 }, 75.0f, 8, 10);   
    PhysicsBody platform11 = CreatePhysicsBodyRectangle((Vector2){ 225 , 55 }, 150, 110, 10);
    PhysicsBody platform12 = CreatePhysicsBodyRectangle((Vector2){ 335, 43 }, 70, 86, 10);
    PhysicsBody platform13 = CreatePhysicsBodyPolygon((Vector2){ 370, 10 }, 75.0f, 4, 10);   
    PhysicsBody platform2 = CreatePhysicsBodyRectangle((Vector2){ 705, 245 }, 180, 350, 10);
    PhysicsBody wall_left = CreatePhysicsBodyRectangle((Vector2){ 0, screenHeight/2 }, 10, screenHeight*2, 10);
 
	Rectangle wall_right = { 800, 200, 10, 200};

	// Disable dynamics to floor and walls physics bodies
    floor1->enabled = false;
    floor2->enabled = false;
    floor3->enabled = false;
    floor4->enabled = false;
    floor5->enabled = false;
    floor6->enabled = false;
    floor7->enabled = false;
    platform11->enabled = false;
    platform12->enabled = false;
    platform13->enabled = false;
    platform2->enabled = false;
    wall_left->enabled = false;
    
	PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){40, screenHeight/2 }, 50, 60, 10);
    body->freezeOrient = true;    // Constrain body rotation to avoid little collision torque amounts
	
	while (!victory && !dead && !WindowShouldClose()) {   // Detect window close button, ESC key or victory
		
		RunPhysicsStep();
		Rectangle rect_solin = { body -> position.x - 30, body -> position.y - 30, 60, 60 };
		
		LevelTwoRead(player_Struct, &body, &wall_right, &rect_solin);
		
		BeginDrawing();

		DrawTextureEx(background_lvl2, (Vector2){0, 0}, 0.0f, 0.85f, WHITE); 
        DrawTextureEx(imgPlayer, (Vector2){ body -> position.x - 40, body -> position.y - 30}, 0.0f, 0.15f, WHITE);
		DrawTextureEx(solin_head, (Vector2){10, 20}, 0.0f, 0.25f, WHITE);
		DrawText(TextFormat("%f", body->position.x), 10, 85, 30, WHITE);
		DrawText(TextFormat("%f", body->position.y), 10, 55, 30, RED);
		DrawText(TextFormat("%d", player_Struct->health_point), 90, 35, 30, WHITE);
        DrawFPS(screenWidth - 90, screenHeight - 30);
		EndDrawing();
	}
}





