

#include <raylib.h>
#include <unistd.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "levelBonus.h"

Asset barrel;
Asset lean[6];
Asset gelanoStatus;

// Return true if player hit the barrel
bool lb_collisionBarrel(Player *player) {
	return CheckCollisionRecs (
        // Player
    	(Rectangle){
            .x = player->asset.position.x, .y = player->asset.position.y,
            .width = player->asset.swidth, .height = player->asset.sheight
        },
        // Barrel
        (Rectangle){
            .x = barrel.position.x, barrel.position.y,
            .width = barrel.swidth, .height = barrel.sheight
        }
	);
}

bool lb_collisionGelano(Player *player) {
	return CheckCollisionRecs (
        // Player
    	(Rectangle){
            .x = player->asset.position.x, .y = player->asset.position.y,
            .width = player->asset.swidth, .height = player->asset.sheight
        },
        // Hole1
        (Rectangle){
            .x = res.items.gelano.position.x, res.items.gelano.position.y,
            .width = res.items.gelano.swidth, .height = res.items.gelano.sheight
        }
	);
}

bool lb_collisionPortal(Player *player) {

    return CheckCollisionRecs(
        // Player
        (Rectangle){
            .x = player->asset.position.x, .y = player->asset.position.y,
            .width = player->asset.swidth, .height = player->asset.sheight
        },
        // Portal
        (Rectangle){
            .x = res.items.portal.position.x, .y = res.items.portal.position.y,
            .width = 25, .height = 100
        }
    );
}

// Check collision between player and all the lean items
bool lb_collisionLean(Player *player) {
    
    for(int i = 0; i < 6; i++) {
        
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
void lb_readCollisions(Player *player) {

    if(lb_collisionBarrel(player)) {

        barrel.disabled = true;
		
        if(!player->gelano) {
            res.items.gelano.disabled = false;
        }
    }
	
	else if(lb_collisionGelano(player)) {
		PrintDebug("Getting gelano");

		player->gelano = true;
		res.items.gelano.disabled = true;
        gelanoStatus.disabled = false;
        
	} else if(lb_collisionLean(player)) {

        ++player->lean;

    // Player reached the portal
    } else if(lb_collisionPortal(player)) {
          
		// Update its info board, and switch level
		game.levelPassed = LEVEL_BONUS;
		game.gameScreen = LEVEL_4;
            
    // Clean the notification if not needed
	} else {
        
        game.notification.message = "";
    }	

}

void LevelBonusRead(Player *player) {
    
    gp_readPlayer(player);
    lb_readCollisions(player);
}
    
void LevelBonusInit(Player *player) {
    
    ResetPhysics();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    PrintDebug(TextFormat("Initializing: %s", screenNames[game.gameScreen]));
    
    gp_resetNotification();
    
    //TODO: PUT THIS INTO A GLOBAL FUNCTION TO Initialize BASE PROPERTIES FOR PLAYER AT EACH LEVEL
    //TODO: IMPLEMENT THREAD FUNCTION TO TIMEOUT LEVEL LOADING AND DEATH TIMEOUT
    player->slip = false;

    /** CUSTOM ****************************************************************************/
        
    // Create floor and walls rectangle physics body
    
	PhysicsBody floor1 = CreatePhysicsBodyRectangle((Vector2){ 205, 390 }, 110, 40, 10);
    PhysicsBody floor2 = CreatePhysicsBodyRectangle((Vector2){ 430, 360 }, 290, 100, 10);
    PhysicsBody floor3 = CreatePhysicsBodyRectangle((Vector2){ 650, 390 }, 110, 40, 10);

    PhysicsBody wall_left = CreatePhysicsBodyRectangle(
        (Vector2){ 130, screenHeight/2 },
        10, screenHeight*2, 10
    );
    PhysicsBody wall_right = CreatePhysicsBodyRectangle(
        (Vector2){ 730, screenHeight/2 },
        10, screenHeight*2, 10
    );
    
	floor1->enabled = false;
	floor2->enabled = false;
	floor3->enabled = false;
    wall_left->enabled = false;
    wall_right->enabled = false;

    for(int i = 0; i < 6; i++) {
        
        lean[i] = res.items.lean;
        lean[i].scale = 0.18;
        lean[i].swidth = lean[i].width * lean[i].scale;
        lean[i].sheight = lean[i].height * lean[i].scale;
        lean[i].disabled = (player->lean > 2) ? true : false;
    }
    
    lean[0].position = (Vector2){340, 150};
    lean[1].position = (Vector2){340, 200};
    lean[2].position = (Vector2){340, 250};
    lean[3].position = (Vector2){500, 150};
    lean[4].position = (Vector2){500, 200};
    lean[5].position = (Vector2){500, 250};
   
    gelanoStatus = res.items.gelano;
    gelanoStatus.disabled = (player->gelano) ? false : true;
    gelanoStatus.scale = 0.8;
    res.items.gelano.position = (Vector2){410, 150};
    res.items.gelano.disabled = true;

	barrel = res.items.barrel_full;
    barrel.scale = 1.3;
    barrel.swidth = barrel.width * barrel.scale;
    barrel.sheight = barrel.height * barrel.scale;
    barrel.position = (Vector2){430, 290};
    barrel.disabled = (player->gelano) ? true : false;
		
    res.items.portal.position = (Vector2){640, 270};

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
        
        // THIS ALLOWS THE PLAYER TO MOVE EVEN AFTER SUCCESS AND RELOAD
        //TODO: PUT INTO A FUNCTION OR CHECK WHY resetPlayer() in success.c
        player->can_move = true;
    }

    // Read user input and interact
    LevelBonusRead(player);
    
    RunPhysicsStep();
    BeginDrawing();

    // Draw level's background
    DrawTextureEx(res.backgrounds.levelBonus, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
    
    /** CUSTOM ****************************************************************************/
    
    // Draw the portal
    gp_drawAsset(&res.items.portal, res.items.portal.position, res.items.portal.scale);

	gp_drawAsset(&barrel, barrel.position, barrel.scale);

	gp_drawAsset(&res.items.gelano, res.items.gelano.position, res.items.gelano.scale);

    gp_drawAsset(&res.items.gelano, res.items.gelano.position, res.items.gelano.scale);

    gp_drawAsset(&gelanoStatus,(Vector2){ 750, 45,}, gelanoStatus.scale);

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

