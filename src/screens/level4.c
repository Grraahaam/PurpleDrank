#include <raylib.h>
#include <unistd.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "level4.h"

Asset spikes[3];
Asset lean[3];
Asset portalTo;
Asset portalFrom;
Asset gelanoStatus;

// Return true if player fall into the hole
bool l4_collisionHole(Player *player) {
    
	return CheckCollisionRecs (
        // Player
    	(Rectangle){
            .x = player->asset.position.x, .y = player->asset.position.y,
            .width = player->asset.swidth, .height = player->asset.sheight
        },
        // Hole1
        (Rectangle){
            .x = 5, .y = 500,
            .width = screenWidth, .height = 5
        }
	);
}

// Return true if player fall into the hole
bool l4_collisionHoleBonus(Player *player) {
    
	return CheckCollisionRecs (
        // Player
    	(Rectangle){
            .x = player->asset.position.x, .y = player->asset.position.y,
            .width = player->asset.swidth, .height = player->asset.sheight
        },
        // Hole1
        (Rectangle){
            .x = 390, .y = screenHeight - 30,
            .width = 50, .height = 5
        }
	);
}

//Return true if player reach the right side of the screen
bool l4_collisionRightWall(Player *player) {
    
	return CheckCollisionRecs(
        // Player
        (Rectangle){
            .x = player->asset.position.x, .y = player->asset.position.y,
            .width = player->asset.swidth, .height = player->asset.sheight
        },
        // Right wall
        (Rectangle){
            .x = screenWidth, .y = 0,
            .width = 5, .height = screenHeight
        }
    );
}

bool l4_collisionSpikes(Player *player) {
	
	for(int i = 0; i < 3; i++) {
        
        if( CheckCollisionRecs(
            // Player
            (Rectangle){
                .x = player->asset.position.x, .y = player->asset.position.y,
                .width = player->asset.swidth -10, .height = player->asset.sheight
            },
            // Current lean
            (Rectangle){
                .x = spikes[i].position.x, .y = spikes[i].position.y,
                .width = spikes[i].swidth, .height = spikes[i].sheight
            })) {

            return true;
        }
    }
}

bool l4_collisionPortal(Player *player) {

    return CheckCollisionRecs(
        // Player
        (Rectangle){
            .x = player->asset.position.x, .y = player->asset.position.y,
            .width = player->asset.swidth, .height = player->asset.sheight
        },
        // Portal
        (Rectangle){
            .x = portalFrom.position.x + 10, .y = portalFrom.position.y,
            .width = portalFrom.swidth, .height = portalFrom.sheight
        }
    );
}



// Function checking player's collisions with other physic bodies or items
void l4_readCollisions(Player *player) {

    // Player fall into spikes
    if (l4_collisionSpikes(player) || l4_collisionHole(player)) {

        // If the player just fall in the spikes
        if(!player->dead) {
            
            // Set the player as dead and static
            player->dead = true;
            player->can_move = false;
            
            // Decrement player's lifes
            --player->lives;
            
        // Check if player is dead
        } else {
            
            // Set the player alive again and allow movements
            player->dead = false;
            player->can_move = true;
            
            // Time the re-apparition
            //sleep(GAME_DEFAULT_TIMEOUT);
            
            // Reset player's position
            player->body->position = game.levelPos.level_4;
            
        }
        
	} else if (l4_collisionRightWall(player) ) {
        
        // Update its info board, and switch level
		game.levelPassed = LEVEL_4;
		game.gameScreen = VICTORY;
        
	} else if (l4_collisionHoleBonus(player)) {
        
		PrintDebug("Teleporting to Bonus level");
		game.gameScreen = LEVEL_BONUS;
	
    // Player reached the portal
    } else if (l4_collisionPortal(player)) {
        
        if(!portalFrom.disabled && !player->portalPowerUsed) {

            PrintDebug("Teleporting to level 2");
            
            game.notification.message = "Teleporting through the Gob-Portal!";
            game.notification.color = GREEN;
            portalTo.disabled = true;
            portalFrom.disabled = true;
            player->body->position.x = portalTo.position.x;
            player->body->position.y = portalTo.position.y;
            player->portalPowerUsed = true;
        }
            
    // Clean the notification if not needed
	} else {
        
        game.notification.message = "";
    }
}

void LevelFourRead(Player *player) {
    
    gp_readPlayer(player);
    l4_readCollisions(player);
    
    /*if(player->slip) {
        
        if(player->asset.direction == RIGHT) {
            player->body->position.x += 1.0f;
        }
        else {
            player->body->position.x -= 1.0f;
        }
    }*/


	for(int i = 0; i < 3; i++) {
        
		lean[i].position = (Vector2){
            player->asset.position.x - 15 + i * 10,
            player->asset.position.y + 28
        };
	}
	
	if(IsKeyPressed(KEY_R) && !player->portalPowerUsed) {
        
        if(portalTo.disabled) {

            portalTo.position = (Vector2){
                player->asset.position.x + 150,
                player->asset.position.y - 190
            };
            portalFrom.position = (Vector2){
                player->asset.position.x + 40,
                player->asset.position.y
            };
            
            portalTo.disabled = false;	
            portalFrom.disabled = false;
            
        } else {
            
            portalTo.disabled = true;
            portalFrom.disabled = true;
        }
	}

	//Activate jumpLean mode
	if(IsKeyPressed(KEY_V)) {
        
		if(player->lean >= 3 && !player->jetLean ) {

			player->jetLean = true;

			for(int i = 0; i < 3; i++) {
                
				lean[i].disabled = false;
			}
		}
	}

    if(IsKeyPressed(KEY_G)) {

        player->slip = player->gelano ? false : true;
    }

	if(IsKeyPressed(KEY_SPACE) && player->jetLean) {
        
		player->body->velocity.y = -VELOCITY * 4.5;
		player->lean -= 3;
		player->jetLean = false;
        
		for(int i = 0; i < 3; i++) {
            
			lean[i].disabled = true;
		}
	}
}
    
