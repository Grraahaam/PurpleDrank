
#include <stdio.h>
#include <raylib.h>

#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#define VELOCITY 0.5f
#include "../lib/raymath.h"
#include "../lib/physac.h"

#include "../globals.h"
#include "../lib/defines.c"


//Initialize and Default settings 
//Needs to be defined in the main, but seems we can't extern a struct in C
Player player = { (Vector2){20,300},10,true,true, 5};

float vitesse = VELOCITY*0.4;
bool boule = true;
bool right = true;
bool col_trou = false;
bool col_wall_right = false;
bool victory = false;
int fall = 0;
int nb_lives = 5;

bool Fallen_Hole(Rectangle trou, Rectangle rect_solin) {  // Return true si tombé dans le trou
	return CheckCollisionRecs(rect_solin, trou);

}

bool End_Level(Rectangle wall_right, Rectangle rect_solin) { //Return true if leveld ended
	return CheckCollisionRecs(rect_solin, wall_right); 
}

void Check_Event(PhysicsBody body, Rectangle trou, Rectangle wall_right, Rectangle rect_solin) {
	if ( Fallen_Hole(trou, rect_solin) ) {
		body->position.x = 80;
		body->position.y = screenHeight/2;
		fall++;
		nb_lives--;
	}
	else if ( End_Level(wall_right, rect_solin) ) {
		victory = true;	
	}
}


void LevelOneRead(PhysicsBody body, Rectangle trou, Rectangle wall_right, Rectangle rect_solin)
{
	// Horizontal movement input
        if (IsKeyDown(KEY_RIGHT)) {
            body->velocity.x = vitesse;
            if (boule && !right){
                imgPlayer = soincPlayer;
            }
            right = true;
        }        
        else if (IsKeyDown(KEY_LEFT)) {
            body->velocity.x = -vitesse;
			if (boule && right){
                imgPlayer = soincReverse;
            }
            right = false;
        }

		// Vertical movement input checking if player physics body is grounded
        //if (IsKeyDown(KEY_UP) && body->isGrounded) {
		if (IsKeyDown(KEY_UP)) {
			body->velocity.y = -VELOCITY*1.2;
		} 
		Check_Event(body, trou, wall_right, rect_solin);
}


void LevelOneDraw() {

 	SetConfigFlags(FLAG_MSAA_4X_HINT);
	
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
		Rectangle rect_solin = { body -> position.x - 30, body -> position.y - 30, 60, 60 };
		
		LevelOneRead(body,trou,wall_right, rect_solin);

		BeginDrawing();
		ClearBackground(BLACK);

		DrawFPS(screenWidth - 90, screenHeight - 30);

		DrawTextureEx(background_lvl1, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
        DrawTextureEx(imgPlayer, (Vector2){ body -> position.x - 40, body -> position.y - 30}, 0.0f, 0.15f, WHITE);
		DrawTextureEx(solin_head, (Vector2){10, 20}, 0.0f, 0.25f, WHITE);
		DrawText(TextFormat("%f", body->position.y), 10, 85, 30, WHITE);
		DrawText(TextFormat("%f", body->position.x), 10, 55, 30, RED);
		DrawText(TextFormat("%d", player.health_point), 90, 35, 30, WHITE);
		
		EndDrawing();
	}
}




