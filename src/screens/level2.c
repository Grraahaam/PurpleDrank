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
bool boule = true, right = true, victory = false, dead = false, canMove = true, desactivate = false, l1 = false, l2 = false, l3 = false;
int time_spikes = 0, time_tp = 0;


bool Fallen_Spikes(Rectangle *rect_piques, Rectangle *rect_solin) {
	if(CheckCollisionRecs(*rect_solin, *rect_piques)){
		if(time_spikes == 0){
			imgPlayer = solinDead;
			canMove = false;
			time_spikes++;
		}
		else{
			time_spikes++;
		}
	}
	if(time_spikes>30){
	time_spikes = 0;
	return true;
	}
	else return false;
}

void reset_lean(Player *player_Struct){
        lean1 = lean1_available;
        lean2 = lean2_available;
        lean3 = lean3_available;
        l1 = false;
        l2 = false;
        l3 = false;
	player_Struct->nb_lean = 0;
}	
void saut_tremplin(PhysicsBody *body, Rectangle *rect_solin, Rectangle *rect_tremplin){
	if(CheckCollisionRecs(*rect_solin, *rect_tremplin)){
	(*body)->velocity.y = -VELOCITY*5.5;
	(*body)->velocity.x = +VELOCITY*2;
	}
}

bool use_teleporter(Player *player_Struct, Rectangle *rect_solin, Rectangle *rect_tp){
	if (CheckCollisionRecs(*rect_solin, *rect_tp) && player_Struct->nb_lean == 3){
		teleportation = teleportation_OK;
		return true;
	}
	else if(CheckCollisionRecs(*rect_solin, *rect_tp) && player_Struct->nb_lean < 3){
		desactivate = true;
		return false;
	}
}

bool LevelTwoEnd(Player *player_Struct, Rectangle *rect_tp, Rectangle *rect_solin) { //Return true if leveld ended
	return use_teleporter(player_Struct, rect_solin, rect_tp); 
}

bool GetLean(Rectangle *rect_lean1, Rectangle *rect_lean2, Rectangle *rect_lean3 , Rectangle *rect_solin){
	bool success = false;
	if(!l1 && CheckCollisionRecs(*rect_solin, *rect_lean1)){
	lean1 = lean1_taken;
	l1 = true;
	success = true;
	}
	else if(!l2 && CheckCollisionRecs(*rect_solin, *rect_lean2)){
	lean2 = lean2_taken;
	l2 = true;
	success = true;
	}
	else if(!l3 && CheckCollisionRecs(*rect_solin, *rect_lean3)){
	lean3 = lean3_taken;
	l3 = true;
	success = true;
	}	
	return success;
} 

void LevelTwoCheck_Event(Player *player_Struct, PhysicsBody *body, Rectangle *rect_piques, Rectangle *rect_tp, Rectangle *rect_solin, Rectangle *rect_lean1, Rectangle *rect_lean2, Rectangle *rect_lean3) {
	
	if ( Fallen_Spikes(rect_piques, rect_solin) ) {
		(*body)->position.x = 40;
		(*body)->position.y = screenHeight/2;
		player_Struct->health_point -= 1 ;
		imgPlayer = solinPlayer;
		canMove = true;
		reset_lean(player_Struct);

		// Check if player is dead
		if(player_Struct->health_point <= 0) {
			// Reset 3 lives and show Game over screen
			player_Struct->health_point = 3;
			dead = true;
			game.gameScreen = GAME_OVER;
		}
	}
	if(GetLean(rect_lean1, rect_lean2, rect_lean3, rect_solin)) {
	player_Struct->nb_lean++;
	}
	else if ( LevelTwoEnd(player_Struct, rect_tp, rect_solin) ) {
		victory = true;
		game.gameScreen = SUCCESS;
	}
}

