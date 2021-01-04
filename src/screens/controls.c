
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
        "Controls", res.fonts.pixellari,
        (Vector2){0, gp_perY(5)},
        gp_perX(8), CENTER_X, DARKGRAY
    );
    
    // Draw text
    gp_drawText(
        "FOR QWERTY KEYBORD", res.fonts.pixellari,
        (Vector2){0, gp_perY(18)},
        gp_perX(2), CENTER_X, DARKGRAY
    );
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);

	EndDrawing();
}
