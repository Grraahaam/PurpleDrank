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

Enemy gob, dark_solin;
Asset lean, gob_dying, fire;

Texture2D portal;

Damage l3_collisionLean(Asset *lean, Enemy *gob, Enemy *dark_solin) {
    
    // Lean hit a gob
    if(gp_collisionAssets(lean, &gob->asset)) return GOB;
    
    //Lean hit the Dark solin
    else if(gp_collisionAssets(lean, &dark_solin->asset)) return BOSS;
    
    // Lean hit nothing
    else return NONE;
}

Damage l3_collisionPlayer(Player *player, Enemy *gob, Enemy *dark_solin) {
    
    // Define player hitbox
    Rectangle player_hitbox = {
        .x = player->asset.position.x - player->asset.swidth / 2,
        .y = player->asset.position.y - player->asset.sheight / 2,
        .width = player->asset.swidth, .height = player->asset.sheight
    };
    
    // Player hit a gob (TOP)
    if(CheckCollisionRecs(
        // Player
        player_hitbox,
        // Gob top
        (Rectangle){
            .x = gob->asset.position.x - gob->asset.swidth / 2 + 20,
            .y = gob->asset.position.y - gob->asset.sheight / 2,
            .width = gob->asset.swidth - 40, .height = 30

        })) return GOB;
    
    // Player hit a gob (RIGHT/LEFT)
    if(CheckCollisionRecs(
        // Player
        player_hitbox,
        // Gob left
        (Rectangle){
            .x = gob->asset.position.x - gob->asset.swidth / 2 + 5,
            .y = gob->asset.position.y - gob->asset.sheight / 2 + 30,
            .width = gob->asset.swidth - 10, .height = gob->asset.sheight - 30

        })) return PLAYER;
    
    // Player hit by dark solin (BOSS)
    if(gp_collisionAssets(&player->asset, &dark_solin->asset)) return PLAYER;
        
    // Player hit by fire
    if(CheckCollisionRecs(
        // Player
        player_hitbox,
        // Fire
        (Rectangle){
            .x = fire.position.x - 10, .y = fire.position.y - 80,
            .width = 20, .height = 200
            
        }) && !fire.disabled) return PLAYER;
    
    // Player hit by nothing
    return NONE;
}

