#include <stdio.h>
#include <raylib.h>
#include <unistd.h>
#include <pthread.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "level3.h"

//pthread_t thread_id;

Enemy gob, goblean;
Asset lean, gob_dying, fire_columns;

Texture2D portal;

bool l3_collisionAssets(Asset *ast1, Asset *ast2) {
    
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

Damage l3_collisionLean(Asset *lean, Enemy *gob, Enemy *goblean) {
    
    // Lean hit a gob
    if(l3_collisionAssets(lean, &gob->asset)) return GOB;
    
    //Lean hit the Goblean
    else if(l3_collisionAssets(lean, &goblean->asset)) return BOSS;
    
    // Lean hit nothing
    else return NONE;
}

Damage l3_collisionPlayer(Player *player, Enemy *gob, Enemy *goblean) {
    
    // Define player hitbox
    Rectangle player_hitbox = {
        .x = player->asset.position.x, .y = player->asset.position.y,
        .width = player->asset.swidth, .height = player->asset.sheight
    };
    
    // Player hit a gob (TOP)
    if(CheckCollisionRecs(
        // Player
        player_hitbox,
        // Gob top
        (Rectangle){
            .x = gob->asset.position.x - 60, .y = gob->asset.position.y - 30,
            .width = 120, .height = 10

        })) return GOB;
    
    // Player hit a gob (LEFT)
    if(CheckCollisionRecs(
        // Player
        player_hitbox,
        // Gob left
        (Rectangle){
            .x = gob->asset.position.x - 80, .y = gob->asset.position.y,
            .width = 10, .height = 20

        })) return PLAYER;
    
    // Player hit a gob (RIGHT)
    if(CheckCollisionRecs(
        // Player
        player_hitbox,
        // GOB right
        (Rectangle){
            .x = gob->asset.position.x + 70, .y = gob->asset.position.y,
            .width = 10, .height = 20

        })) return PLAYER;
    
    // Player hit by goblean (GOBLEAN)
    if(CheckCollisionRecs(
        // Player
        player_hitbox,
        // Goblean
        (Rectangle){
            .x = goblean->asset.position.x - 60, .y = goblean->asset.position.y - 100,
            .width = 120, .height = 160
            
        })) return PLAYER;
    
    // Player hit by goblean (SKATE)
    if(CheckCollisionRecs(
        // Player
        player_hitbox,
        // Skate
        (Rectangle){
            .x = goblean->asset.position.x - 95, .y = goblean->asset.position.y + 62,
            .width = 180, .height = 40
            
        })) return PLAYER;
    
    // Lean hit nothing
    return NONE;
}

void l3_resetAsset(Asset *asset) {
    
    // Reset the gob to the initial position (right border + gob width + margin)
    asset->position.x = screenWidth + asset->swidth + 20;
}

void l3_throwLean(Player *player, Asset *lean) {

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

void l3_destroyLean(Asset *lean) {
    
    // Destroy throwed lean
    lean->disabled = true;
    lean->position = (Vector2){0,0};
}

void l3_readDamage(Damage dtype, Damage actor, Player *player, Asset *lean, Enemy *gob, Enemy *goblean) {
    
    // If gob or goblean hit by a throwed lean
    switch(dtype) {
        
        // TODO: PLAY SOUND EFFECT ON HIT + SPRITE HIT FRAME
        
        case GOB: {
            
            // Decrement the amount of gob to spawn
            --gob->amount;
            
            gob_dying.disabled = false;
            gob_dying.position = gob->asset.position;
            gob_dying.frame.animate = true;
            
            // Reset gob inital position
            l3_resetAsset(&gob->asset);
            
            // Increasing the gob's velocity each time one disappear or is killed (max. speed limit)
            if(gob->asset.speed < 2.5f) {
                
                gob->asset.speed += 0.5f;
                gob->asset.frame.speed += 2;
            }
            
            // If lean throwed, destroy it
            if(actor == LEAN) l3_destroyLean(lean);
            
        } break;
        
        case BOSS: {
            
            // Decrement goblean's lives
            --goblean->lives;
            
            // If lean throwed, destroy iy
            if(actor == LEAN) l3_destroyLean(lean);
            
        } break;
        
        case PLAYER: {
            
            // Decrement player's lives
            --player->lives;
            
            // Replace player
            player->body->position = game.levelPos.level_3;
            
        } break;

        // Default action
        default : break;
    }
}

void l3_readCollisions(Player *player, Asset *lean, Enemy *gob, Enemy *goblean) {
    
    // Read and process collision damage type
    l3_readDamage(l3_collisionLean(lean, gob, goblean), LEAN, player, lean, gob, goblean);
    l3_readDamage(l3_collisionPlayer(player, gob, goblean), PLAYER, player, lean, gob, goblean);
}

void LevelThreeRead(Player *player, Asset *lean, Enemy *gob, Enemy *goblean) {
    
    gp_readPlayer(player);
    l3_readCollisions(player, lean, gob, goblean);
    
    // If ended level
    if(goblean->lives <= 0) {
        
        goblean->asset.disabled = true;
        
        game.levelPassed = LEVEL_3;
        game.gameScreen = LEVEL_4;
    
    } else {
    
        // Throw lean
        if(IsKeyPressed(KEY_B)) l3_throwLean(player, lean);
        
        // If still gob enemies to kill
        if(gob->amount > 0) {
            
            // Gob moving to left until they go through the left border
            gob->asset.position.x -= gob->asset.speed;
        
        } else {
            
            // If goblean still alive (translate back and forth infinitely on X axis)
            if(goblean->lives > 0) {
                
                if(goblean->asset.direction == LEFT) {
                    
                    goblean->asset.position.x -= goblean->asset.speed;
                    
                    // If goblean reached the middle and is facing left
                    if(goblean->asset.position.x < (screenWidth / 2 + goblean->asset.swidth)) {
                    
                    	 fire_columns.disabled = true;
                    	 fire_columns.animate = false;  
                        // Invert direction
                        goblean->asset.direction = RIGHT;
                    }
                    
                } else {
                    
                    goblean->asset.position.x += goblean->asset.speed;
                    
                    // If goblean reached the right side and is facing right (right - goblean width - margin)
                    if(goblean->asset.position.x > (screenWidth - goblean->asset.swidth + 20)) {
                    
                    	 fire_columns.disabled = false;
                    	 fire_columns.animate = true;                    
                        // Invert direction
                        goblean->asset.direction = LEFT;
                    }
                }
            }
        }
        
        // If there's a lean being throwed
        if(!lean->disabled) {
            
            if(lean->direction == RIGHT) lean->position.x += lean->speed;
            else lean->position.x -= lean->speed;
        }
        
        // Destroy lean if outside the screen
        if(gp_isOutScreen(lean)) {
            
            PrintDebug("Lean destroyed");
            l3_destroyLean(lean);
        }
        
        // Destroy gob if outside the screen
        if(gp_isOutScreen(&gob->asset)) {
            
            //--gob->amount; // Do not countdown gob if they're not killed

            PrintDebug("Gob destroyed");
            
            // Reset the gob inital position
            l3_resetAsset(&gob->asset);
            
            // Increasing the gob's velocity each time one disappear or is killed
            if(gob->asset.speed < 2.5f) {
                
                gob->asset.speed += 0.5f;
                gob->asset.frame.speed += 2;
            }
        }
        
        // Stop and reset dying gob animation if reached 
        if(gob_dying.frame.current >= gob_dying.frame.amount - 1) {
            
            gob_dying.disabled = true;
            gob_dying.frame.animate = false;
            gob_dying.frame.current = 0;
        }
    }
    
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
}
    
void LevelThreeInit(Player *player) {
    
    ResetPhysics();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
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
    gob_dying.disabled = true;
    
    // Initialize Goblean asset
    goblean.asset = res.items.dark_sonic;
    goblean.asset.speed = 1;
    goblean.asset.direction = LEFT;
    goblean.lives = 3;
    
    goblean.asset.position = (Vector2){
        .x = screenWidth + goblean.asset.swidth + 20,
        .y = 180
    };
    
    fire_columns = res.items.fire_columns;
    fire_columns.disabled = true;
    fire_columns.position = (Vector2){
        .x = goblean.asset.position.x - 50,
        .y = goblean.asset.position.y - 20;
    };
    
    // Create floor and walls rectangle physics body
    PhysicsBody body_floor = CreatePhysicsBodyRectangle(
        (Vector2){screenWidth/2, 290},
        screenWidth, 10, 10);
    
    PhysicsBody body_platform = CreatePhysicsBodyRectangle(
        (Vector2){35, 180},
        70, 80, 10);
    
    PhysicsBody body_wall_left = CreatePhysicsBodyRectangle(
        (Vector2){-5, screenHeight / 2},
        10, screenHeight*2, 10);
    
    PhysicsBody body_wall_right = CreatePhysicsBodyRectangle(
        (Vector2){screenWidth + 5, screenHeight / 2},
        10, screenHeight*2, 10);
    
    // Disable dynamics to floor and walls physics bodies
    body_floor->enabled = false;
    body_platform->enabled = false;
    body_wall_left->enabled = false;
    body_wall_right->enabled = false;
    
    /************************************************************************************/
    
    // Set default position
    player->asset.position = game.levelPos.level_3;
    
    gp_initializeBody(player);
}

void LevelThreeDraw(Player *player, ScreenFX *screenFx) {

     // Initialize the level only if not loaded
    if(game.screenLoaded != LEVEL_3) {
        
        // If just arrived from previous level
        if(!player->can_move) {

            LevelThreeInit(player);
            PrintDebug(TextFormat("Drawing: %s", screenNames[game.gameScreen]));
            
            player->can_move = true;
            
            // Player just came from the previous level's portal
            res.items.portal.disabled = false;

        // If player just waited in the portal
        } else {
            
            //PrintDebug("Level 3 unlocked");
            
            //pthread_create(thread_id, NULL, unlockPlayer, NULL);
            //pthread_join(thread_id, NULL);
            
            // Update game informations
            game.screenLoaded = LEVEL_3;
            game.levelPassed = LEVEL_2;
            
            // Set default fade properties
            gp_resetFx(screenFx);
            
            //sleep(GAME_DEFAULT_TIMEOUT);
            res.items.portal.disabled = false;
            player->can_move = true;
        }
    }
    
    // Read user input and interact
    LevelThreeRead(player, &lean, &gob, &goblean);
    
    RunPhysicsStep();
    BeginDrawing();

    // Draw level's background
    DrawTextureEx(res.backgrounds.level3, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
    
    /** CUSTOM ****************************************************************************/
    
    // Draw lean, gob and goblean
    gp_drawAsset(&res.items.portal, (Vector2){15, 105}, 0.4);
    gp_drawAsset(&lean, lean.position, lean.scale);
    gp_drawAsset(&gob.asset, gob.asset.position, gob.asset.scale);
    gp_drawAsset(&gob_dying, gob_dying.position, gob_dying.scale);
    gp_drawAsset(&goblean.asset, goblean.asset.position, goblean.asset.scale);
    gp_drawAsset(&fire_columns, fire_columns.position, fire_columns.scale);
    
    gp_drawText(
        (char*)TextFormat("Goblean lives : %d / 3", goblean.lives),
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
