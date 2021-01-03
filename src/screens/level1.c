
#include <raylib.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "level1.h"

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
            .x = GetScreenWidth() + player->asset.swidth, .y = 0,
            .width = 5, .height = GetScreenHeight()
        }
    );
}

// Function checking player's collisions with other physic bodies
void l1_readCollisions(Player *player) {

    if (l1_collisionRightWall(player)) {
        
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
    
    //TODO: IMPLEMENT THREAD FUNCTION TO TIMEOUT LEVEL LOADING AND DEATH TIMEOUT
    
    gp_resetPlayer(player);
    
    /** CUSTOM ****************************************************************************/
    
    gp_createPhyRec((Vector2){
        .x = gp_perX(-1),
        .y = gp_perY(0)
    }, gp_perX(0.5), gp_perY(100));

    gp_createPhyRec((Vector2){
        .x = gp_perX(0),
        .y = gp_perY(59)
    }, gp_perX(52), gp_perY(40));
    
    gp_createPhyRec((Vector2){
        .x = gp_perX(83),
        .y = gp_perY(64)
    }, gp_perX(30), gp_perY(35));
    
    gp_createPhyRec((Vector2){
        .x = gp_perX(60.5),
        .y = gp_perY(68)
    }, gp_perX(11), gp_perY(13));
    
    /************************************************************************************/
    
    // Initialize player's body and default position
    gp_initializeBody(player, game.levelPos.level_1);
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
    }
    
    // Read user input and interact
    LevelOneRead(player);

    RunPhysicsStep();
    BeginDrawing();

    // Draw level's background
    gp_drawImage(&res.backgrounds.level1, res.backgrounds.level1.scale);
    
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
