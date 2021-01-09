#include <stdio.h>
#include <raylib.h>
#include <unistd.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "level3.h"

//pthread_t thread_id;

Enemy gob, dark_solin;
Asset lean, gob_dying, fire, damage;

Texture2D portal;

Damage l3_collisionLean(Asset *lean, Asset *fire, Enemy *gob, Enemy *dark_solin) {
    
    // Lean hit a gob
    if(gp_collisionAssets(lean, &gob->asset)) return GOB;
    
    //Lean hit the Dark solin
    if(gp_collisionAssets(lean, &dark_solin->asset)) return BOSS;
    
    // Lean hit the fire
    if(gp_collisionAssets(lean, fire) && !fire->disabled) return FIRE;
    
    // Lean hit nothing
    return NONE;
}

Damage l3_collisionPlayer(Player *player, Enemy *gob, Enemy *dark_solin, Asset *fire) {
    
    // Define player hitbox
    Rectangle player_hitbox = {
        .x = player->asset.position.x - player->asset.scale * player->asset.width / 2,
        .y = player->asset.position.y - player->asset.scale * player->asset.height / 2,
        .width = player->asset.scale * player->asset.width, .height = player->asset.scale * player->asset.height
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
    if(gp_collisionAssets(&player->asset, fire) && !fire->disabled) return PLAYER;
    
    // Player hit by nothing
    return NONE;
}

void l3_resetGob(Asset *asset) {
    
    // Reset the gob to the initial position (right border + gob width + margin)
    asset->position.x = GetScreenWidth() + asset->swidth + 20;
}

// Function showing an explosion animation on given asset (+ sound fx)
void l3_playDamage(Asset *ast) {
    
    PlaySound(res.sounds.explosion);
    
    damage.position.x = ast->position.x;
    damage.position.y = ast->position.y;
    damage.frame.current = 0;
    damage.disabled = false;
    damage.frame.animate = true;
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
        PlaySound(res.sounds.lean);
    }
}

void l3_destroyLean(Asset *lean) {
    
    // Destroy throwed lean
    lean->disabled = true;
    lean->position = (Vector2){0,0};
}

void l3_readDamage(Damage dtype, Damage actor, Player *player, Asset *lean, Asset *fire, Enemy *gob, Enemy *dark_solin) {
    
    // If gob or dark solin hit by a throwed lean
    switch(dtype) {
        
        case GOB: {
            
            // Decrement the amount of gob to spawn
            --gob->amount;
            
            if(gob->amount > 0) PlaySound(res.sounds.monster);
            
            gob_dying.disabled = false;
            gob_dying.position = (Vector2){
                gob->asset.position.x,
                gob->asset.position.y + gp_perY(.5)
            };
            gob_dying.frame.animate = true;
            
            // Increasing the gob's velocity each time one disappear or is killed (max. speed limit)
            if(gob->asset.speed < gp_perX(2)) {
                
                gob->asset.speed += gp_perX(.1);
                gob->asset.frame.speed += 3;
            }
            
            // If lean throwed, destroy it
            if(actor == LEAN) {
                
                l3_destroyLean(lean);
                
                damage.type = GOB;
                l3_playDamage(&gob->asset);
            }
            
            // Reset gob inital position
            l3_resetGob(&gob->asset);
            
        } break;
        
        case BOSS: {
            
            // Decrement dark solin's lives
            --dark_solin->lives;
            
            // If lean throwed, destroy it
            if(actor == LEAN) l3_destroyLean(lean);
            
            damage.type = BOSS;
            l3_playDamage(lean);
            
        } break;
        
        case FIRE: {
            
            // If lean throwed, destroy it
            if(actor == LEAN) {
                
                l3_destroyLean(lean);
            
                damage.type = FIRE;
                
            }
            
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

void l3_readCollisions(Player *player, Asset *lean, Asset *fire, Enemy *gob, Enemy *dark_solin) {
    
    // Read and process collision damage type
    l3_readDamage(l3_collisionLean(lean, fire, gob, dark_solin), LEAN, player, lean, fire, gob, dark_solin);
    l3_readDamage(l3_collisionPlayer(player, gob, dark_solin, fire), PLAYER, player, lean, fire, gob, dark_solin);
}

void LevelThreeRead(Player *player, Asset *lean, Asset *fire, Enemy *gob, Enemy *dark_solin) {
    
    gp_readPlayer(player);
    l3_readCollisions(player, lean, fire, gob, dark_solin);
    
    // If ended level
    if(dark_solin->lives <= 0) {
        
        dark_solin->asset.disabled = true;
        
        PlaySound(res.sounds.level_change);
        
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
                    if(dark_solin->asset.position.x < (GetScreenWidth() / 2 + dark_solin->asset.swidth)) {
                    
                    	 PlaySound(res.sounds.fire);
                        fire->disabled = false;
                        fire->frame.animate = true;  
                        
                        // Invert direction
                        dark_solin->asset.direction = RIGHT;
                    }
                    
                } else {
                    
                    dark_solin->asset.position.x += dark_solin->asset.speed;
                    
                    // If dark solin reached the right side and is facing right (right - dark_solin width - margin)
                    if(dark_solin->asset.position.x > (GetScreenWidth() - dark_solin->asset.swidth + 20)){
                    
                    	 StopSound(res.sounds.fire);
                        fire->disabled = true;
                        fire->frame.animate = false;                    
                        
                        // Invert direction
                        dark_solin->asset.direction = LEFT;
                    }
                }
            }
        }
        
        // Handle damage, clean it when done and follow boss when moving (explosion animation)
        if(damage.frame.current >= 9) damage.disabled = true;
        if(!damage.disabled && damage.type == BOSS) {
            
            damage.position.x = dark_solin->asset.position.x;
            damage.position.y = dark_solin->asset.position.y;
        }
        
        // If there's a lean being throwed
        if(!lean->disabled) {
            
            if(lean->direction == RIGHT) lean->position.x += lean->speed;
            else lean->position.x -= lean->speed;
        }
        
        // Destroy lean if outside the screen
        if(gp_isOutScreen(lean) && !lean->disabled) {
            
            PrintDebug("Lean destroyed");
            l3_destroyLean(lean);
        }
        
        // Destroy gob if outside the screen
        if(gp_isOutScreen(&gob->asset)) {

            PrintDebug("Gob destroyed");
            
            // Reset the gob inital position
            l3_resetGob(&gob->asset);
            
            // Increasing the gob's velocity each time one disappear or is killed
            if(gob->asset.speed < gp_perX(2)) {
                
                gob->asset.speed += gp_perX(.1);
                gob->asset.frame.speed += 3;
            }
        }
        
        // Stop and reset dying gob animation if reached 
        if(gob_dying.frame.current >= gob_dying.frame.amount - 1) {
            
            gob_dying.disabled = true;
            gob_dying.frame.animate = false;
            gob_dying.frame.current = 0;
        }
    }
}
    
