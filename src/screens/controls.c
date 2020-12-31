
#include <stdio.h>
#include <raylib.h>

#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "controls.h"

// Function reading user inputs
void ControlsRead() {

    if (IsKeyPressed(KEY_ENTER)) {
        
        PrintDebug("Pressed ENTER, returning to the menu!");
        
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

	ClearBackground(RAYWHITE);

    /*DrawTextureEx(
        res.backgrounds.controls,
        (Vector2){0, 0},
        0.0f, 0.85f, WHITE
    );*/
    
    gp_drawImage(&res.backgrounds.controls, res.backgrounds.controls.scale);
    
    // Draw test
    gp_drawText(
        "Controls are coming!", res.fonts.pixellari,
        (Vector2){0, 0},
        30, CENTER, DARKGRAY
    );
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);

	EndDrawing();
}
