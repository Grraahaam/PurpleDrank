/*******************************************************************************************
*
*   raylib [textures] example - Texture loading and drawing a part defined by a rectangle
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1
#define FRAME_PER_LINE       9
#define SPRITE_LINES         6

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "PurpleDrank - Sprite cheet");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    Texture2D sprite = LoadTexture("../res/player/spritecheet.png");

    Rectangle frameRec = { 0.0f, 0.0f, (float)sprite.width/FRAME_PER_LINE, (float)sprite.height/SPRITE_LINES };
    Vector2 position = {
        screenWidth / 2 - frameRec.width / 2,
        30 + sprite.height + 100
    };
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 8;            // Number of spritesheet frames shown by second
    int line = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        framesCounter++;

        if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > FRAME_PER_LINE - 1) currentFrame = 0;

            frameRec.x = (float)currentFrame*(float)sprite.width/FRAME_PER_LINE;
            frameRec.y = (float)line*(float)sprite.height/SPRITE_LINES;
        }

        if (IsKeyPressed(KEY_RIGHT)) framesSpeed++;
        else if (IsKeyPressed(KEY_LEFT)) framesSpeed--;

        if (framesSpeed > MAX_FRAME_SPEED) framesSpeed = MAX_FRAME_SPEED;
        else if (framesSpeed < MIN_FRAME_SPEED) framesSpeed = MIN_FRAME_SPEED;

        if (IsKeyPressed(KEY_UP)) {
            
            if(line > 0) line--;
            
        } else if (IsKeyPressed(KEY_DOWN)) {
            
            if(line < SPRITE_LINES - 1) line++;
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTexture(sprite,
                screenWidth / 2 - sprite.width / 2,
                20, WHITE);
            
            DrawRectangleLines(
                screenWidth / 2 - sprite.width / 2,
                20, sprite.width, sprite.height, LIME);
            
            DrawRectangleLines(
                screenWidth / 2 - sprite.width / 2 + frameRec.x,
                20 + frameRec.y, frameRec.width, frameRec.height, RED);

            DrawText("FRAME SPEED: ", 155,
                20 + sprite.height + 10, 10, DARKGRAY);
            
            DrawText(TextFormat("%02i FPS", framesSpeed), 565,
                20 + sprite.height + 10, 10, DARKGRAY);
            
            DrawText("RIGHT/LEFT : SPEED", 10, screenHeight - 35, 10, DARKGRAY);
            DrawText("UP/DOWN : SPRITE", 10, screenHeight - 20, 10, DARKGRAY);

            for (int i = 0; i < MAX_FRAME_SPEED; i++)
            {
                if (i < framesSpeed) {
                    
                    DrawRectangle(240 + 21*i, 
                        30 + sprite.height + 5 , 20, 20, RED);
                }
                
                DrawRectangleLines(240 + 21*i, 
                    30 + sprite.height + 5, 20, 20, MAROON);
            }

            DrawTextureRec(sprite, frameRec, position, WHITE);  // Draw part of the texture

            DrawText("(c) The Purple Team", screenWidth - 120, screenHeight - 20, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(sprite);       // Texture unloading

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
