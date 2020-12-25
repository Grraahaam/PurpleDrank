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

//Enemy gob, goblean;
//Asset lean, gob_dying;

//Texture2D portal;

void LevelFiveRead(Player *player) {
    
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
}
    
void LevelFiveInit(Player *player) {
    
    ResetPhysics();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
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
    */
    
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
    LevelFiveRead(player);
    
    RunPhysicsStep();
    BeginDrawing();

    // Draw level's background
    DrawTextureEx(res.backgrounds.level5, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
    
    /** CUSTOM ****************************************************************************/
    
    // Draw lean, gob and goblean
    /*gp_drawAsset(&res.items.portal, (Vector2){15, 105}, 0.4);
    gp_drawAsset(&lean, lean.position, lean.scale);
    gp_drawAsset(&gob.asset, gob.asset.position, gob.asset.scale);
    gp_drawAsset(&gob_dying, gob_dying.position, gob_dying.scale);
    gp_drawAsset(&goblean.asset, goblean.asset.position, goblean.asset.scale);
    
    gp_drawText(
        (char*)TextFormat("Goblean lives : %d / 3", goblean.lives),
        res.fonts.pixellari, (Vector2){0, 70},
        20, CENTER_X, DARKGRAY
    );*/

    /**************************************************************************************/
    
    // Player's info board (lifes, lean, level) and notification
    gp_drawBoard(player);
    
    // Draw player
    gp_drawAsset(&player->asset, player->asset.position, player->asset.scale);
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);
    
    EndDrawing();
}
