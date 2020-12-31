
#include <stdio.h>
#include <raylib.h>

#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "gameover.h"

char* subTitles[] = {
    "Do you know how to use a keyboard?",
    "Are you good at something?",
    "Why do you play this game?",
    "Shame on you, stop trying!",
    "Come on, quit the game!"
};

char* subtitle;

// Function reading user ENTER input
void GameoverRead(Player *player) {

	if (IsKeyPressed(KEY_ENTER)) {
        
		PrintDebug("Pressed ENTER, returning to the menu!");
        
        gp_initPlayer(player);
        
        game.levelPassed = MENU;
		game.gameScreen = MENU;
	}
}

// Function drawing the menu screen
void GameoverDraw(Player *player, ScreenFX *fadeFx, ScreenFX *textBounceFx) {

    // Initialize the level only if not loaded
    if(game.screenLoaded != GAMEOVER) {
        
        // Update game informations
        game.screenLoaded = GAMEOVER;
        
        // Set default fade properties
        gp_resetFx(fadeFx);
        
         // Set default fade properties
        gp_resetFx(textBounceFx);
        
        textBounceFx->duration = 2;
        textBounceFx->scaleBase = 90;
        textBounceFx->scaleFinal = textBounceFx->scaleBase + 10;
        textBounceFx->scale = textBounceFx->scaleBase;
        textBounceFx->color = RAYWHITE;
        
        subtitle = subTitles[rand() % 4];
    }
    
    GameoverRead(player);
    
    BeginDrawing();

    ClearBackground(RAYWHITE);

    /*DrawTextureEx(
        res.backgrounds.gameover,
        (Vector2){0, 0},
        0.0f, 0.85f, WHITE
    );*/
    
    gp_drawImage(&res.backgrounds.gameover, res.backgrounds.gameover.scale);
    
    // Draw title
    gp_drawBounceText(
        "Wasted!", res.fonts.pixellari, // text, font style
        (Vector2){0, 0}, CENTER, // position, align
        textBounceFx // text effect
    );
    
    // Draw sub-title
    gp_drawText(
        subtitle, res.fonts.pixellari,
        (Vector2){0, GetScreenHeight() / 2 + 60},
        25, CENTER_X, RAYWHITE
    );
    
    // Drawing the fade in effect
    gp_drawFade(fadeFx);

    EndDrawing();
}
