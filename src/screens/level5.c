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
Asset goblin_flip, skate, left_hand, right_hand, lean, damage;
int attaque;

//Texture2D portal;

bool l5_collisionAssets(Asset *ast1, Asset *ast2) {
    
    return CheckCollisionRecs(
        // Asset 1
        (Rectangle){
            .x = ast1->position.x, .y = ast1->position.y,
            .width = ast1->swidth, .height = ast1->sheight
        },
        // Asset 2
        (Rectangle){
            .x = ast2->position.x, .y = ast2->position.y,
            .width = ast2->swidth, .height = ast2->sheight
        });
}

Damage l5_collisionLean(Asset *lean, Enemy *goblin) {
        
    //Lean hit the Goblean
    if(l5_collisionAssets(lean, &goblin->asset)) {
    	damage.position.x = lean->position.x;
    	damage.position.y = lean->position.y;
    	damage.frame.current = 0;
    	damage.disabled = false;
    	damage.frame.animate = true;
    	return BOSS;
    }
    
    // Lean hit nothing
    else return NONE;
}

Damage l5_collisionPlayer(Player *player, Enemy *goblin, Asset *skate, Asset *left_hand, Asset *right_hand) {
    
    // Define player hitbox
    Rectangle player_hitbox = {
        .x = player->asset.position.x, .y = player->asset.position.y,
        .width = player->asset.swidth, .height = player->asset.sheight
    };
        
    // Player hit by goblean (GOBLEAN) or hit goblean
    if(CheckCollisionRecs(
        // Player
        player_hitbox,
        // Goblean
        (Rectangle){
            .x = goblin->asset.position.x - 60, .y = goblin->asset.position.y - 100,
            .width = 120, .height = 160
            
        })) {
        if(player->super) return BOSS;
        else return PLAYER;
        }
    
    // Player hit by goblean (SKATE)
    if(CheckCollisionRecs(
        // Player
        player_hitbox,
        // Skate
        (Rectangle){
            .x = skate->position.x - 40, .y = skate->position.y +- 20,
            .width = 80, .height = 40
            
        }) && !skate->disabled) {  skate->disabled = true; return PLAYER;}
    
   // Player hit by goblean (hands)
    if(CheckCollisionRecs(
        // Player
        player_hitbox,
        // Skate
        (Rectangle){
            .x = left_hand->position.x - 95, .y = left_hand->position.y + 62,
            .width = 50, .height = 40
            
        }) ||
        CheckCollisionRecs(
        // Player
        player_hitbox,
        // Skate
        (Rectangle){
            .x = right_hand->position.x - 95, .y = right_hand->position.y + 62,
            .width = 50, .height = 40
            
        })) return PLAYER;
}

void l5_throwLean(Player *player, Asset *lean) {

    // Only throw if lean left in inventory and no lean in the air
	if(player->lean > 0 && lean->disabled){

        // Decrement the lean amount and start throwing from player's position
        --player->lean;
        lean->position = player->asset.position;
        
        // Set the lean's inital size
        lean->swidth = lean->width * lean->scale;
        lean->sheight = lean->height * lean->scale;

        // Set the throwed lean's direction
        lean->direction = player->asset.direction == RIGHT ? RIGHT : LEFT;
        
        // Enable drawing
        lean->disabled = false;
    }
}

void l5_destroyLean(Asset *lean) {
    
    // Destroy throwed lean
    lean->disabled = true;
    lean->position = (Vector2){0,0};
}

void l5_readDamage(Damage dtype, Damage actor, Player *player, Asset *lean, Enemy *goblean) {
    
    // If gob or goblean hit by a throwed lean
    switch(dtype) {
        
        // TODO: PLAY SOUND EFFECT ON HIT + SPRITE HIT FRAME
        
        case BOSS: {
            
            // Decrement goblean's lives
            --goblean->lives;
            
            // If lean throwed, destroy iy
            if(actor == LEAN) l5_destroyLean(lean);
            
        } break;
        
        case PLAYER: {
            
            // Decrement player's lives
            --player->lives;
            
            // Replace player
            player->body->position = game.levelPos.level_5;
            
        } break;

        // Default action
        default : break;
    }
}

void l5_readCollisions(Player *player, Asset *lean, Enemy *goblin, Asset *skate, Asset *left_hand, Asset *right_hand) {
    
    // Read and process collision damage type
    l5_readDamage(l5_collisionLean(lean, goblin), LEAN, player, lean, goblin);
    l5_readDamage(l5_collisionPlayer(player, goblin, skate, left_hand, right_hand), PLAYER, player, lean, goblin);
}
        

