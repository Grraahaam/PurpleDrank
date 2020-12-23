
#include <stdio.h>
#include <raylib.h>

#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "credits.h"

char* cred_names[] = {
    "Hugo Mellouet",
    "Liam Le Strat",
    "Nabil Belfodil",
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
        
        PrintDebug("Pressed ENTER, returning to the menu!");
        
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

    DrawTextureEx(
        res.backgrounds.credits,
        (Vector2){0, 0},
        0.0f, 0.65f, WHITE
    );
    
    int top = 50, margin_mates = 15, margin_desc = 5;
    int size_name = 25, size_desc = 16;
    
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
    
    /*
    // Hugo ------------------------------------------------------
    gp_drawText(
        "@Hugo Mellouet", res.fonts.pixellari,
        (Vector2){0, top},
        size_name, CENTER_X, BLACK
    );
    
    gp_drawText(
        "Level designer and bool expert", res.fonts.pixellari,
        (Vector2){0, top + size_name + margin_desc},
        15, CENTER_X, DARKGRAY
    );
    
    // Liam ------------------------------------------------------
    gp_drawText(
        "@Liam Le Strat", res.fonts.pixellari,
        (Vector2){0, 120},
        size_name, CENTER_X, BLACK
    );
    
    gp_drawText(
        "Level & screen designer", res.fonts.pixellari,
        (Vector2){0, 155},
        15, CENTER_X, DARKGRAY
    );
    
    // Bilal ------------------------------------------------------
    gp_drawText(
        "@Bilal Belfodil", res.fonts.pixellari,
        (Vector2){0, 180},
        size_name, CENTER_X, BLACK
    );
    
    gp_drawText(
        "Minigames and sound designer", res.fonts.pixellari,
        (Vector2){0, 215},
        15, CENTER_X, DARKGRAY
    );
    
    // Najib ------------------------------------------------------
    gp_drawText(
        "@Najib Belfodil", res.fonts.pixellari,
        (Vector2){0, 240},
        size_name, CENTER_X, BLACK
    );
    
    gp_drawText(
        "Minigames designer", res.fonts.pixellari,
        (Vector2){0, 275},
        15, CENTER_X, DARKGRAY
    );
    
    // Milan ------------------------------------------------------
    gp_drawText(
        "@Milan Kot", res.fonts.pixellari,
        (Vector2){0, 300},
        size_name, CENTER_X, BLACK
    );
    
    gp_drawText(
        "Refactor and code conflict resolver", res.fonts.pixellari,
        (Vector2){0, 335},
        15, CENTER_X, DARKGRAY
    );
    
    // Graham ------------------------------------------------------
    gp_drawText(
        "@Graham Laignel", res.fonts.pixellari,
        (Vector2){0, 360},
        size_name, CENTER_X, BLACK
    );
    
    gp_drawText(
        "Game engine and structure", res.fonts.pixellari,
        (Vector2){0, 395},
        15, CENTER_X, DARKGRAY
    );*/
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);

	EndDrawing();
}
