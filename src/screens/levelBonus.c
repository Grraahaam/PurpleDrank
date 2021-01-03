

#include <raylib.h>
#include <unistd.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "levelBonus.h"

Asset portal, barrel, lean[6];

// Function checking player's collisions with other physic bodies or items
void lb_readCollisions(Player *player, Asset *portal, Asset *gelano) {

    // Player hit the barrel to enable Gelano
    if(gp_collisionAssets(&player->asset, &barrel) && barrel.version != 1 && !player->gelano) {

        PrintDebug("Hit barrel");
        
        // Change the barrel, to a broken one
        barrel = res.items.barrel_broken;
        barrel.position = (Vector2){gp_perX(53), gp_perY(64)};
        barrel.scale = gp_perX(5) / res.items.barrel_broken.width;
        barrel.version = 1;

        if(!player->gelano) {
            
            gelano->disabled = false;
        }
    }
    
    // Player hit the Gelano box when active
    if(gp_collisionAssets(&player->asset, gelano) &&
       barrel.version == 1 && !player->gelano) {
        
        PrintDebug("Getting Gelano");

        player->gelano = true;
        gelano->scale = 0.8;
        /*res.items.gelano.swidth = res.items.gelano.width * res.items.gelano.scale;
        res.items.gelano.sheight = res.items.gelano.height * res.items.gelano.scale;*/
        gelano->position = (Vector2){
            GetScreenWidth() - gelano->width * gelano->scale - 5,
            gelano->height * gelano->scale + 5
        };
    }
    
    // Player reached the portal
    if(gp_collisionAssets(&player->asset, portal)) {
            
        // Update its info board, and switch level
        game.levelPassed = LEVEL_BONUS;
        game.gameScreen = LEVEL_4;
    }
    
    // Check lean collisions
    for(int i = 0; i < 6; i++) {
    
        if(gp_collisionAssets(&player->asset, &lean[i]) && !lean[i].disabled) {
            
            PrintDebug("Getting Lean");
            
            ++player->lean;
            lean[i].disabled = true;
        }
    }
}

void LevelBonusRead(Player *player, Asset *portal, Asset *gelano) {
    
    gp_readPlayer(player);
    lb_readCollisions(player, portal, gelano);
}
    
void LevelBonusInit(Player *player) {
    
    ResetPhysics();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    PrintDebug(TextFormat("Initializing: %s", screenNames[game.gameScreen]));
    
    gp_resetNotification();
    
    //TODO: IMPLEMENT THREAD FUNCTION TO TIMEOUT LEVEL LOADING AND DEATH TIMEOUT
    
    gp_resetPlayer(player);

    /** CUSTOM ****************************************************************************/
        
    // Left wall
    gp_createPhyRec((Vector2){
        .x = gp_perX(16),
        .y = gp_perY(0)
    }, gp_perX(0.5), gp_perY(100));
    
    // Right wall
    gp_createPhyRec((Vector2){
        .x = gp_perX(88),
        .y = 0
    }, gp_perX(0.5), gp_perY(100));
    
    //Floor left
    gp_createPhyRec((Vector2){
        .x = gp_perX(18),
        .y = gp_perY(82)
    }, gp_perX(14), gp_perY(8));
    
    // Floor middle
    gp_createPhyRec((Vector2){
        .x = gp_perX(35),
        .y = gp_perY(68)
    }, gp_perX(36), gp_perY(20));
    
    // Floor right
    gp_createPhyRec((Vector2){
        .x = gp_perX(73.5),
        .y = gp_perY(82)
    }, gp_perX(13), gp_perY(8));
    
    for(int i = 0; i < 6; i++) {
        
        lean[i] = res.items.lean;
        lean[i].scale = gp_perX(1.8) / res.items.lean.width;;
        lean[i].disabled = (player->lean > 2) ? true : false;
    }
    
    lean[0].position = (Vector2){gp_perX(42.5), gp_perY(33)};
    lean[1].position = (Vector2){gp_perX(42.5), gp_perY(42)};
    lean[2].position = (Vector2){gp_perX(42.5), gp_perY(52)};
    lean[3].position = (Vector2){gp_perX(62.5), gp_perY(33)};
    lean[4].position = (Vector2){gp_perX(62.5), gp_perY(42)};
    lean[5].position = (Vector2){gp_perX(62.5), gp_perY(52)};
    
    res.items.gelano.disabled = (player->gelano) ? false : true;
    res.items.gelano.scale = (player->gelano) ?
        gp_perX(2) / res.items.gelano.width :
        gp_perX(4) / res.items.gelano.width;
    res.items.gelano.position = (player->gelano) ?
        (Vector2){
            GetScreenWidth() - res.items.gelano.width * res.items.gelano.scale - gp_perX(1),
            res.items.gelano.height * res.items.gelano.scale + gp_perY(1)
        } :
        (Vector2){gp_perX(52), gp_perY(33)};

    barrel = (player->gelano) ? res.items.barrel_broken : res.items.barrel_full;
    barrel.scale = gp_perX(5) / res.items.barrel_full.width;
    barrel.position = (Vector2){gp_perX(53), gp_perY(64)};
    barrel.version = (player->gelano) ? 1 : 0;

    portal = res.items.portal;
    portal.scale = gp_perX(4) / res.items.portal.width;
    portal.position = (Vector2){gp_perX(84), gp_perY(67)};

    /************************************************************************************/
    
    // Initialize player's body and default position
    gp_initializeBody(player, game.levelPos.level_bonus);
}

void LevelBonusDraw(Player *player, ScreenFX *screenFx) {

     // Initialize the level only if not loaded
    if(game.screenLoaded != LEVEL_BONUS) {
        
        LevelBonusInit(player);
        PrintDebug(TextFormat("Drawing: %s", screenNames[game.gameScreen]));
        
        // Update game informations
        game.screenLoaded = LEVEL_BONUS;
        
        // Set default fade properties
        gp_resetFx(screenFx);
    }

    // Read user input and interact
    LevelBonusRead(player, &portal, &res.items.gelano);
    
    RunPhysicsStep();
    BeginDrawing();
    
    ClearBackground(BLACK);

    // Draw level's background
    gp_drawImage(&res.backgrounds.level_bonus, res.backgrounds.level_bonus.scale);
    
    /** CUSTOM ****************************************************************************/
    
    // Draw the portal
    gp_drawAsset(&portal, portal.position, portal.scale);

	gp_drawAsset(&barrel, barrel.position, barrel.scale);

	gp_drawAsset(&res.items.gelano, res.items.gelano.position, res.items.gelano.scale);

    // Draw the leans
    for(int i = 0; i < 6; i++) {
        
        gp_drawAsset(&lean[i], lean[i].position, lean[i].scale);
    }

    /**************************************************************************************/
    // Player's info board (lifes, lean, level) and notification
    gp_drawBoard(player);
    
    // Draw player
    gp_drawAsset(&player->asset, player->asset.position, player->asset.scale);
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);
    
    EndDrawing();
}

