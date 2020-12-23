
#include <raylib.h>
//#include <stdio.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "level1.h"

// Return true if player fall into the hole
bool l1_collisionHole(Player *player) {
    
	return CheckCollisionRecs(
        // Player
        (Rectangle){
            .x = player->asset.position.x, .y = player->asset.position.y,
            .width = player->asset.swidth, .height = player->asset.sheight
        },
        // Hole
        (Rectangle){
            .x = 415, .y = screenHeight + player->asset.sheight,
            .width = 245, .height = 5
        }
    );
}

//Return true if player reach the right side of the screen
bool l1_collisionRightWall(Player *player) {
    
	return CheckCollisionRecs(
        // Player
        (Rectangle){
            .x = player->asset.position.x, .y = player->asset.position.y,
            .width = player->asset.swidth, .height = player->asset.sheight
        },
        // Right wall
        (Rectangle){
            .x = screenWidth + player->asset.swidth, .y = 0,
            .width = 5, .height = screenHeight
        }
    );
}

// Function checking player's collisions with other physic bodies
void l1_readCollisions(Player *player) {

    // Player fall into the hole
	if (l1_collisionHole(player)) {
        
        // Reset player's position
        player->body->position = game.levelPos.level_1;
        
        // Decrement player's lifes
		--player->lives;

    // Player reached the next level
	} else if (l1_collisionRightWall(player) ) {
        
        // Update its info board, and switch level
		game.levelPassed = LEVEL_1;
		game.gameScreen = LEVEL_2;
	}
}

// Reading player in-game interaction
void LevelOneRead(Player *player) {

    gp_readPlayer(player);
	l1_readCollisions(player);
}

// Function generating physic bodies 
void LevelOneInit(Player *player) {
    
    ResetPhysics();
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    PrintDebug(TextFormat("Initializing: %s", screenNames[game.gameScreen]));
    
    gp_resetNotification();
    
    // Initialize physic game box
    //gp_initializeGameBorders();
    
    //TODO: PUT THIS INTO A GLOBAL FUNCTION TO Initialize BASE PROPERTIES FOR PLAYER AT EACH LEVEL
    //TODO: IMPLEMENT THREAD FUNCTION TO TIMEOUT LEVEL LOADING AND DEATH TIMEOUT
    player->slip = false;
    
    /** CUSTOM ****************************************************************************/
    
    // CreatePhysicsBodyRectangle(Vector2 position{x,y}, width, height, density)
    // Create floor and walls physics bodies (fixed)
    PhysicsBody floor_left = CreatePhysicsBodyRectangle(
        (Vector2){
            .x = 190,
            .y = 350
        }, 445, 170, 0
    ); floor_left->enabled = false;
    
    PhysicsBody floor_right = CreatePhysicsBodyRectangle(
        (Vector2){
            .x = screenWidth,
            .y = 360
        }, 290, 150, 0
    ); floor_right->enabled=false;
    
    PhysicsBody platform = CreatePhysicsBodyRectangle(
        (Vector2){
            .x = 535,
            .y = 340
        }, 100, 70, 0
    ); platform->enabled = false;
    
    PhysicsBody wall_left = CreatePhysicsBodyRectangle(
        (Vector2){
            .x = 0,
            .y = screenHeight / 2
        }, 5, screenHeight, 0
    ); wall_left->enabled = false;
    
    /************************************************************************************/
    
    // Set default position
    player->asset.position = game.levelPos.level_1;
    
    gp_initializeBody(player);
}

void LevelOneDraw(Player *player, ScreenFX *screenFx) {
    
    // Initialize the level only if not loaded
    if(game.screenLoaded != LEVEL_1) {
        
        LevelOneInit(player);
        PrintDebug(TextFormat("Drawing: %s", screenNames[game.gameScreen]));
        
        // Update game informations
        game.screenLoaded = LEVEL_1;
        game.levelPassed = MENU;
        
        // Set default fade properties
        gp_resetFx(screenFx);
        
        // THIS ALLOWS THE PLAYER TO MOVE EVEN AFTER SUCCESS AND RELOAD
        //TODO: PUT INTO A FUNCTION OR CHECK WHY resetPlayer() in success.c
        player->can_move = true;
    }
    
    // Read user input and interact
    LevelOneRead(player);

    RunPhysicsStep();
    BeginDrawing();

    // Draw level's background
    DrawTextureEx(res.backgrounds.level1, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
    
    /** CUSTOM ****************************************************************************/
    
    // No additional items
    
    /**************************************************************************************/

    // Player's info board (lifes, lean, level) and notification
    gp_drawBoard(player);
    
    // Draw player
    gp_drawAsset(&player->asset, player->asset.position, player->asset.scale);
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);
    
    EndDrawing();
}
