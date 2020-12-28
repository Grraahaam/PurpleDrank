#include <stdio.h>
#include <raylib.h>
#include <unistd.h>
#include <pthread.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "level5.h"

//pthread_t thread_id;

Enemy goblin;
Asset goblin_flip, skate, left_hand, right_hand;
//Asset lean, gob_dying;
int attaque;

//Texture2D portal;

void LevelFiveRead(Player *player, Enemy *goblin) {
    
    gp_readPlayer(player);

    
    /*
    // Check if assets is still in the screen, otherwise reset their position
    switch(gp_isOutScreen((player, lean, gob, goblean))) {
        
        // Player
        case 0: {
            
        } break;
        
        // Lean
        case 1: {
            
        } break;
        
        // Gob
        case 2: {
            
        } break;
        
        // Goblean
        case 3: {
            
        } break;
        
        default: {
            // Nothing for now
        } break;
    }
    */
    
    if(goblin->asset.direction == LEFT) {
                    
            goblin->asset.position.x -= goblin->asset.speed;
            
            // If goblean reached the middle and is facing left
            if(goblin->asset.position.x < (screenWidth / 2.5 + goblin->asset.swidth)) {
            	
            // Invert direction
               goblin->asset.direction = RIGHT;
                            	
            	switch(attaque) {
            		
            		case 1: {
				skate.disabled = false;
				skate.position.x = goblin->asset.position.x;
				goblin_flip.disabled = false;
				goblin->asset.disabled = true;
            		} break;
            		case 2: {
            			left_hand.position.x = goblin->asset.position.x + 60;
            			right_hand.position.x = goblin->asset.position.x + 20;
            			left_hand.position.y = goblin->asset.position.y - 20;
            			right_hand.position.y = goblin->asset.position.y - 20;
            			left_hand.disabled = false;
            			right_hand.disabled = false;
            		} break;
            		default: {
            		 	// Nothing for now
            		}
            	}
            }
            			
                    
  	} else {
	            
	    goblin->asset.position.x += goblin->asset.speed*3;
	    
	    // If goblean reached the right side and is facing right (right - goblean width - margin)
	    if(goblin->asset.position.x > (screenWidth - goblin->asset.swidth + 20)) {
	    
	    		    
	        // Invert direction
	        goblin->asset.direction = LEFT;
	    
	    	switch(attaque) {
    		
	    		case 1: {
				goblin_flip.disabled = true;
	        		goblin->asset.disabled = false;
	        		attaque = 2;
	    		} break;
	    		case 2: {
	    			attaque = 1;		    			
	    		} break;
	    		default: {
	    		 	// Nothing for now
	    		}
    		}
	    }
	}
    
    if(skate.position.x > 0) skate.position.x -= 4;
    if(skate.position.x < 0) skate.disabled = true;
    if(left_hand.position.x > 0) {
    	left_hand.position.x -= 4;
    	left_hand.position.y += 1;
    }
    if(left_hand.position.x < 0) left_hand.disabled = true;
    if(right_hand.position.x > 0) {
    	right_hand.position.x -= 4;
    	right_hand.position.y += 1;
    }
    if(right_hand.position.x < 0) right_hand.disabled = true;
}
    
