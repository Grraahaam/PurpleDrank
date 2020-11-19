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
#include "../screens/level3.h"

float vitesse = VELOCITY*0.4;
bool boule = true, right = true, victory = false, dead = false, canMove = false, desactivate = false, lean_activate = false, lean_right = false, lean_left = false, destroy = true, start = true, lancer = false, lance_mainD = true, lance_mainG = true;
int time_spikes = 0, time_start = 0, time_killed = 0, time_kill = 0, car_killed = 0, goblean_hurt = 0;
float leanX, leanY, mainGx, mainGy, mainDx, mainDy;


/*bool Fallen_Spikes(Rectangle *rect_piques, Rectangle *rect_solin) {
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
}*/

void LeanAvance() { 
	if(lean_right) leanX += 5.0f;
	if(lean_left) leanX -= 5.0f;
}


void LaunchLean(Player *player_Struct, PhysicsBody *body) {
	if(player_Struct->nb_lean > 0 && destroy && !lean_activate){
		leanX = (*body)->position.x;
       	leanY = (*body)->position.y;
        	lean = lean_launched;
        	player_Struct->nb_lean --;
        	lean_activate = true;
        	if(right) lean_right = true;
        	else lean_left = true;
	}
}



void LevelThreeCheck_Event(Player *player_Struct, PhysicsBody *body, Rectangle *rect_goblean, Rectangle *rect_skate, Rectangle *rect_solin, Rectangle *rect_lean, Rectangle *rect_car_top, Rectangle *rect_car_left, Rectangle *rect_car_right) {
	
	/*if ( Fallen_Spikes(rect_piques, rect_solin) ) {
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
	else if ( LevelTwoEnd(player_Struct, rect_tp, rect_solin) ) {
		victory = true;
		game.gameScreen = SUCCESS;
	}*/
}

void LevelThreeRead(Player *player_Struct, PhysicsBody *body, Rectangle *rect_goblean, Rectangle *rect_skate, Rectangle *rect_solin, Rectangle *rect_lean, Rectangle *rect_car_top, Rectangle *rect_car_left, Rectangle *rect_car_right) {
	// Horizontal movement input
    if (IsKeyDown(KEY_RIGHT) && canMove) {
        (*body)->velocity.x = vitesse;
        if (!boule && !right){
            imgPlayer = solinPlayer;
        }
        right = true;
    }        
    else if (IsKeyDown(KEY_LEFT) && canMove) {
        (*body)->velocity.x = -vitesse;
	if (!boule && right){
            imgPlayer = solinReverse;
        }
        right = false;
    }
    else if (IsKeyPressed(KEY_DOWN) && !boule) {
    	imgPlayer = solinBall;
        vitesse = vitesse*3;
        boule = true;
    }
    
    else if (IsKeyPressed(KEY_DOWN) && boule && vitesse > 0.5 ) {
        if(right) imgPlayer = solinPlayer;
        else imgPlayer = solinReverse;
        vitesse = vitesse/3;
        boule = false;
    }

	// Vertical movement input checking if player physics body is grounded
    if (IsKeyDown(KEY_UP) && (*body)->isGrounded && canMove) {
		(*body)->velocity.y = -VELOCITY*3;
    }
	
     
	//LevelTwoCheck_Event(player_Struct, body, rect_piques, rect_tp, rect_solin, rect_lean1, rect_lean2, rect_lean3);
}

void LevelThreeDraw(Player *player_Struct) {

    PrintDebug("Drawing Level 2");
	
 	SetConfigFlags(FLAG_MSAA_4X_HINT);

     // Re/set default player image
	imgPlayer = solinPlayer;
	goblean_ennemy = gobleanAlive;
	goblean_mainG = mainG_empty;
	goblean_mainD = mainD_empty;
	lean = lean_destroyed;
	teleportation = teleportation_OK;
	car_ennemy = car_new;

	//Re/set the victory switch (otherwise when gameover + retry = infinite loop)
	victory = false;
	dead = false;

	// Create floor and walls rectangle physics body
    PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2){ 435, 380 }, 730, 190, 10);
    PhysicsBody platform = CreatePhysicsBodyRectangle((Vector2){ 35, 330 }, 70, 240, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ -5, screenHeight/2 }, 10, screenHeight*2, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth + 5, screenHeight/2 }, 10, screenHeight*2, 10);
    PhysicsBody car = CreatePhysicsBodyRectangle((Vector2){ 1000, 260}, 140, 50, 10);
    PhysicsBody goblean = CreatePhysicsBodyRectangle((Vector2){ 1000, 180}, 120, 210, 10);
    

	// Disable dynamics to floor and walls physics bodies
    floor->enabled = false;
    platform->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;
    car-> enabled = false;
    goblean -> enabled = false;

    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){ 40, screenHeight/2 }, 50, 60, 1);
    body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
	
	while (!victory && !dead && !WindowShouldClose()) {   // Detect window close button, ESC key or victory
		
		RunPhysicsStep();
		LeanAvance();
		Rectangle rect_solin = { body -> position.x - 30, body -> position.y - 30, 60, 60 };
		Rectangle rect_lean = { leanX, leanY - 20, 25, 40 };
        
		Rectangle rect_car_top = { car-> position.x - 60, car-> position.y - 30, 120, 10 };  
		Rectangle rect_car_left = { car-> position.x - 80, car-> position.y, 10, 20 };      
		Rectangle rect_car_right = { car-> position.x + 70, car-> position.y, 10, 20 }; 
		
		Rectangle rect_goblean = { goblean-> position.x - 60, goblean-> position.y - 100, 120, 160 };
		Rectangle rect_skate = { goblean-> position.x - 95, goblean-> position.y + 62, 180, 40 };
		
		LevelThreeRead(player_Struct, &body, &rect_goblean, &rect_skate, &rect_solin, &rect_lean, &rect_car_top, &rect_car_left, &rect_car_right);
		
		BeginDrawing();

		DrawTextureEx(background_lvl3, (Vector2){0, 0}, 0.0f, 0.85f, WHITE); 
        	DrawTextureEx(imgPlayer, (Vector2){ body -> position.x - 40, body -> position.y - 30}, 0.0f, 0.15f, WHITE);
		DrawTextureEx(solin_head, (Vector2){10, 20}, 0.0f, 0.25f, WHITE);
            	DrawTextureEx(teleportation, (Vector2){5, 125}, 0.0f, 0.1f, WHITE);
            	DrawTextureEx(inventory_lean, (Vector2){30, 60}, 0.0f, 0.08f, WHITE);
            	DrawText(TextFormat("%d", player_Struct->nb_lean), 90, 70, 30, WHITE);
            	DrawTextureEx(lean, (Vector2){leanX, leanY - 20}, 0.0f, 0.08f, WHITE); 
            	DrawTextureEx(car_ennemy, (Vector2){ car-> position.x - 75, car-> position.y - 40 }, 0.0f, 0.1f, WHITE ); 
            	DrawTextureEx(goblean_ennemy, (Vector2){ goblean-> position.x - 115, goblean-> position.y - 110 }, 0.0f, 0.2f, WHITE );   
            	//DrawText(TextFormat("%f", body->position.x), 10, 85, 30, WHITE);
		//DrawText(TextFormat("%f", body->position.y), 10, 55, 30, RED);
		DrawText(TextFormat("%d", player_Struct->health_point), 90, 25, 30, WHITE);
        	DrawFPS(screenWidth - 90, screenHeight - 30);
		EndDrawing();
	}
}