void LevelThreeInit(Player *player) {
    
    ResetPhysics();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    PlaySound(res.sounds.monster);
    
    PrintDebug(TextFormat("Initializing: %s", screenNames[game.gameScreen]));
    
    gp_resetNotification();
    
    //TODO: IMPLEMENT THREAD FUNCTION TO TIMEOUT LEVEL LOADING AND DEATH TIMEOUT
    
    gp_resetPlayer(player);
    
    /** CUSTOM ****************************************************************************/
    
    // Initialize Lean object (to throw)
    lean = res.items.lean;
    lean.scale = gp_perX(1.5) / res.items.lean.width;
    lean.speed = gp_perX(.5);
    lean.position = (Vector2){0,0};
    lean.disabled = true;
    
    // Initialize gob asset
    gob.asset = res.items.gob_walking;
    gob.asset.speed = gp_perX(.1);
    gob.amount = 4;
    
    gob.asset.position = (Vector2){
        .x = GetScreenWidth() + gob.asset.swidth + 20,
        .y = gp_perY(63) - gob.asset.sheight / 2
    };
    
    // Initialize dying gob
    gob_dying = res.items.gob_dying;
    gob_dying.disabled = true;
    
    // Initialize Dark solin asset
    dark_solin.asset = res.items.dark_sonic;
    dark_solin.asset.speed = gp_perX(.2);
    dark_solin.asset.direction = LEFT;
    dark_solin.lives = 3;
    
    dark_solin.asset.position = (Vector2){
        .x = GetScreenWidth() + dark_solin.asset.swidth + 20,
        .y = gp_perY(40)
    };
    
    fire = res.items.fire_columns;
    fire.disabled = true;
    fire.position = (Vector2){
        .x = gp_perX(50),
        .y = gp_perY(63) - fire.sheight / 2
    };
    
    damage = res.items.damage;
    damage.disabled = true;
    damage.scale = gp_perX(12) / res.items.damage.width;
    
    // Left wall
    gp_createPhyRec((Vector2){
        .x = gp_perX(-1),
        .y = gp_perY(0)
    }, gp_perX(0.5), gp_perY(100));
    
    // Right wall
    gp_createPhyRec((Vector2){
        .x = GetScreenWidth() + gp_perX(1),
        .y = 0
    }, gp_perX(0.5), gp_perY(100));
    
    // Main floor
    gp_createPhyRec((Vector2){
        .x = 0,
        .y = gp_perY(63)
    }, gp_perX(100), gp_perY(20));
    
    // Platform
    gp_createPhyRec((Vector2){
        .x = 0,
        .y = gp_perY(30.5)
    }, gp_perX(8.5), gp_perY(15));
    
    /************************************************************************************/
    
    // Initialize player's body and default position
    gp_initializeBody(player, game.levelPos.level_3);
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
    LevelThreeRead(player, &lean, &fire, &gob, &dark_solin);
    
    RunPhysicsStep();
    BeginDrawing();
    
    ClearBackground(BLACK);

    // Draw level's background
    gp_drawImage(&res.backgrounds.level3, res.backgrounds.level3.scale);
    
    /** CUSTOM ****************************************************************************/
    
    // Draw lean, gob and dark solin
    gp_drawAsset(&res.items.portal, (Vector2){
        gp_perX(1.5), gp_perY(23)},
        gp_perX(2) / res.items.portal.width
    );
    
    gp_drawAsset(&lean, lean.position, lean.scale);
    
    gp_drawAsset(&gob.asset, gob.asset.position, gob.asset.scale);
    gp_drawAsset(&gob_dying, gob_dying.position, gob_dying.scale);
    
    gp_drawAsset(&dark_solin.asset, dark_solin.asset.position, dark_solin.asset.scale);
    
    gp_drawAsset(&fire, fire.position, fire.scale);
    
    gp_drawAsset(&damage, damage.position, damage.scale);
    
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
    }, 2, PINK);
    
    gp_drawText(
        (char*)TextFormat("Dark solin lives : %d / 3", dark_solin.lives),
        res.fonts.pixellari, (Vector2){0, 70},
        20, CENTER_X, DARKGRAY
    );
    */

    /**************************************************************************************/
    
    // Player's info board (lifes, lean, level) and notification
    gp_drawBoard(player);
    
    // Draw player
    gp_drawAsset(&player->asset, player->asset.position, player->asset.scale);
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);
    
    EndDrawing();
}