void LevelFourInit(Player *player) {

    ResetPhysics();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    PrintDebug(TextFormat("Initializing: %s", screenNames[game.gameScreen]));
    
    gp_resetNotification();
    
    //TODO: PUT THIS INTO A GLOBAL FUNCTION TO Initialize BASE PROPERTIES FOR PLAYER AT EACH LEVEL
    //TODO: IMPLEMENT THREAD FUNCTION TO TIMEOUT LEVEL LOADING AND DEATH TIMEOUT
    player->slip = false;
    
    /** CUSTOM ****************************************************************************/
        
    // Create floor and walls rectangle physics body
    PhysicsBody floor1 = CreatePhysicsBodyRectangle((Vector2){60, 585},210, 520, 10);
	PhysicsBody floor2 = CreatePhysicsBodyRectangle((Vector2){300, 580},70, 560, 10);
	PhysicsBody floor3 = CreatePhysicsBodyRectangle((Vector2){485, 600},80, 520, 10);
	PhysicsBody floor4 = CreatePhysicsBodyRectangle((Vector2){785, 600},230, 520, 10);
    PhysicsBody platform = CreatePhysicsBodyRectangle((Vector2){620, 190},155, 70, 10);

    PhysicsBody wall_left = CreatePhysicsBodyRectangle(
        (Vector2){-5, screenHeight / 2},
        10, screenHeight * 2, 10
    );
    
    PhysicsBody wall_right = CreatePhysicsBodyRectangle(
        (Vector2){screenWidth + 5, screenHeight / 2},
		10, screenHeight*2, 10
    );
	
	floor1->enabled = false;
	floor2->enabled = false;
	floor3->enabled = false;
	floor4->enabled = false;
    platform->enabled = false;
    wall_left->enabled = false;
    wall_right->enabled = false;
    
    portalTo = res.items.portal;
    portalTo.disabled = true;
    portalFrom = res.items.portal;
    portalFrom.disabled = true;

	res.items.gelano.disabled = true;
    
	for(int i = 0; i < 3; i++) {
        
        spikes[i] = res.items.spikes;
        spikes[i].disabled = false;
        spikes[i].scale = 0.15;
        spikes[i].swidth = spikes[i].width * spikes[i].scale;
        spikes[i].sheight = spikes[i].height * spikes[i].scale;
    }
    
    //spikes[0].position = (Vector2){95, 325};
    spikes[1].position = (Vector2){600, 145};
    spikes[2].position = (Vector2){780, 328};

	for(int i = 0; i < 3; i++) {
        
        lean[i] = res.items.lean;
        lean[i].scale = 0.15;
        lean[i].disabled = true;
    }

    gelanoStatus = res.items.gelano;
    gelanoStatus.scale = 0.8;
    gelanoStatus.disabled = player->gelano ? false : true;

    portalFrom.scale = 0.7;
    portalTo.scale = 0.7;
    portalFrom.swidth = portalFrom.width * portalFrom.scale;
    portalFrom.sheight = portalFrom.height * portalFrom.scale;
    portalTo.swidth = portalTo.width * portalTo.scale;
    portalTo.sheight = portalTo.height * portalTo.scale;
    
    player->slip = true;

	/************************************************************************************/
    // Set default position
    player->asset.position = game.levelPos.level_4;
    gp_initializeBody(player);
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
        
        // THIS ALLOWS THE PLAYER TO MOVE EVEN AFTER SUCCESS AND RELOAD
        //TODO: PUT INTO A FUNCTION OR CHECK WHY resetPlayer() in success.c
        player->can_move = true;
    }

    // Read user input and interact
    LevelFourRead(player);
    
    RunPhysicsStep();
    BeginDrawing();

    // Draw level's background
    DrawTextureEx(res.backgrounds.level4, (Vector2){0,0}, 0.0f, 0.85f, WHITE);
    
    /** CUSTOM ****************************************************************************/
    
    // Draw the spikes
    for(int i = 0; i < 3; i++) {
        
        gp_drawAsset(&spikes[i], spikes[i].position, spikes[i].scale);
    }

	// Draw the jetlean
	for(int i = 0; i < 3; i++) {

        gp_drawAsset(&lean[i], lean[i].position, lean[i].scale);
    }
    
    // Draw the portal
    gp_drawAsset(&portalTo, portalTo.position, portalTo.scale);
    gp_drawAsset(&portalFrom, portalFrom.position, portalFrom.scale);

    gp_drawAsset(&gelanoStatus, (Vector2){750, 45}, gelanoStatus.scale);

    /**************************************************************************************/
    
    // Player's info board (lifes, lean, level) and notification
    gp_drawBoard(player);
    
    // Draw player
    gp_drawAsset(&player->asset, player->asset.position, player->asset.scale);
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);
    
    EndDrawing();
}

