
#include <stdio.h>
#include <raylib.h>

#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "victory.h"

// Function reading user inputs
void VictoryRead(Player *player) {

    if (IsKeyPressed(KEY_ENTER)) {
        
        PrintDebug("Victory => Menu");
        
        gp_initPlayer(player);
        
        game.levelPassed = MENU;
        game.gameScreen = MENU;
    }
}

// Function drawing the victory screen
void VictoryDraw(Player *player, ScreenFX *fadeFx, ScreenFX *textBounceFx) {

    // Initialize the level only if not loaded
    if(game.screenLoaded != VICTORY) {
        
        // Update game informations
        game.screenLoaded = VICTORY;
        
        // Set default fade properties
        gp_resetFx(fadeFx);
        
        // Set default fade properties
        gp_resetFx(textBounceFx);
        
        textBounceFx->duration = 2;
        textBounceFx->scaleBase = gp_perX(10);
        textBounceFx->scaleFinal = textBounceFx->scaleBase + gp_perX(1.25);
        textBounceFx->scale = textBounceFx->scaleBase;
        textBounceFx->color = RAYWHITE;
    }
    
    VictoryRead(player);
    
    BeginDrawing();

    ClearBackground(RAYWHITE);
    
    // Draw background
    gp_drawImage(&res.backgrounds.victory, res.backgrounds.victory.scale);
    
    // Draw title
    gp_drawBounceText(
        "Victory!", res.fonts.pixellari, // text, font style
        (Vector2){0, 0}, CENTER, // position, align
        textBounceFx // text effect
    );
    
    // Draw sub-title
    gp_drawText(
        "Solin destroyed the GobLean!", res.fonts.pixellari,
        (Vector2){0, gp_perY(60)},
        gp_perX(2.4), CENTER_X, RAYWHITE
    );
    
    // Drawing the fade in effect
    gp_drawFade(fadeFx);

    EndDrawing();
}
