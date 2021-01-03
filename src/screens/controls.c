
#include <stdio.h>
#include <raylib.h>

#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "controls.h"

// Function reading user inputs
void ControlsRead() {

    if (IsKeyPressed(KEY_ENTER)) {
        
        PrintDebug("Controls => Menu");
        
        game.levelPassed = MENU;
        game.gameScreen = MENU;
    }
}

// Function drawing the controls screen
void ControlsDraw(ScreenFX *screenFx) {

    // Initialize the level only if not loaded
    if(game.screenLoaded != CONTROLS) {
        
        // Update game informations
        game.screenLoaded = CONTROLS;
        
        // Set default fade properties
        gp_resetFx(screenFx);
        
        screenFx->color = (Color){242, 215, 255, 255};
    }
    
    ControlsRead();
    
    BeginDrawing();

    ClearBackground(BLACK);

    // Draw background
    gp_drawImage(&res.backgrounds.controls, res.backgrounds.controls.scale);
    
    // Draw text
    gp_drawText(
        "Controls are coming!", res.fonts.pixellari,
        (Vector2){0, 0},
        gp_perX(3), CENTER, DARKGRAY
    );
    
    // Draw text
    gp_drawText(
        "Milan is working on it", res.fonts.pixellari,
        (Vector2){0, gp_perY(54)},
        gp_perX(1.7), CENTER_X, DARKGRAY
    );
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);

	EndDrawing();
}
