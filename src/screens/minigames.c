
#include <stdio.h>
#include <raylib.h>

#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "minigames.h"

// Function reading user inputs
void MinigamesRead() {

    if (IsKeyPressed(KEY_ENTER)) {
        
        PrintDebug("Minigames => Menu");
        
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

	ClearBackground(BLACK);

    // Draw background
    gp_drawImage(&res.backgrounds.minigames, res.backgrounds.minigames.scale);
    
    // Draw text
    gp_drawText(
        "Minigames are coming!", res.fonts.pixellari,
        (Vector2){0, 0},
        gp_perX(3), CENTER, DARKGRAY
    );
    
    // Draw text
    gp_drawText(
        "Najib and Bilal are working on it", res.fonts.pixellari,
        (Vector2){0, gp_perY(54)},
        gp_perX(1.7), CENTER_X, DARKGRAY
    );
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);

	EndDrawing();
}
