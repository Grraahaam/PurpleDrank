

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
        barrel.position = (Vector2){425, 290};
        barrel.scale = 1.3f;
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
        
    // Create floor and walls rectangle physics body
    
	PhysicsBody floor_left = CreatePhysicsBodyRectangle((Vector2){ 205, 390 }, 110, 40, 10);
    PhysicsBody floor_middle = CreatePhysicsBodyRectangle((Vector2){ 420, 360 }, 270, 100, 10);
    PhysicsBody floor_right = CreatePhysicsBodyRectangle((Vector2){ 650, 390 }, 110, 40, 10);

    PhysicsBody wall_left = CreatePhysicsBodyRectangle(
        (Vector2){ 130, screenHeight/2 },
        10, screenHeight*2, 10
    );
    PhysicsBody wall_right = CreatePhysicsBodyRectangle(
        (Vector2){ 730, screenHeight/2 },
        10, screenHeight*2, 10
    );
    
	floor_left->enabled = false;
	floor_middle->enabled = false;
	floor_right->enabled = false;
    
    wall_left->enabled = false;
    wall_right->enabled = false;

    for(int i = 0; i < 6; i++) {
        
        lean[i] = res.items.lean;
        lean[i].scale = 0.18;
        lean[i].disabled = (player->lean > 2) ? true : false;
    }
    
    lean[0].position = (Vector2){340, 150};
    lean[1].position = (Vector2){340, 200};
    lean[2].position = (Vector2){340, 250};
    lean[3].position = (Vector2){500, 150};
    lean[4].position = (Vector2){500, 200};
    lean[5].position = (Vector2){500, 250};
    
    res.items.gelano.disabled = (player->gelano) ? false : true;
    res.items.gelano.scale = (player->gelano) ? 0.8 : 1;
    res.items.gelano.position = (player->gelano) ?
        (Vector2){
            GetScreenWidth() - res.items.gelano.width * res.items.gelano.scale - 5,
            res.items.gelano.height * res.items.gelano.scale + 5
        } :
        (Vector2){418, 150};

    barrel = (player->gelano) ? res.items.barrel_broken : res.items.barrel_full;
    barrel.scale = 1.3;
    barrel.position = (Vector2){425, 290};
    barrel.version = (player->gelano) ? 1 : 0;

    portal = res.items.portal;
    portal.scale = 0.8f;
    portal.position = (Vector2){650, 280};

    /************************************************************************************/
    // Set default position
    player->asset.position = game.levelPos.level_bonus;
        
    gp_initializeBody(player);
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

