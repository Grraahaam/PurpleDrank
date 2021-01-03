#include <stdio.h>
#include <raylib.h>
#include <unistd.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "level2.h"

Asset lean[3];

// Check collision between player and all the lean items
bool l2_collisionLean(Player *player) {

    for(int i = 0; i < 3; i++) {
        
        if(gp_collisionAssets(&player->asset, &lean[i]) && !lean[i].disabled) {
         
            lean[i].disabled = true;
            return true;
        }
    }

    return false;
}

// Function checking player's collisions with other physic bodies or items
void l2_readCollisions(Player *player) {

    // Player fall into the hole
    if (gp_collisionAssets(&player->asset, &res.items.spikes) || player->dead) {

        // Check if player is dead
        if(player->lives <= 0) {

            // Redirect player to the gameover screen
            game.gameScreen = GAMEOVER;

        } else {
            // If the player just fall in the spikes
            if(!player->dead) {

                // Set the player as dead and static
                player->dead = true;
                player->can_move = false;

                // Decrement player's lifes
                --player->lives;

                // Re-enable the lean assets, and remove player's lean inventory
                l2_resetLean();
                player->lean = 0;
            
            // Check if player is dead
            } else {
                
                // Set the player alive again and allow movements
                player->dead = false;
                player->can_move = true;
                
                //TODO: THREAD this TIMEOUT because it cause a segmentation issue
                
                // Reset player's position
                player->body->position = game.levelPos.level_2;
                
                // Empty lean inventory and reset the lean item on map
                l2_resetLean(player);
            }
        }
    }

    // Player reached the portal
    if(gp_collisionAssets(&player->asset, &res.items.portal)) {
        
        // If player have enough lean
        if(player->lean >= 3) {
            
            PrintDebug("Teleporting to level 2");
            
            //game.notification.message = "Teleporting through the Gob-Portal!";
            //game.notification.color = GREEN;
                
            // Block player's movements
            player->can_move = false;
            
            game.levelPassed = LEVEL_2;
            game.gameScreen = LEVEL_3;
            
        }
    }

    // Player is using the launchpad
    if(gp_collisionAssets(&player->asset, &res.items.launchpad)) {
        
        // If launchpad activated
        if(player->can_move && IsKeyPressed(KEY_SPACE)) {
            
            player->body->velocity.y = -1 * gp_perY(.4);
            player->body->velocity.x = gp_perX(.15);
            
            res.items.launchpad.frame.animate = true;
        }
    }
    
    // Player pick up the lean buckets
    if(l2_collisionLean(player)) {
        
        ++player->lean;
    }
}

void l2_resetLean() {
    
    for(int i = 0; i < 3; i++) {
        lean[i].disabled = false;
    }
}

void LevelTwoRead(Player *player) {
    
    gp_readPlayer(player);
    l2_readCollisions(player);
    
    // En/disable the portal
    if(player->lean >= 3) {
        
        res.items.portal.disabled = false;
        
    } else {
        
        res.items.portal.disabled = true;
    }
    
    // If launchpad animation ended, reset it
    if(res.items.launchpad.frame.current >= res.items.launchpad.frame.amount - 1) {
        
        res.items.launchpad.frame.animate = false;
        res.items.launchpad.frame.current = 0;
        res.items.launchpad.frame.x = 0;
    }
}

