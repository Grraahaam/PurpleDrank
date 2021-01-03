
#include <stdio.h>
#include <raylib.h>

#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "credits.h"

char* cred_names[] = {
    "Hugo Mellouet",
    "Liam Le Strat",
    "Bilal Belfodil",
    "Najib Belfodil",
    "Milan Kot",
    "Graham Laignel"
};

char* cred_desc[] = {
    "Level designer and bool expert",
    "Level and screen designer",
    "Minigames and sound designer",
    "Minigames designer and beta tester",
    "Refactor and code conflicts resolver",
    "Game engine and structure"
};

// Function reading user inputs
void CreditsRead() {

    if (IsKeyPressed(KEY_ENTER)) {
        
        PrintDebug("Credits => Menu");
        
        game.levelPassed = MENU;
        game.gameScreen = MENU;
    }
}

// Function drawing the credits screen
void CreditsDraw(ScreenFX *screenFx) {

    // Initialize the level only if not loaded
    if(game.screenLoaded != CREDITS) {
        
        // Update game informations
        game.screenLoaded = CREDITS;
        
        // Set default fade properties
        gp_resetFx(screenFx);
        
        screenFx->color = (Color){242, 215, 255, 255};
    }
    
    CreditsRead();
    
    BeginDrawing();

    ClearBackground(RAYWHITE);
    
    //Draw background
    gp_drawImage(&res.backgrounds.credits, res.backgrounds.credits.scale);
    
    int top = gp_perX(7), margin_mates = gp_perX(2), margin_desc = gp_perX(1);
    int size_name = gp_perX(3), size_desc = gp_perX(1.5);
    
    for(int i = 0; i < 6; i++) {
        
        int y = top + (i * size_name) + (i * size_desc) + (i * margin_mates) + (i * margin_desc);
        
        // Name
        gp_drawText(
            (char*)TextFormat("@%s", cred_names[i]), res.fonts.pixellari,
            (Vector2){0, y},
            size_name, CENTER_X, BLACK
        );
        
        // Description
        gp_drawText(
            cred_desc[i], res.fonts.pixellari,
            (Vector2){0, y + size_name + margin_desc},
            size_desc, CENTER_X, DARKGRAY
        );
    }
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);

    EndDrawing();
}
