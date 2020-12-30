
#include <stdio.h>
#include <raylib.h>

#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "menu.h"

// Define the action choice variable (buttons)
int choice = 0;
char* btnStr[] = {"Play game", "Select player", "Mini-games", "Controls", "Credits", "Quit"};
GameScreen screens[] = {LEVEL_1, SELECT_PLAYER, MINIGAMES, CONTROLS, CREDITS};

// Function drawing the menu buttons
void menu_drawButtons(ScreenFX *screenFx/*, ScreenFX *screenFxSmall*/) {
    
    for(int i = 0; i < 6; i++) {
        
        Vector2 textSize;
        Vector2 textPosition;
        TextAlign align;
        
        // Original buttons
        if(i < 3) {
            
            align = CENTER_X;
            textPosition = (Vector2){
                0,
                GetScreenHeight() / 2 + 10 + i * (40 + 5)
            };
            
        // Reduced buttons
        } else {
            
            if(i != 5) {
                
                align = CUSTOM;
                textSize = MeasureTextEx(
                    res.fonts.pixellari, btnStr[i],
                    18, 0
                );
                
                if(i == 3)
                    textSize.x = -textSize.x - 4;
                else if(i == 4)
                    textSize.x = 4;
                
                textPosition = (Vector2){
                    GetScreenWidth() / 2 + textSize.x,
                    GetScreenHeight() / 2 + 10 + 3 * (40 + 5)
                };
                
            } else {
                
                align = CENTER_X;
                textPosition = (Vector2){
                    0,
                    GetScreenHeight() / 2 + 10 + 3 * (40 + 5) + (18 + 5)
                };
            }
        }
        
        // If current button is selected, draw select animation (bounce)
        if(i == choice) {
            
            if(i < 3) {
                
                screenFx->scaleBase = 35;
                screenFx->scaleFinal = screenFx->scaleBase + 5;
                
                if(screenFx->scale < screenFx->scaleBase) screenFx->scale = screenFx->scaleBase;
            
            } else {
                
                screenFx->scaleBase = 18;
                screenFx->scaleFinal = screenFx->scaleBase + 2;
                
                if(screenFx->scale > screenFx->scaleFinal) screenFx->scale = screenFx->scaleFinal;
            }
            
            gp_drawBounceText(
                btnStr[i], res.fonts.pixellari, // text, font style
                textPosition, align, // position, align
                screenFx // text effect
            );
            
        // Draw un-selected buttons
        } else {
            
            if(i < 3) {
                
                gp_drawText(
                    btnStr[i], res.fonts.pixellari,
                    textPosition, 35,
                    align, DARKGRAY
                );
            
            } else {
                
                gp_drawText(
                    btnStr[i], res.fonts.pixellari,
                    textPosition, 18,
                    align, DARKGRAY
                );
            }
        }    
    }
}

// Function reading user inputs
void MenuRead() {
    
    if(IsKeyPressed(KEY_ENTER)) {
        
        PrintDebug(TextFormat("Menu => %s", btnStr[choice]));
        
        // If exiting game
        if(choice == 5) game.quit = true;
        // Automatically handle user's action
        else game.gameScreen = screens[choice];
    }
    
    // Navigating up (arrow or tab+shift)
    if(IsKeyPressed(KEY_UP) || (IsKeyPressed(KEY_TAB) && IsKeyDown(KEY_LEFT_SHIFT))) {
        
        choice = choice <= 0 ? 5 : choice - 1;
    
    // Navigating down (arrow or tab)
    } else if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_TAB)) {
        
        choice = choice >= 5 ? 0 : choice + 1;
    }
}

// Function drawing the menu screen
void MenuDraw(ScreenFX *crossFadeFx, ScreenFX *textBounceFx) {
    
    // Initialize the level only if not loaded
    if(game.screenLoaded != MENU) {
        
        PrintDebug(TextFormat("Initializing: %s", screenNames[game.gameScreen]));
        PrintDebug(TextFormat("Drawing: %s", screenNames[game.gameScreen]));
        
        // Update game informations
        game.screenLoaded = MENU;
        
        // Reset the choice
        choice = 0;
        
         // Set default cross fade properties
        gp_resetFx(crossFadeFx);
        
        // Set default cross fade properties
        gp_resetFx(textBounceFx);
        
        textBounceFx->duration = 2;
        textBounceFx->scaleBase = 35;
        textBounceFx->scaleFinal = textBounceFx->scaleBase; + 5;
        textBounceFx->scale = textBounceFx->scaleBase;
        textBounceFx->color = BLACK;
        
        // If game just launched
        if(game.levelPassed == NULL) {
        
            crossFadeFx->duration = 6;
            crossFadeFx->timeout = 3;
            crossFadeFx->scale = res.backgrounds.splash.scale;
        
        // If game already started and returning to the menu
        } else {
            
            crossFadeFx->color = (Color){242, 215, 255, 255};
        }
    }
    
    // Only read user input when the animation is done
    if(crossFadeFx->timer >= (crossFadeFx->duration - 1.75) * GAME_FPS) {
        
        MenuRead();
    }
    
    BeginDrawing();

    ClearBackground(RAYWHITE);
    
    // If game just launched
    if(game.levelPassed == NULL) {
        
        gp_drawCrossFade(&res.backgrounds.splash, &res.backgrounds.menu, crossFadeFx);
        
        // Animation done, draw the menu buttons and the player animation
        if(crossFadeFx->timer >= (crossFadeFx->duration - 1.75) * GAME_FPS) {
            
            menu_drawButtons(textBounceFx);
            
            gp_drawAsset(
                &res.items.player_idle,
                (Vector2){40, GetScreenHeight() - res.items.player_idle.scale * res.items.player_idle.height - 9},
                res.items.player_idle.scale
            );
        }
        
    } else {
        
        // Draw MENU background
        DrawTextureEx(
            res.backgrounds.menu.screen,
            (Vector2){0, 0},
            0.0f, res.backgrounds.menu.scale, WHITE
        );
        
        // Draw the menu buttons
        menu_drawButtons(textBounceFx);
        
        // Draw the IDLE player animation
        gp_drawAsset(
            &res.items.player_idle,
            (Vector2){40, GetScreenHeight() - res.items.player_idle.scale * res.items.player_idle.height - 6},
            res.items.player_idle.scale
        );
        
        // Drawing the cross fade in effect
        gp_drawFade(crossFadeFx);
    }

    EndDrawing();
}
