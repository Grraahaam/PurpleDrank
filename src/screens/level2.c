#include <stdio.h>
#include <raylib.h>
#include <unistd.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "level2.h"

Asset lean[3];

bool l2_collisionSpikes(Player *player) {

    return CheckCollisionRecs(
        // Player
        (Rectangle){
            .x = player->asset.position.x, .y = player->asset.position.y,
            .width = player->asset.swidth, .height = player->asset.sheight
        },
        // Spikes
        (Rectangle){
            .x = res.items.spikes.position.x, .y = res.items.spikes.position.y,
            .width = res.items.spikes.swidth, .height = res.items.spikes.sheight
        }
    );
}

bool l2_collisionPortal(Player *player) {

    return CheckCollisionRecs(
        // Player
        (Rectangle){
            .x = player->asset.position.x, .y = player->asset.position.y,
            .width = player->asset.swidth, .height = player->asset.sheight
        },
        // Portal
        (Rectangle){
            .x = res.items.portal.position.x, .y = res.items.portal.position.y,
            .width = res.items.portal.swidth, .height = res.items.portal.sheight
        }
    );
}

bool l2_collisionLaunchpad(Player *player) {

    return CheckCollisionRecs(
        // Player
        (Rectangle){
            .x = player->asset.position.x, .y = player->asset.position.y,
            .width = player->asset.swidth, .height = player->asset.sheight
        },
        // Launchpad
        (Rectangle){
            .x = res.items.launchpad.position.x, .y = res.items.launchpad.position.y,
            .width = res.items.launchpad.swidth, .height = res.items.launchpad.sheight
        }
    );
}

// Check collision between player and all the lean items
bool l2_collisionLean(Player *player) {

    for(int i = 0; i < 3; i++) {

        if(!lean[i].disabled && CheckCollisionRecs(
            // Player
            (Rectangle){
                .x = player->asset.position.x, .y = player->asset.position.y,
                .width = player->asset.swidth, .height = player->asset.sheight
            },
            // Current lean
            (Rectangle){
                .x = lean[i].position.x, .y = lean[i].position.y,
                .width = lean[i].swidth, .height = lean[i].sheight
            })) {

            lean[i].disabled = true;
            return true;
        }
    }

    return false;
}

// Function checking player's collisions with other physic bodies or items
void l2_readCollisions(Player *player) {

    // Player fall into the hole
	if (l2_collisionSpikes(player) || player->dead) {

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
                // Time the re-apparition
                //sleep(GAME_DEFAULT_TIMEOUT);
                
                // Reset player's position
                player->body->position = game.levelPos.level_2;
                
                // Empty lean inventory and reset the lean item on map
                l2_resetLean(player);
            }
        }
    }
	
	// Player reached the portal
	if (l2_collisionPortal(player)) {
        
        // If player have enough lean
        if(player->lean >= 3) {
            
            PrintDebug("Teleporting to level 2");
            
            //game.notification.message = "Teleporting through the Gob-Portal!";
            //game.notification.color = GREEN;
                
            // Block player's movements
            player->can_move = false;
            
            game.levelPassed = LEVEL_2;
            game.gameScreen = LEVEL_3;
            
        } else {
            
            //game.notification.message = "Not enough Lean to use the Gob-Portal!";
            game.notification.color = RED;
        }
    
    // Clean the notification if not needed
	} else {
        
        game.notification.message = "";
    }
	
	// Player is using the launchpad
	if(l2_collisionLaunchpad(player)) {
        
        // If launchpad activated
        if(player->can_move && IsKeyPressed(KEY_SPACE)) {
            
            player->body->velocity.y = -VELOCITY * 5.5;
            player->body->velocity.x = +VELOCITY * 2;
            
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
    
    //TODO: PUT THIS INTO A GLOBAL FUNCTION TO Initialize BASE PROPERTIES FOR PLAYER AT EACH LEVEL
    //TODO: IMPLEMENT THREAD FUNCTION TO TIMEOUT LEVEL LOADING AND DEATH TIMEOUT
    player->slip = false;
    
    /** CUSTOM ****************************************************************************/
    
    // Disable the portal
    res.items.portal.disabled = true;
    
    PhysicsBody floor_start = CreatePhysicsBodyRectangle((Vector2){ 30, 360 }, 65, 190, 0);
    PhysicsBody floor_after_spikes = CreatePhysicsBodyRectangle((Vector2){ 218, 390 }, 105, 70, 0);
    PhysicsBody floor_launchpad = CreatePhysicsBodyPolygon((Vector2){ 510, 475 }, 130, 10, 0);
    PhysicsBody floor_main = CreatePhysicsBodyRectangle((Vector2){ 545, 402 }, 510, 50, 0);
    PhysicsBody floor_before_portal = CreatePhysicsBodyPolygon((Vector2){ 245, 440 }, 90, 10, 0);
    
    floor_start->enabled = false;
    floor_after_spikes->enabled = false;
    floor_main->enabled = false;
    floor_before_portal->enabled = false;
    floor_launchpad->enabled = false;
    
    PhysicsBody platform_lean = CreatePhysicsBodyRectangle(
        (Vector2){ 705, 135 },
        180, 130, 0
    ); platform_lean->enabled = false;
    
    PhysicsBody wall_left = CreatePhysicsBodyRectangle(
        (Vector2){-5, screenHeight / 2 },
        5, screenHeight * 2, 0
    ); wall_left->enabled = false;
    
    PhysicsBody wall_right = CreatePhysicsBodyRectangle(
        (Vector2){screenWidth + 5 , screenHeight / 2},
        5, screenHeight * 2, 0
    ); wall_right->enabled = false;
    
    // Initialize the lean assets
    for(int i = 0; i < 3; i++) {
        lean[i] = res.items.lean;
        lean[i].disabled = false;
        lean[i].scale = 0.18;
        lean[i].swidth = lean[i].width * lean[i].scale;
        lean[i].sheight = lean[i].height * lean[i].scale;
    }
    
    lean[0].position = (Vector2){655, 53};
    lean[1].position = (Vector2){705, 53};
    lean[2].position = (Vector2){755, 53};
    
    res.items.spikes.position = (Vector2){108, 390};
    res.items.spikes.scale = 0.35;
    res.items.spikes.swidth = res.items.spikes.width * res.items.spikes.scale;
    res.items.spikes.sheight = res.items.spikes.height * res.items.spikes.scale;

    res.items.launchpad.position = (Vector2){297, 346};
    res.items.launchpad.scale = 0.35;
    res.items.launchpad.swidth = res.items.launchpad.width * res.items.launchpad.scale;
    res.items.launchpad.sheight = res.items.launchpad.height * res.items.launchpad.scale;
    
    res.items.portal.position = (Vector2){720, 295};
    res.items.portal.scale = 0.7;
    res.items.portal.swidth = res.items.portal.width * res.items.portal.scale;
    res.items.portal.sheight = res.items.portal.height * res.items.portal.scale;
    
    /************************************************************************************/
    
    // Set default position
    player->asset.position = game.levelPos.level_2;
    
    gp_initializeBody(player);
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
        
        // THIS ALLOWS THE PLAYER TO MOVE EVEN AFTER SUCCESS AND RELOAD
        //TODO: PUT INTO A FUNCTION OR CHECK WHY resetPlayer() in success.c
        player->can_move = true;
    }

    // Read user input and interact
    LevelTwoRead(player);
    
    RunPhysicsStep();
    BeginDrawing();

    // Draw level's background
    DrawTextureEx(res.backgrounds.level2, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
    
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