void l3_resetGob(Asset *asset) {
    
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

void l3_readDamage(Damage dtype, Damage actor, Player *player, Asset *lean, Enemy *gob, Enemy *dark_solin) {
    
    // If gob or dark solin hit by a throwed lean
    switch(dtype) {
        
        // TODO: PLAY SOUND EFFECT ON HIT + SPRITE HIT FRAME
        
        case GOB: {
            
            // Decrement the amount of gob to spawn
            --gob->amount;
            
            gob_dying.disabled = false;
            gob_dying.position = gob->asset.position;
            gob_dying.frame.animate = true;
            
            // Reset gob inital position
            l3_resetGob(&gob->asset);
            
            // Increasing the gob's velocity each time one disappear or is killed (max. speed limit)
            if(gob->asset.speed < 2.5f) {
                
                gob->asset.speed += 0.5f;
                gob->asset.frame.speed += 2;
            }
            
            // If lean throwed, destroy it
            if(actor == LEAN) l3_destroyLean(lean);
            
        } break;
        
        case BOSS: {
            
            // Decrement dark solin's lives
            --dark_solin->lives;
            
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

void l3_readCollisions(Player *player, Asset *lean, Enemy *gob, Enemy *dark_solin) {
    
    // Read and process collision damage type
    l3_readDamage(l3_collisionLean(lean, gob, dark_solin), LEAN, player, lean, gob, dark_solin);
    l3_readDamage(l3_collisionPlayer(player, gob, dark_solin), PLAYER, player, lean, gob, dark_solin);
}

void LevelThreeRead(Player *player, Asset *lean, Enemy *gob, Enemy *dark_solin) {
    
    gp_readPlayer(player);
    l3_readCollisions(player, lean, gob, dark_solin);
    
    // If ended level
    if(dark_solin->lives <= 0) {
        
        dark_solin->asset.disabled = true;
        
        game.levelPassed = LEVEL_3;
        game.gameScreen = LEVEL_4;
    
    } else {
    
        // Throw lean
        if(IsKeyPressed(KEY_Q)) l3_throwLean(player, lean);
        
        // If still gob enemies to kill
        if(gob->amount > 0) {
            
            // Gob moving to left until they go through the left border
            gob->asset.position.x -= gob->asset.speed;
        
        } else {
         
            
            // If dark solin still alive (translate back and forth infinitely on X axis)
            if(dark_solin->lives > 0) {
                
                if(dark_solin->asset.direction == LEFT) {
                    
                    dark_solin->asset.position.x -= dark_solin->asset.speed;
                    
                    // If dark solin reached the middle and is facing left
                    if(dark_solin->asset.position.x < (screenWidth / 2 + dark_solin->asset.swidth)) {
                    
                    	 fire.disabled = false;
                    	 fire.frame.animate = true;  
                        // Invert direction
                        dark_solin->asset.direction = RIGHT;
                    }
                    
                } else {
                    
                    dark_solin->asset.position.x += dark_solin->asset.speed;
                    
                    // If dark solin reached the right side and is facing right (right - dark_solin width - margin)
                    if(dark_solin->asset.position.x > (screenWidth - dark_solin->asset.swidth + 20)) {
                    
                    	 fire.disabled = true;
                    	 fire.frame.animate = false;                    
                        // Invert direction
                        dark_solin->asset.direction = LEFT;
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
            l3_resetGob(&gob->asset);
            
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
    switch(gp_isOutScreen((player, lean, gob, dark_solin))) {
        
        // Player
        case 0: {
            
        } break;
        
        // Lean
        case 1: {
            
        } break;
        
        // Gob
        case 2: {
            
        } break;
        
        // Dark solin
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
    
    //TODO: IMPLEMENT THREAD FUNCTION TO TIMEOUT LEVEL LOADING AND DEATH TIMEOUT
    
    gp_resetPlayer(player);
    
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
    
    // Initialize Dark solin asset
    dark_solin.asset = res.items.dark_sonic;
    dark_solin.asset.speed = 1;
    dark_solin.asset.direction = LEFT;
    dark_solin.lives = 3;
    
    dark_solin.asset.position = (Vector2){
        .x = screenWidth + dark_solin.asset.swidth + 20,
        .y = 180
    };
    
    fire = res.items.fire_columns;
    fire.disabled = true;
    fire.position = (Vector2){
        .x = screenWidth/2,
        .y = 160
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
        
        LevelThreeInit(player);
        PrintDebug(TextFormat("Drawing: %s", screenNames[game.gameScreen]));
        
        //pthread_create(thread_id, NULL, unlockPlayer, NULL);
        //pthread_join(thread_id, NULL);
        //res.items.portal.disabled = true;
            
        // Update game informations
        game.screenLoaded = LEVEL_3;
        game.levelPassed = LEVEL_2;
        
        // Set default fade properties
        gp_resetFx(screenFx);
    }
    
    // Read user input and interact
    LevelThreeRead(player, &lean, &gob, &dark_solin);
    
    RunPhysicsStep();
    BeginDrawing();

    // Draw level's background
    gp_drawImage(&res.backgrounds.level3, res.backgrounds.level3.scale);
    
    /** CUSTOM ****************************************************************************/
    
    // Draw lean, gob and dark solin
    gp_drawAsset(&res.items.portal, (Vector2){15, 105}, 0.4);
    gp_drawAsset(&lean, lean.position, lean.scale);
    gp_drawAsset(&gob.asset, gob.asset.position, gob.asset.scale);
    gp_drawAsset(&gob_dying, gob_dying.position, gob_dying.scale);
    gp_drawAsset(&dark_solin.asset, dark_solin.asset.position, dark_solin.asset.scale);
    gp_drawAsset(&fire, fire.position, fire.scale);
    
    // HITBOXES debug boxes
    /*DrawRectangleLinesEx(
        (Rectangle){
        .x = gob.asset.position.x - gob.asset.swidth / 2 + 20,
        .y = gob.asset.position.y - gob.asset.sheight / 2,
        .width = gob.asset.swidth - 40, .height = 30
    }, 2, GREEN);
    
    DrawRectangleLinesEx(
        (Rectangle){
        .x = gob.asset.position.x - gob.asset.swidth / 2,
        .y = gob.asset.position.y - gob.asset.sheight / 2 + 30,
        .width = gob.asset.swidth, .height = gob.asset.sheight - 30
    }, 2, PINK);*/
    
    gp_drawText(
        (char*)TextFormat("Dark solin lives : %d / 3", dark_solin.lives),
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
    
    //DrawRectangle(fire.position.x - 10, fire.position.y - 80, 20, 200, GOLD);
    
    EndDrawing();
}