void LevelFiveInit(Player *player) {
    
    ResetPhysics();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    attaque = 1;
    
    PrintDebug(TextFormat("Initializing: %s", screenNames[game.gameScreen]));
    
    gp_resetNotification();
    
    //TODO: PUT THIS INTO A GLOBAL FUNCTION TO Initialize BASE PROPERTIES FOR PLAYER AT EACH LEVEL
    //TODO: IMPLEMENT THREAD FUNCTION TO TIMEOUT LEVEL LOADING AND DEATH TIMEOUT
    player->slip = false;
    
    /** CUSTOM ****************************************************************************/
    
    // Initialize Lean object (to throw)
    /*lean = res.items.lean;
    lean.scale = 0.15;
    lean.speed = 5;
    lean.position = (Vector2){0,0};
    lean.disabled = true;
    
    // Initialize gob asset
    gob.asset = res.items.gob_walking;
    gob.asset.speed = 1;
    gob.amount = 3;
    
    gob.asset.position = (Vector2){
        .x = screenWidth + gob.asset.swidth + 20,
        .y = 235
    };
    
    // Initialize dying gob
    gob_dying = res.items.gob_dying;
    gob_dying.disabled = true;*/
    
    // Initialize Goblean asset
    goblin.asset = res.items.goblean_skate;
    goblin.asset.speed = 1;
    goblin.asset.direction = LEFT;
    goblin.lives = 3;
    goblin.asset.disabled = false;
    
    goblin.asset.position = (Vector2){
        .x = screenWidth + goblin.asset.swidth + 20,
        .y = 300
    };
    
    goblin_flip = res.items.goblean_flip;
    goblin_flip.disabled = true;
    
    skate = res.items.skate_flip;
    skate.disabled = true;
    skate.position = (Vector2){
        .x = goblin.asset.position.x,
        .y = goblin.asset.position.y+50
    };
    
    left_hand = res.items.left_hand;
    left_hand.disabled = true;
    left_hand.position = (Vector2){
        .x = goblin.asset.position.x + 60,
        .y = goblin.asset.position.y - 20
    };
    
    right_hand = res.items.right_hand;
    right_hand.disabled = true;
    right_hand.position = (Vector2){
        .x = goblin.asset.position.x + 20,
        .y = goblin.asset.position.y - 20
    };
    
    // Create floor and walls rectangle physics body
    PhysicsBody body_floor = CreatePhysicsBodyRectangle(
        (Vector2){screenWidth/2, 380},
        screenWidth, 10, 10);
    
    PhysicsBody body_wall_left = CreatePhysicsBodyRectangle(
        (Vector2){-5, screenHeight / 2},
        10, screenHeight*2, 10);
    
    PhysicsBody body_wall_right = CreatePhysicsBodyRectangle(
        (Vector2){screenWidth + 5, screenHeight / 2},
        10, screenHeight*2, 10);
    
    // Disable dynamics to floor and walls physics bodies
    body_floor->enabled = false;
    body_wall_left->enabled = false;
    body_wall_right->enabled = false;
    
    /************************************************************************************/
    
    // Set default position
    player->asset.position = game.levelPos.level_5;
    
    gp_initializeBody(player);
}

void LevelFiveDraw(Player *player, ScreenFX *screenFx) {

     // Initialize the level only if not loaded
    if(game.screenLoaded != LEVEL_5) {
        
         LevelFiveInit(player);
        PrintDebug(TextFormat("Drawing: %s", screenNames[game.gameScreen]));
        
        player->can_move = true;
        
        game.screenLoaded = LEVEL_5;
        game.levelPassed = LEVEL_4;
        
        // Set default fade properties
        gp_resetFx(screenFx);
        
        res.items.portal.disabled = false;
        player->can_move = true;
    }
    
    // Read user input and interact
    LevelFiveRead(player, &goblin);
    
    RunPhysicsStep();
    BeginDrawing();

    // Draw level's background
    DrawTextureEx(res.backgrounds.level5, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
    
    /** CUSTOM ****************************************************************************/
    
    // Draw lean, gob and goblean
    //gp_drawAsset(&res.items.left_hand, (Vector2){500, 280}, 0.7);
    /*gp_drawAsset(&lean, lean.position, lean.scale);*/
    gp_drawAsset(&goblin.asset, goblin.asset.position, goblin.asset.scale);
    gp_drawAsset(&right_hand, right_hand.position, right_hand.scale);
    gp_drawAsset(&left_hand, left_hand.position, left_hand.scale);
    gp_drawAsset(&skate, skate.position, skate.scale);
    gp_drawAsset(&goblin_flip, goblin.asset.position, goblin_flip.scale);
    
    gp_drawText(
        (char*)TextFormat("posX : %f", skate.position.x),
        res.fonts.pixellari, (Vector2){0, 70},
        20, CENTER_X, DARKGRAY
    );

    /**************************************************************************************/
    
    // Player's info board (lifes, lean, level) and notification
    gp_drawBoard(player);
    
    // Draw player
    gp_drawAsset(&player->asset, player->asset.position, player->asset.scale);
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);
    
    EndDrawing();
}
