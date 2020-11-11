
#include <stdio.h>
#include <raylib.h>

#include "../lib/raymath.h"
#include "../lib/physac.h"

#include "../globals.h"
#include "../lib/defines.c"

//Initialize and Default settings 
//Needs to be defined in the main, but seems we can't extern a struct in C
Player player = { (Vector2){20,300},10,true,true, 5};

// Maybe load PhysicsBody in Struct decor per level (in the main)
/*
Decor decor_level1 = {   
	CreatePhysicsBodyRectangle((Vector2){ 190, 350 }, 445, 170, 10),
	CreatePhysicsBodyRectangle((Vector2){ 535, 340 }, 100, 70, 10),
	CreatePhysicsBodyRectangle((Vector2){ 740, 360 }, 150, 150, 10),
	CreatePhysicsBodyRectangle((Vector2){ -5, screenHeight/2 }, 10, screenHeight, 10),
	CreatePhysicsBodyRectangle((Vector2){ screenWidth + 5, screenHeight/2 }, 10, screenHeight, 10)
};
*/

void LevelOneDraw() {
	BeginDrawing();

	ClearBackground(BLACK);

	DrawFPS(screenWidth - 90, screenHeight - 30);

	DrawTextureEx(background_lvl1, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);

	//Next line doesn't work, 
	//DrawTextureEx(soincPlayer, (Vector2){ body -> position.x - 40, body -> position.y - 30}, 0.0f, 0.15f, WHITE);
	//error message : 
	//error: dereferencing pointer to incomplete type ‘struct PhysicsBodyData’

	DrawTextureEx(solin_head, (Vector2){10, 20}, 0.0f, 0.25f, WHITE);
	DrawText(TextFormat("%d", player.health_point), 90, 25, 30, WHITE);

	EndDrawing();
}

/* 
void LevelOneRead()
{
	
    if (IsKeyDown(KEY_RIGHT)) {
        body->velocity.x = vitesse;
        if (boule && !right){
        	solin = soincPlayer;
        }
        right = true;
    }        
    else if (IsKeyDown(KEY_LEFT)) {
        body->velocity.x = -vitesse;
        if (boule && right){
        	solin = soincReverse;
        }
        right = false;
    }
        
    col_trou = CheckCollisionRecs(rect_solin, trou);
	
	if(col_trou){
		body->position.x = 80;
		body->position.y = screenHeight/2;
		fall++;
		nb_lives--;
	}
	
	col_wall_right = CheckCollisionRecs(rect_solin, wall_right);
        
    if(col_wall_right) victory = true;
        

    // Vertical movement input checking if player physics body is grounded
    if (IsKeyDown(KEY_UP) && body->isGrounded) body->velocity.y = -VELOCITY*3; 
}
*/