void LevelTwoInit(Player *player) {
    
    ResetPhysics();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    PrintDebug(TextFormat("Initializing: %s", screenNames[game.gameScreen]));
    
    gp_resetNotification();
    
    //TODO: IMPLEMENT THREAD FUNCTION TO TIMEOUT LEVEL LOADING AND DEATH TIMEOUT
    
    gp_resetPlayer(player);
    
    /** CUSTOM ****************************************************************************/
    
    // Disable the portal
    res.items.portal.disabled = true;
    
    // Left wall
    gp_createPhyRec((Vector2){
        .x = gp_perX(-1),
        .y = gp_perY(0)
    }, gp_perX(0.5), gp_perY(100));
    
    // Right wall
    gp_createPhyRec((Vector2){
        .x = GetScreenWidth() + gp_perX(1),
        .y = gp_perY(-20)
    }, gp_perX(0.5), gp_perY(120));
    
    // Roof
    gp_createPhyRec((Vector2){
        .x = 0,
        .y = gp_perY(-20)
    }, gp_perX(100), gp_perY(5));
    
    // Start floor
    gp_createPhyRec((Vector2){
        .x = 0,
        .y = gp_perY(58)
    }, gp_perX(10), gp_perY(42));
    
    // Floor after spikes
    gp_createPhyRec((Vector2){
        .x = gp_perX(22.5),
        .y = gp_perY(78.5)
    }, gp_perX(7), gp_perY(20));
    
    // Launchpad floor
    gp_createPhyPoly((Vector2){
        .x = gp_perX(25),
        .y = gp_perY(84.8)
    }, gp_perX(8), 10);
    
    // Main floor
    gp_createPhyRec((Vector2){
        .x = gp_perX(35),
        .y = gp_perY(83.5)
    }, gp_perX(68), gp_perY(10));
    
    // Floor before portal
    gp_createPhyPoly((Vector2){
        .x = gp_perX(56),
        .y = gp_perY(90.5)
    }, gp_perX(16), 10);
    
    // Lean platform
    gp_createPhyRec((Vector2){
        .x = gp_perX(77),
        .y = gp_perY(15)
    }, gp_perX(22), gp_perY(20));
    
    // Initialize the lean assets
    for(int i = 0; i < 3; i++) {
        lean[i] = res.items.lean;
        lean[i].disabled = false;
        lean[i].scale = gp_perX(1.8) / res.items.lean.width;
    }
    
    lean[0].position = (Vector2){gp_perX(82), gp_perY(11.8)};
    lean[1].position = (Vector2){gp_perX(88), gp_perY(11.8)};
    lean[2].position = (Vector2){gp_perX(94), gp_perY(11.8)};
    
    res.items.spikes.position = (Vector2){gp_perX(13.4), gp_perY(86)};
    res.items.spikes.scale = gp_perX(10) / res.items.spikes.width;

    res.items.launchpad.position = (Vector2){gp_perX(37), gp_perY(77)};
    res.items.launchpad.scale = gp_perX(7) / res.items.launchpad.width;
    
    res.items.portal.position = (Vector2){gp_perX(90), gp_perY(65.5)};
    res.items.portal.scale = gp_perX(5) / res.items.portal.width;
    
    /************************************************************************************/
    
    // Initialize player's body and default position
    gp_initializeBody(player, game.levelPos.level_2);
}

void LevelTwoDraw(Player *player, ScreenFX *screenFx) {

     // Initialize the level only if not loaded
    if(game.screenLoaded != LEVEL_2) {
        
        LevelTwoInit(player);
        PrintDebug(TextFormat("Drawing: %s", screenNames[game.gameScreen]));
        
        // Update game informations
        game.screenLoaded = LEVEL_2;
        game.levelPassed = LEVEL_1;
        
        // Set default fade properties
        gp_resetFx(screenFx);
    }

    // Read user input and interact
    LevelTwoRead(player);
    
    RunPhysicsStep();
    BeginDrawing();

    // Draw level's background
    gp_drawImage(&res.backgrounds.level2, res.backgrounds.level2.scale);
    
    /** CUSTOM ****************************************************************************/
    
    // Draw the spikes
    gp_drawAsset(&res.items.spikes, res.items.spikes.position, res.items.spikes.scale);
    
    // Draw the leans
    for(int i = 0; i < 3; i++) {
        
        gp_drawAsset(&lean[i], lean[i].position, lean[i].scale);
    }
    
    // Draw the launchpad
    gp_drawAsset(&res.items.launchpad, res.items.launchpad.position, res.items.launchpad.scale);
    
    // Draw the portal
    gp_drawAsset(&res.items.portal, res.items.portal.position, res.items.portal.scale);

    /**************************************************************************************/

    // Player's info board (lifes, lean, level) and notification
    gp_drawBoard(player);
    
    // Draw player
    gp_drawAsset(&player->asset, player->asset.position, player->asset.scale);
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);
    
    EndDrawing();
}
