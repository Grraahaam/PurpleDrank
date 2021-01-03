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
    //const int screenWidth = 800;
    //const int screenHeight = 450;
    
    // Setting game config flags
    SetConfigFlags(FLAG_FULLSCREEN_MODE);

    InitWindow(0, 0, "PurpleDrank - Screen cross fade");

    // Create the fullscreen box
    Rectangle fullscreen = { 0, 0, GetScreenWidth(), GetScreenHeight() };
    
    Texture2D splash = LoadTexture("../res/backgrounds/splash.png");
    Texture2D background = LoadTexture("../res/backgrounds/menu.png");
    Texture2D screen = splash;
    
    Color color = BLACK;
    
    bool exiting = false;
    
    float rotation = 0.0f;
    float alpha = 1.0f;
    float scale_init = 1.45f, scale = scale_init;
    
    int duration = 6;
    int framesCounter = 0;

    SetTargetFPS(FPS);
    
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        
        // Reset animation at any moment
        if (IsKeyPressed(KEY_SPACE)) {
            alpha = 1.0f; //alphaBis = 0.0f;
            rotation = 0.0f; scale = scale_init;
            framesCounter = 0; //first = true;
            screen = splash;
            color = BLACK;
            exiting = false;
        }
        
        // In/decrease animation time
        if (IsKeyPressed(KEY_UP)) ++duration;
        if (IsKeyPressed(KEY_DOWN)) --duration;
        
        if (IsKeyPressed(KEY_KP_ADD)) scale_init += 0.05f;
        if (IsKeyPressed(KEY_KP_SUBTRACT)) scale_init += 0.05f;
        
        //------------------------------------------------------------------------
        
        if (framesCounter < duration * FPS) framesCounter++;
        
        if(framesCounter <= (duration / 5 * 3) * FPS) {
            
           /* fullscreen.height = EaseBounceOut(framesCounter, 100, -90, 120);
            * fullscreen.width = EaseBounceOut(framesCounter, 100, GetScreenWidth(), 120);
            * rotation = EaseQuadOut(framesCounter, 0.0f, 270.0f, 240);
            * fullscreen.height = EaseCircOut(framesCounter, 10, GetScreenWidth(), 120);
            */

            // Easing functions arguments Ease*(currentTime, startPositionX, finalPositionX, duration)
            //Ease[Sine,Expo,Cubic,Quad,Circle,Bounce][In,Out,InOut]
            
            alpha = EaseCubicInOut(framesCounter, 1.0f, -1.0f, duration / 5 * FPS);
            
        } else {
            
            if(framesCounter > (duration / 5 * 4) * FPS) {
                
                color = (Color){242, 215, 255, 255};
                
                screen = background;
                scale = 1.45f;
                
                exiting = false;
                
                alpha = EaseCubicInOut(
                    framesCounter - (duration / 5 * 4) * FPS,
                    1.0f, -1.0f,
                    (duration / 5) * FPS
                ); 
            
            } else {
                
                if(framesCounter > (duration / 5 * 2.5) * FPS) {
                    
                    color = (Color){242, 215, 255, 255};
                    
                    exiting = true;
                    
                    alpha = EaseCubicInOut(
                        framesCounter - (duration / 5 * 2.5) * FPS,
                        1.0f, -1.0f,
                        (duration / 5) * FPS
                    );
                }
                
                //scale = EaseExpoIn(
                scale = EaseExpoInOut(
                    framesCounter - (duration / 5 * 3) * FPS,
                    scale_init, 15.0f,
                    (duration / 5) * FPS
                );
            }
        }
        
        // Correct alpha according the current direction
        alpha = exiting ? 1.0f - alpha : alpha;

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextureEx(
            screen,
            (Vector2){
                (int)(GetScreenWidth() - screen.width * scale) / 2,
                (int)(GetScreenHeight() - screen.height * scale) / 2
            },
            0, scale, WHITE
        );
        
        DrawRectanglePro(
            fullscreen,
            (Vector2){0, 0},
            rotation,
            Fade(color, alpha)
        );

        DrawText(TextFormat("Duration: %i", duration), 10, GetScreenHeight() - 100, 15, BLACK);
        DrawText(TextFormat("scale: %.2f", scale), 10, GetScreenHeight() - 80, 15, BLACK);
        DrawText(TextFormat("scale init: %.2f", scale_init), 10, GetScreenHeight() - 60, 15, BLACK);
        DrawText(TextFormat("Frame: %i", framesCounter), 10, GetScreenHeight() - 40, 15, BLACK);
        DrawText("Reset animation: SPACE", 10, GetScreenHeight() - 20, 15, BLACK);
            
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
