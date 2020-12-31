
#include <stdio.h>
#include <raylib.h>

#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "minigames.h"

// Function reading user inputs
void MinigamesRead() {

    if (IsKeyPressed(KEY_ENTER)) {
        
        PrintDebug("Pressed ENTER, returning to the menu!");
        
        game.levelPassed = MENU;
        game.gameScreen = MENU;
    }
}

// Function drawing the player selection screen
void MinigamesDraw(ScreenFX *screenFx) {

    // Initialize the level only if not loaded
    if(game.screenLoaded != MINIGAMES) {
        
        // Update game informations
        game.screenLoaded = MINIGAMES;
        
        // Set default fade properties
        gp_resetFx(screenFx);
        
        screenFx->color = (Color){242, 215, 255, 255};
    }
    
    MinigamesRead();
    
	BeginDrawing();

	ClearBackground(RAYWHITE);

    /*DrawTextureEx(
        res.backgrounds.minigames,
        (Vector2){0, 0},
        0.0f, 0.85f, WHITE
    );*/
    
    gp_drawImage(&res.backgrounds.controls, res.backgrounds.controls.scale);
    
    // Draw test
    gp_drawText(
        "Minigames are coming!", res.fonts.pixellari,
        (Vector2){0, 0},
        30, CENTER, DARKGRAY
    );
    
    // Draw test
    gp_drawText(
        "Najib and Bilal are working on it", res.fonts.pixellari,
        (Vector2){0, GetScreenHeight() / 2 + 30},
        16, CENTER_X, DARKGRAY
    );
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);

	EndDrawing();
}
