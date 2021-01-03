/*******************************************************************************************
*
*   raylib [textures] example - Texture source and destination rectangles
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <stdio.h>
#include "raylib.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    float scale = 1;
    int curScreen = 0;
    int screenAmount = 10;
    int frameX, frameY;
    
    InitWindow(screenWidth, screenHeight, "PurpleDrank - Screen sprite cheet drawing");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    //Texture2D item = LoadTexture("../res/items/spritecheet_assets.png");
    Texture2D screens = LoadTexture("../res/backgrounds/spritecheet_screens.png");
    
    // Level 1
    int frameWidth = 952;
    int frameHeight = 536;
    frameX = 2251; frameY = 550;
    scale = 0.85;
    
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        
        // Dynamic scaling
        if(IsKeyPressed(KEY_KP_ADD)) scale += 0.1;
        else if(IsKeyPressed(KEY_KP_SUBTRACT)) scale -= 0.1;

        // Dynamic sprite line selection
        if (IsKeyPressed(KEY_LEFT)) {
            
            if(curScreen > 0) curScreen--;
            
        } else if (IsKeyPressed(KEY_RIGHT)) {
            
            if(curScreen < screenAmount - 1) curScreen++;
        }
        
        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw middle lines
            DrawLine(
                (int)(screenWidth/2), 0,
                (int)(screenWidth/2), screenHeight,
            LIGHTGRAY);
            DrawLine(
                0, (int)(screenHeight/2),
                screenWidth, (int)(screenHeight/2),
            LIGHTGRAY);

            // NOTE: Using DrawTexturePro() we can easily rotate and scale the part of the texture we draw
            // sourceRec defines the part of the texture we use for drawing
            // destRec defines the rectangle where our texture part will fit (scaling it to fit)
            // origin defines the point of the texture used as reference for rotation and scaling
            // rotation defines the texture rotation (using origin as rotation point)
           DrawTexturePro(screens,
                // Select corresponding frame from original spritecheet
                (Rectangle){
                    .x = frameX, .y = frameY,
                    .width = frameWidth, .height = frameHeight
                },
                // Draw its scaled version at the matching position
                (Rectangle){
                    .x = (GetScreenWidth() - frameWidth * scale) / 2,
                    .y = (GetScreenHeight() - frameHeight * scale) / 2,
                    .width = frameWidth * scale,
                    .height = frameHeight * scale
                },
                (Vector2){0,0}, 0, WHITE
            );
            
            /*
            DrawText(TextFormat("Frame speed: %i", framesSpeed), 10, 10, 10, DARKBROWN);
            DrawText(TextFormat("Current line: %i", line), 10, 25, 10, DARKBROWN);
            DrawText(TextFormat("Scale: %.1f", scale), 10, 40, 10, BROWN);
            DrawText(TextFormat("Rotation: %s", rotate ? "enabled" : "disabled"), 10, 55, 10, BROWN);
            
            DrawText("SPACE : ROTATION", 10, screenHeight - 65, 10, DARKBROWN);
            DrawText("KEYPAD PLUS/MINUS : SCALE", 10, screenHeight - 50, 10, DARKBROWN);
            DrawText("RIGHT/LEFT : SPEED", 10, screenHeight - 35, 10, DARKGRAY);
            DrawText("UP/DOWN : SPRITE", 10, screenHeight - 20, 10, DARKGRAY);
            */
            
            /*
            DrawRectangleLinesEx(
                (Rectangle){
                    .x = spritePos.x, .y = spritePos.y,
                    .width = frameWidth, .height = frameHeight
                }, 1, RED);
            
            DrawRectangleLinesEx(
                (Rectangle){
                    .x = (screenWidth/2 - frameWidth*scale/2),
                    .y = (screenHeight/2 - frameHeight*scale/2),
                    .width = frameWidth*scale, .height = frameHeight*scale
                }, 1, GREEN);
            */
            
            //DrawText("(c) The Purple Team", screenWidth - 120, screenHeight - 20, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(screens);        // Texture unloading

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