void LevelFiveRead(Player *player, Enemy *goblin, Asset *lean, Asset *skate, Asset *left_hand, Asset *right_hand) {
    
    gp_readPlayer(player);

    l5_readCollisions(player, lean, goblin, skate, left_hand, right_hand);
    
    // If ended level
    if(goblin->lives <= 0) {
        
        goblin->asset.disabled = true;
        
        game.levelPassed = LEVEL_5;
        game.gameScreen = VICTORY;
    
    } else {
    
        // Throw lean
        if(IsKeyPressed(KEY_B)) l5_throwLean(player, lean);
    
    if(goblin->asset.direction == LEFT) {
                    
            goblin->asset.position.x -= goblin->asset.speed;
            
            // If goblean reached the middle and is facing left
            if(goblin->asset.position.x < (screenWidth / 2.5 + goblin->asset.swidth)) {
            	
            // Invert direction
               goblin->asset.direction = RIGHT;
                            	
            	switch(attaque) {
            		
            		case 1: {
				skate->disabled = false;
				skate->position.x = goblin->asset.position.x;
				goblin_flip.disabled = false;
				goblin->asset.disabled = true;
            		} break;
            		case 2: {
            			left_hand->position.x = goblin->asset.position.x + 60;
            			right_hand->position.x = goblin->asset.position.x + 20;
            			left_hand->position.y = goblin->asset.position.y - 20;
            			right_hand->position.y = goblin->asset.position.y - 20;
            			left_hand->disabled = false;
            			right_hand->disabled = false;
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
    
	    if(skate->position.x > 0) skate->position.x -= 4;
	    if(skate->position.x < 0) skate->disabled = true;
	    if(left_hand->position.x > 0) {
	    	left_hand->position.x -= 4;
	    	left_hand->position.y += 1;
	    }
	    if(left_hand->position.x < 0) left_hand->disabled = true;
	    if(right_hand->position.x > 0) {
	    	right_hand->position.x -= 4;
	    	right_hand->position.y += 1;
	    }
	    if(right_hand->position.x < 0) right_hand->disabled = true;
	    
	    // If there's a lean being throwed
	    if(!lean->disabled) {
	    	if(lean->direction == RIGHT) lean->position.x += lean->speed;
		else lean->position.x -= lean->speed;
	    }
	    
	    // Destroy lean if outside the screen
	    if(gp_isOutScreen(lean)) {
		PrintDebug("Lean destroyed");
		l5_destroyLean(lean);
	    }
	}
}
    
void LevelFiveInit(Player *player) {
    
    ResetPhysics();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    attaque = 1;
    player->lean = 20;
    player->lives = 10;
    
    PrintDebug(TextFormat("Initializing: %s", screenNames[game.gameScreen]));
    
    gp_resetNotification();
    
    //TODO: PUT THIS INTO A GLOBAL FUNCTION TO Initialize BASE PROPERTIES FOR PLAYER AT EACH LEVEL
    //TODO: IMPLEMENT THREAD FUNCTION TO TIMEOUT LEVEL LOADING AND DEATH TIMEOUT
    player->slip = false;
    
    /** CUSTOM ****************************************************************************/
    
    // Initialize Lean object (to throw)
    lean = res.items.lean;
    lean.scale = 0.15;
    lean.speed = 5;
    lean.position = (Vector2){0,0};
    lean.disabled = true;
    
    // Initialize Goblean asset
    goblin.asset = res.items.goblean_skate;
    goblin.asset.speed = 1;
    goblin.asset.direction = LEFT;
    goblin.lives = 5;
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
    
    damage = res.items.damage;
    damage.disabled = true;
    damage.position = (Vector2){
    	.x = lean.position.x,
    	.y = lean.position.y
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
    LevelFiveRead(player, &goblin, &lean, &skate, &left_hand, &right_hand);
    
    RunPhysicsStep();
    BeginDrawing();

    // Draw level's background
    DrawTextureEx(res.backgrounds.level5, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
    
    /** CUSTOM ****************************************************************************/
    
    // Draw lean, gob and goblean
    //gp_drawAsset(&res.items.damage, (Vector2){500, 280}, 0.7);
    gp_drawAsset(&goblin.asset, goblin.asset.position, goblin.asset.scale);
    gp_drawAsset(&right_hand, right_hand.position, right_hand.scale);
    gp_drawAsset(&left_hand, left_hand.position, left_hand.scale);
    gp_drawAsset(&skate, skate.position, skate.scale);
    gp_drawAsset(&goblin_flip, goblin.asset.position, goblin_flip.scale);
    gp_drawAsset(&lean, lean.position, lean.scale);
    gp_drawAsset(&damage, damage.position, damage.scale);
    
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
    
    //DrawRectangle(skate.position.x - 40, skate.position.y - 20, 80, 40, GOLD);
    
    EndDrawing();
}
