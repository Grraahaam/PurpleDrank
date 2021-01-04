

#include <raylib.h>
#include <unistd.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "levelBonus.h"

Asset portal, barrel, lean[6];

// Function checking player's collisions with other physic bodies or items
void lb_readCollisions(Player *player, Asset *portal, Asset *gelano, Asset *portal_bonus, Asset *extraLife) {

    // Player hit the barrel to enable power
    if(gp_collisionAssets(&player->asset, &barrel) && barrel.version != 1 && !player->gelano && !player->portal && !player->extraLife) {

        PrintDebug("Hit barrel");
        
        // Change the barrel, to a broken one
        barrel = res.items.barrel_broken;
        barrel.position = (Vector2){gp_perX(53), gp_perY(64)};
        barrel.scale = gp_perX(5) / res.items.barrel_broken.width;
        barrel.version = 1;
        
        int power = rand() % 3;
        
        PrintDebug(TextFormat("Random: %d", power));

        if(!player->gelano && power == 0) {
            PrintDebug("Gelano");
            
            gelano->disabled = false;
        }
        if(!player->portal && power == 1) {
            PrintDebug("Portal");
            
            portal_bonus->disabled = false;
        }
        if(!player->extraLife && power == 2) {
            PrintDebug("Extra Life");
            
            extraLife->disabled = false;
        }
    }
    
    // Player hit the Gelano box when active
    if(gp_collisionAssets(&player->asset, gelano) &&
       barrel.version == 1 && !player->gelano && !gelano->disabled) {
        
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
    
    // Player hit the portal box when active
    if(gp_collisionAssets(&player->asset, portal_bonus) &&
       barrel.version == 1 && !player->portal && !portal_bonus->disabled) {
        
        PrintDebug("Getting Portal Bonus");

        player->portal = true;
        portal_bonus->scale = 0.5;
        /*res.items.gelano.swidth = res.items.gelano.width * res.items.gelano.scale;
        res.items.gelano.sheight = res.items.gelano.height * res.items.gelano.scale;*/
        portal_bonus->position = (Vector2){
            GetScreenWidth() - portal_bonus->width * portal_bonus->scale - 5,
            portal_bonus->height * portal_bonus->scale + 5
        };
    }
    
    // Player hit the power box when active
    if(gp_collisionAssets(&player->asset, extraLife) &&
       barrel.version == 1 && !player->extraLife && !extraLife->disabled) {
        
        PrintDebug("Getting extraLife");

        player->extraLife = true;
        ++player->lives;
        extraLife->disabled = true;
    }
    
    // Player reached the portal
    if(gp_collisionAssets(&player->asset, portal)) {
            
        PlaySound(res.sounds.portal);
            
        // Update its info board, and switch level
        game.levelPassed = LEVEL_BONUS;
        game.gameScreen = LEVEL_4;
        player->bonus_level_visited = true;
    }
    
    // Check lean collisions
    for(int i = 0; i < 6; i++) {
    
        if(gp_collisionAssets(&player->asset, &lean[i]) && !lean[i].disabled) {
            
            PrintDebug("Getting Lean");
            
            ++player->lean;
            lean[i].disabled = true;
            PlaySound(res.sounds.drink);
        }
    }
}

void LevelBonusRead(Player *player, Asset *portal, Asset *gelano, Asset *portal_bonus, Asset *extraLife) {
    
    gp_readPlayer(player);
    lb_readCollisions(player, portal, gelano, portal_bonus, extraLife);
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
        lean[i].disabled = false;
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
    barrel.version = 0;

    portal = res.items.portal;
    portal.scale = gp_perX(4) / res.items.portal.width;
    portal.position = (Vector2){gp_perX(84), gp_perY(67)};
    
    res.items.portal_bonus.disabled = (player->portal) ? false : true;
    res.items.portal_bonus.scale = (player->portal) ?
        gp_perX(1) / res.items.portal_bonus.width :
        gp_perX(2) / res.items.portal_bonus.width;
    res.items.portal_bonus.position = (player->portal) ?
        (Vector2){
            GetScreenWidth() - res.items.portal_bonus.width * res.items.portal_bonus.scale - gp_perX(1),
            res.items.portal_bonus.height * res.items.portal_bonus.scale + gp_perY(1)
        } :
        (Vector2){gp_perX(52), gp_perY(33)};
        
    res.items.extraLife.disabled = (player->extraLife) ? false : true;
    res.items.extraLife.scale = (player->extraLife) ?
        gp_perX(8) / res.items.extraLife.width :
        gp_perX(6.4) / res.items.extraLife.width;
    res.items.extraLife.position = (player->extraLife) ?
        (Vector2){
            GetScreenWidth() - res.items.extraLife.width * res.items.extraLife.scale - gp_perX(1),
            res.items.extraLife.height * res.items.extraLife.scale + gp_perY(1)
        } :
        (Vector2){gp_perX(52), gp_perY(33)};

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
    LevelBonusRead(player, &portal, &res.items.gelano, &res.items.portal_bonus, &res.items.extraLife);
    
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
	gp_drawAsset(&res.items.portal_bonus, res.items.portal_bonus.position, res.items.portal_bonus.scale);
	gp_drawAsset(&res.items.extraLife, res.items.extraLife.position, res.items.extraLife.scale);

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