void LevelTwoRead(Player *player_Struct, PhysicsBody *body, Rectangle *rect_piques, Rectangle *rect_tp, Rectangle *rect_solin, Rectangle *rect_lean1, Rectangle *rect_lean2, Rectangle *rect_lean3, Rectangle *rect_tremplin) {
	// Horizontal movement input
    if (IsKeyDown(KEY_RIGHT) && canMove) {
        (*body)->velocity.x = vitesse;
        if (boule && !right){
            imgPlayer = solinPlayer;
        }
        right = true;
    }        
    else if (IsKeyDown(KEY_LEFT) && canMove) {
        (*body)->velocity.x = -vitesse;
		if (boule && right){
            imgPlayer = solinReverse;
        }
        right = false;
    }

	// Vertical movement input checking if player physics body is grounded
    if (IsKeyDown(KEY_UP) && (*body)->isGrounded && canMove) {
		(*body)->velocity.y = -VELOCITY*3;
	}
	
    if (IsKeyDown(KEY_SPACE)) saut_tremplin(body, rect_solin, rect_tremplin);
     
	LevelTwoCheck_Event(player_Struct, body, rect_piques, rect_tp, rect_solin, rect_lean1, rect_lean2, rect_lean3);
}

void LevelTwoDraw(Player *player_Struct) {

    PrintDebug("Drawing Level 2");
	
 	SetConfigFlags(FLAG_MSAA_4X_HINT);

     // Re/set default player image
	imgPlayer = solinPlayer;
	
	lean1 = lean1_available;
	lean2 = lean1_available;
	lean3 = lean1_available;
	teleportation = teleportation_NOK;

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
    PhysicsBody wall_right = CreatePhysicsBodyRectangle((Vector2){ screenWidth, screenHeight/2 }, 10, screenHeight*2, 10);
    
    Rectangle rect_lean1 = { 650, 30, 25, 40 };
    Rectangle rect_lean2 = { 700, 30, 25, 40 };
    Rectangle rect_lean3 = { 750, 30, 25, 40 };
    Rectangle rect_tremplin = {310, 365, 10, 10};
    Rectangle rect_piques = { 70, 370, 80, 20 };
    Rectangle rect_tp = { 600, 300, 10, 70};

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
    wall_right->enabled = false;
    
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){40, screenHeight/2 }, 50, 60, 10);
    body->freezeOrient = true;    // Constrain body rotation to avoid little collision torque amounts
	
	while (!victory && !dead && !WindowShouldClose()) {   // Detect window close button, ESC key or victory
		
		RunPhysicsStep();
		Rectangle rect_solin = { body -> position.x - 30, body -> position.y - 30, 60, 60 };
		
		LevelTwoRead(player_Struct, &body, &rect_piques, &rect_tp, &rect_solin, &rect_lean1, &rect_lean2, &rect_lean3, &rect_tremplin);
		
		BeginDrawing();

		DrawTextureEx(background_lvl2, (Vector2){0, 0}, 0.0f, 0.85f, WHITE); 
        	DrawTextureEx(imgPlayer, (Vector2){ body -> position.x - 40, body -> position.y - 30}, 0.0f, 0.15f, WHITE);
		DrawTextureEx(solin_head, (Vector2){10, 20}, 0.0f, 0.25f, WHITE);
		DrawTextureEx(lean1, (Vector2){650, 30}, 0.0f, 0.08f, WHITE);
            	DrawTextureEx(lean2, (Vector2){700, 30}, 0.0f, 0.08f, WHITE);
            	DrawTextureEx(lean3, (Vector2){750, 30}, 0.0f, 0.08f, WHITE);
            	DrawTextureEx(spikes, (Vector2){70, 370}, 0.0f, 0.13f, WHITE);
            	DrawTextureEx(tremplin, (Vector2){300, 330}, 38.8f, 0.13f, WHITE);
            	DrawTextureEx(teleportation, (Vector2){555, 280}, 0.0f, 0.1f, WHITE);
            	DrawTextureEx(inventory_lean, (Vector2){30, 60}, 0.0f, 0.08f, WHITE);
            	DrawText(TextFormat("%d", player_Struct->nb_lean), 90, 70, 30, WHITE);
            	//DrawText(TextFormat("%f", body->position.x), 10, 85, 30, WHITE);
		//DrawText(TextFormat("%f", body->position.y), 10, 55, 30, RED);
		DrawText(TextFormat("%d", player_Struct->health_point), 90, 25, 30, WHITE);
		if (desactivate){
        	DrawText("Vous n'avez pas assez de lean pour activer le téléporteur!", 100, 100, 20, RED);
		}
        	DrawFPS(screenWidth - 90, screenHeight - 30);
		EndDrawing();
	}
}





