
#include <stdio.h>
#include <raylib.h>

#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "select.h"

Asset sleep[3];
Asset clap[3];

// Function reading user inputs
void SelectRead(Player *player) {

    if(IsKeyPressed(KEY_ENTER)) {
        
        PrintDebug("Pressed ENTER, returning to the menu!");
        
        game.levelPassed = MENU;
        game.gameScreen = MENU;
    }
    
    // If move selection
    if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_TAB)) {
    
        int new;
        
        // Reset the current sleep animation
        sleep[player->asset.version].frame.current = 0;
        sleep[player->asset.version].frame.line = 0;
        sleep[player->asset.version].frame.line_frame = 0;
            
        // Change character LEFT or SHIFT+TAB
        if(IsKeyPressed(KEY_LEFT) || (IsKeyPressed(KEY_TAB) && IsKeyDown(KEY_LEFT_SHIFT))) {
            
            // Select the new character's version
            new = player->asset.version == 0 ?
                player->asset.version_count - 1 : player->asset.version - 1;
        }
        
        // Change character RIGHT or TAB
        if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_TAB)) {
            
            // Select the new character's version
            new = player->asset.version == player->asset.version_count - 1 ?
                0 : player->asset.version + 1;
        }
        
        player->asset.version = new;
        
        // Reset the new clap animation
        clap[new].frame.current = 0;
        clap[new].frame.line = 0;
        clap[new].frame.line_frame = 0;
        clap[new].frame.animate = true;
        
        // Reset the select animation
        res.items.player_anim_flip.frame.current = 0;
        res.items.player_anim_flip.frame.line = 0;
        res.items.player_anim_flip.frame.line_frame = 0;
        res.items.player_anim_flip.frame.animate = true;
    }
}

// Function drawing the player selection screen
void SelectDraw(Player *player, ScreenFX *screenFx) {

    // Initialize the level only if not loaded
    if(game.screenLoaded != SELECT_PLAYER) {
        
        // Update game informations
        game.screenLoaded = SELECT_PLAYER;
        
        // Set default fade properties
        gp_resetFx(screenFx);
        
        screenFx->color = (Color){242, 215, 255, 255};
        
        // Initialize the differents character's versions
        for(int i = 0; i < 3; i++) {
            
            float x = 115;
            //x -= res.items.player_anim_sleep.scale * res.items.player_anim_sleep.height - 40;
            x += i * (res.items.player_anim_sleep.scale * res.items.player_anim_sleep.width + 78);
            
            Vector2 position = {
                .x = x,
                .y =  GetScreenHeight() / 2 + 18
            };
            
            sleep[i] = res.items.player_anim_sleep;
            sleep[i].version = i;
            sleep[i].position = position;
            
            clap[i] = res.items.player_anim_clap;
            clap[i].version = i;
            clap[i].position = position;
        }
        
        res.items.player_anim_flip.scale = 2.8;
        res.items.player_anim_flip.position = (Vector2){
            .x = GetScreenWidth() - res.items.player_anim_flip.scale * res.items.player_anim_flip.width - 55,
            .y = GetScreenHeight() / 2 - 22//+ (res.items.player_anim_flip.scale * res.items.player_anim_flip.height) / 2 - 60
        };
    }
    
    SelectRead(player);
    
	BeginDrawing();

	ClearBackground(RAYWHITE);

    DrawTextureEx(
        res.backgrounds.select,
        (Vector2){0, 0},
        0.0f, 0.65f, WHITE
    );
    
    // Draw title
    gp_drawText(
        "Player", res.fonts.pixellari,
        (Vector2){0, 40},
        40, CENTER_X, BLACK
    );
    
    gp_drawText(
        "selection", res.fonts.pixellari,
        (Vector2){0, 83},
        25, CENTER_X, BLACK
    );
    
    // Draw character's versions
    for(int i = 0; i < 3; i++) {
        
        //res.items.player_anim_flip.version = 1;
        Asset *curAsset;
        
        if(player->asset.version == i) curAsset = &clap[i];
        else curAsset = &sleep[i];
            
        gp_drawAsset(
            curAsset,
            curAsset->position,
            curAsset->scale
        );
    }
    
    // Draw current selection
    res.items.player_anim_flip.version = player->asset.version;
    gp_drawAsset(
        &res.items.player_anim_flip,
        res.items.player_anim_flip.position,
        res.items.player_anim_flip.scale
    );
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);

	EndDrawing();
}
