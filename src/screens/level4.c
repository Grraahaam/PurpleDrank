#include <raylib.h>
#include <unistd.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "level4.h"

Asset spikes[2], lean[3];
Asset portalTo, portalFrom, portalBonus, gelanoStatus;

//Return true if player reach the right side of the screen
bool l4_collisionRightWall(Player *player) {
    
	return CheckCollisionRecs(
        // Player
        (Rectangle){
            .x = player->asset.position.x - player->asset.scale * player->asset.width / 2,
            .y = player->asset.position.y - player->asset.scale * player->asset.height / 2,
            .width = player->asset.scale * player->asset.width, .height = player->asset.scale * player->asset.height
        },
        // Right wall
        (Rectangle){
            .x = GetScreenWidth() + player->asset.scale * player->asset.width / 2, .y = 0,
            .width = gp_perX(2), .height = GetScreenHeight()
        }
    );
}

bool l4_collisionSpikes(Player *player, Asset *spikes) {

    for(int i = 0; i < 2; i++) {
        
        if(gp_collisionAssets(&player->asset, &spikes[i])) return true;
    }
    
    return false;
}

// Function checking player's collisions with other physic bodies or items
void l4_readCollisions(Player *player, Asset *spikes, Asset *portalBonus) {

    // Player fall into spikes
    if (l4_collisionSpikes(player, spikes)) {

        // Set the player as dead and static
        //player->dead = true;
        
        // Decrement player's lifes
        --player->lives;
            
        // Reset player's position
        player->body->position = game.levelPos.level_4;
        
    } else if(l4_collisionRightWall(player)) {
        
        // Update its info board, and switch level
        game.levelPassed = LEVEL_4;
        game.gameScreen = LEVEL_5;
        
    } else if(gp_collisionAssets(&player->asset, portalBonus)) {
        
        PrintDebug("Teleporting to Bonus level");
        game.gameScreen = LEVEL_BONUS;

    // Player reached the portal
    } else if(gp_collisionAssets(&player->asset, &portalFrom)) {
        
        if(!portalFrom.disabled && !player->portalPowerUsed) {

            PrintDebug("Teleporting to Level 2");
            
            portalTo.disabled = true;
            portalFrom.disabled = true;
            player->body->position.x = portalTo.position.x;
            player->body->position.y = portalTo.position.y;
            player->portalPowerUsed = true;
        }
	}
}

void LevelFourRead(Player *player, Asset *spikes, Asset *portalBonus) {
    
    gp_readPlayer(player);
    l4_readCollisions(player, spikes, portalBonus);

    // Enable the portal
    if(IsKeyPressed(KEY_E) && !player->portalPowerUsed) {
        
        if(portalTo.disabled) {

            portalTo.position = (Vector2){
                player->asset.position.x + gp_perX(18.5),
                player->asset.position.y - gp_perY(42)
            };
            
            portalFrom.position = (Vector2){
                player->asset.position.x + portalFrom.scale * portalFrom.width + gp_perX(1),
                player->asset.position.y
            };
            
            portalTo.disabled = false;	
            portalFrom.disabled = false;
            
        } else {
            
            portalTo.disabled = true;
            portalFrom.disabled = true;
        }
    }

    //Enable the JetLean mode
    if(IsKeyPressed(KEY_W)) {
        
        if(player->lean >= 3 && !player->jetLean ) {

            player->jetLean = true;

            for(int i = 0; i < 3; i++) {
                
                lean[i].disabled = false;
            }
        }
    }
    
    // Place the JetLean under the player
    if(player->jetLean) {
        
        for(int i = 0; i < 3; i++) {
            
            lean[i].position = (Vector2){
                player->asset.position.x - gp_perX(1) + i * gp_perX(1.5),
                player->asset.position.y + player->asset.sheight / 2 + gp_perY(.2)
            };
        }
    }

    // Using the JetLean
    if(IsKeyPressed(KEY_SPACE) && player->jetLean) {
        
        player->body->velocity.y = -1 * gp_perY(.36);
        player->jetLean = false;
        player->lean -= 3;
        
        for(int i = 0; i < 3; i++) {
            
            lean[i].disabled = true;
        }
    }

    // Enable Gelano (disable slipping mode)
    if(IsKeyPressed(KEY_G)) {

        player->slip = player->gelano ? false : true;
    }
}
    
