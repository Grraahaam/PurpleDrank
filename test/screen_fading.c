/*******************************************************************************************
*
*   raylib [shapes] example - easings box anim
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014-2019 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <raylib.h>

// Required for easing functions
#include "../src/lib/easings.h"

#define FPS 60

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "PurpleDrank - Box animation");

    // Create the fullscreen box
    Rectangle fullscreen = { 0, 0, screenWidth, screenHeight };
    Texture2D background = LoadTexture("../res/backgrounds/level1.png");;
    
    bool exiting = false;
    
    float rotation = 0.0f;
    float alpha = 1.0f;
    int duration = 2;

    int framesCounter = 0;

    SetTargetFPS(FPS);
    
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        
        // Reset animation at any moment
        if (IsKeyPressed(KEY_SPACE)) {
            rotation = 0.0f; alpha = 1.0f;
            framesCounter = 0;
        }
        
        // In/decrease animation time
        if (IsKeyPressed(KEY_UP)) ++duration;
        if (IsKeyPressed(KEY_DOWN)) --duration;
        
        // Switch animation direction
        if (IsKeyPressed(KEY_S)) {
            exiting = exiting ? false : true;
            rotation = 0.0f; alpha = 1.0f;
            framesCounter = 0;
        }
        
        //------------------------------------------------------------------------
        
        if (framesCounter < duration * FPS) framesCounter++;

        // often : c = finalPositionX - startPositionX
        // easing functions arguments Ease*(currentTime, startPositionX, finalPositionX, duration)
        
        //alpha = EaseSineOut(framesCounter, 1.0f, -1.0f, duration * FPS);
        //alpha = EaseExpoInOut(framesCounter, 1.0f, -1.0f, duration * FPS);
        alpha = EaseCubicInOut(framesCounter, 1.0f, -1.0f, duration * FPS);
        
        // Correct alpha according the current direction
        alpha = exiting ? 1.0f - alpha : alpha;
        
        /*
         * fullscreen.height = EaseBounceOut(framesCounter, 100, -90, 120);
         * fullscreen.width = EaseBounceOut(framesCounter, 100, screenWidth, 120);
         * rotation = EaseQuadOut(framesCounter, 0.0f, 270.0f, 240);
         * fullscreen.height = EaseCircOut(framesCounter, 10, screenWidth, 120);
         */

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureEx(
                background,
                (Vector2){0, 0},
                0.0f, 0.85f, WHITE
            );
            
            DrawRectanglePro(
                fullscreen,
                (Vector2){0, 0},
                rotation,
                Fade(BLACK, alpha)
            );

            DrawText(TextFormat("Duration: %i", duration), 10, 15, 15, RAYWHITE);
            DrawText(TextFormat("Direction: %s", exiting ? "Fading out" : "Fading in"), 10, 30, 15, RAYWHITE);
            
            DrawText("Switch direction: S", 10, GetScreenHeight() - 65, 15, RAYWHITE);
            DrawText("Reset animation: SPACE", 10, GetScreenHeight() - 45, 15, RAYWHITE);
            DrawText("In/Decrease duration: UP/DOWN", 10, GetScreenHeight() - 25, 15, RAYWHITE);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