void LevelFourInit(Player *player) {

    ResetPhysics();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    PrintDebug(TextFormat("Initializing: %s", screenNames[game.gameScreen]));
    
    gp_resetNotification();
    
    //TODO: IMPLEMENT THREAD FUNCTION TO TIMEOUT LEVEL LOADING AND DEATH TIMEOUT
    
    gp_resetPlayer(player);
    
    /** CUSTOM ****************************************************************************/
    
    // Left wall
    gp_createPhyRec((Vector2){
        .x = gp_perX(-1),
        .y = gp_perY(0)
    }, gp_perX(0.5), gp_perY(100));
    
    // Right wall
    gp_createPhyRec((Vector2){
        .x = GetScreenWidth() + gp_perX(5),
        .y = 0
    }, gp_perX(0.5), gp_perY(100));
    
    // Roof
    gp_createPhyRec((Vector2){
        .x = 0,
        .y = gp_perY(-20)
    }, gp_perX(100), gp_perY(5));
    
    // Floor 1
    gp_createPhyRec((Vector2){
        .x = 0,
        .y = gp_perY(73)
    }, gp_perX(19), gp_perY(25));
    
    // Floor 2
    gp_createPhyRec((Vector2){
        .x = gp_perX(33.5),
        .y = gp_perY(63)
    }, gp_perX(8.5), gp_perY(40));
    
    // Floor 3
    gp_createPhyRec((Vector2){
        .x = gp_perX(55.5),
        .y = gp_perY(72)
    }, gp_perX(9), gp_perY(40));
    
    // Floor 4
    gp_createPhyRec((Vector2){
        .x = gp_perX(85),
        .y = gp_perY(72)
    }, gp_perX(25), gp_perY(30));
    
    // Platform
    gp_createPhyRec((Vector2){
        .x = gp_perX(68),
        .y = gp_perY(33.5)
    }, gp_perX(18), gp_perY(13));

    for(int i = 0; i < 2; i++) {
        
        spikes[i] = res.items.spikes;
        spikes[i].disabled = false;
        spikes[i].scale = gp_perX(5) / res.items.spikes.width;
    }
    
    spikes[0].position = (Vector2){gp_perX(75), gp_perY(32.2)};
    spikes[1].position = (Vector2){gp_perX(96), gp_perY(71)};

	for(int i = 0; i < 3; i++) {
        
        lean[i] = res.items.lean;
        lean[i].scale = gp_perX(1.5) / res.items.lean.width;
        lean[i].disabled = true;
    }

    res.items.gelano.disabled = true;
    
    gelanoStatus = res.items.gelano;
    gelanoStatus.scale = gp_perX(3) / res.items.gelano.width;
    gelanoStatus.disabled = player->gelano ? false : true;

    portalTo = res.items.portal;
    portalTo.disabled = true;
    portalTo.scale = gp_perX(3) / res.items.portal.width;
    
    portalFrom = res.items.portal;
    portalFrom.disabled = true;
    portalFrom.scale = gp_perX(3) / res.items.portal.width;
    
    portalBonus = res.items.portal;
    portalBonus.scale = gp_perX(3) / res.items.portal.width;
    portalBonus.rotation = 270;
    portalBonus.position = (Vector2){gp_perX(46), gp_perY(105)};
    
    player->slip = true;

	/************************************************************************************/
    
    // Initialize player's body and default position
    gp_initializeBody(player, game.levelPos.level_4);
}

void LevelFourDraw(Player *player, ScreenFX *screenFx) {

     // Initialize the level only if not loaded
    if(game.screenLoaded != LEVEL_4) {
        
        LevelFourInit(player);
        PrintDebug(TextFormat("Drawing: %s", screenNames[game.gameScreen]));
        
        // Update game informations
        game.screenLoaded = LEVEL_4;
        game.levelPassed = LEVEL_3;
        
        // Set default fade properties
        gp_resetFx(screenFx);
    }

    // Read user input and interact
    LevelFourRead(player, spikes, &portalBonus);
    
    RunPhysicsStep();
    BeginDrawing();

    // Draw level's background
    gp_drawImage(&res.backgrounds.level4, res.backgrounds.level4.scale);
    
    /** CUSTOM ****************************************************************************/
    
    // Draw the spikes
    for(int i = 0; i < 2; i++) {
        
        gp_drawAsset(&spikes[i], spikes[i].position, spikes[i].scale);
    }

	// Draw the jetlean
	for(int i = 0; i < 3; i++) {

        gp_drawAsset(&lean[i], lean[i].position, lean[i].scale);
    }
    
    // Draw the portal
    gp_drawAsset(&portalTo, portalTo.position, portalTo.scale);
    gp_drawAsset(&portalFrom, portalFrom.position, portalFrom.scale);
    
    gp_drawAsset(&portalBonus, portalBonus.position, portalBonus.scale);

    gp_drawAsset(&gelanoStatus, (Vector2){gp_perX(96), gp_perY(6)}, gelanoStatus.scale);

    /**************************************************************************************/
    
    // Player's info board (lifes, lean, level) and notification
    gp_drawBoard(player);
    
    // Draw player
    gp_drawAsset(&player->asset, player->asset.position, player->asset.scale);
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);
    
    EndDrawing();
}

