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

#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    int framesCounter = 0;
    int framesSpeed = 10; // Number of spritesheet frames shown by second
    int line = 0;
    int currentFrame = 0;
    int rotation = 0;
    bool rotate = false;
    float scale = 1.5;
    int frameX = 0, frameY = 0;
    Vector2 spritePos = {0,0};
    
    InitWindow(screenWidth, screenHeight, "PurpleDrank - Sprite cheet scaling");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    
    /*
    Texture2D item = LoadTexture("../res/player/spritecheet_darkpurple.png");
    int spritePerLine = 9; int spriteLines = 6;
    int frameWidth = item.width/spritePerLine;
    int frameHeight = item.height/spriteLines;
    */
    
    /*
    Texture2D item = LoadTexture("../res/items/car.png");
    int spritePerLine = 1; int spriteLines = 1;
    int frameWidth = item.width;
    int frameHeight = item.height;
    scale = 0.85;
    */

    Texture2D item = LoadTexture("../res/items/spritecheet_assets.png");
    int spritePerLine = 1; int spriteLines = 1;
    int frameWidth = 241;
    int frameHeight = 478;
    frameX = 1110; frameY = 174;
    scale = 0.85;
    
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Rotate infitely
        if(rotate) {
            if(rotation >= 360) rotation = 0;
            else rotation++;
        }
        
        // Increment the current frame drawed counter
        framesCounter++;

        if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > spritePerLine - 1) currentFrame = 0;

            spritePos.x = (float)currentFrame*(float)frameWidth;
            spritePos.y = (float)line*(float)frameHeight;
        }

        // En/Disable rotation
        if (IsKeyPressed(KEY_SPACE)) {
            if(rotate) rotate = false;
            else rotate = true;
        }
        
        // Dynamic scaling
        if (IsKeyPressed(KEY_KP_ADD)) scale += 0.1;
        else if (IsKeyPressed(KEY_KP_SUBTRACT)) scale -= 0.1;
        
        // Dynamic speeding
        if (IsKeyPressed(KEY_RIGHT)) framesSpeed++;
        else if (IsKeyPressed(KEY_LEFT)) framesSpeed--;

        if (framesSpeed > MAX_FRAME_SPEED) framesSpeed = MAX_FRAME_SPEED;
        else if (framesSpeed < MIN_FRAME_SPEED) framesSpeed = MIN_FRAME_SPEED;

        // Dynamic sprite line selection
        if (IsKeyPressed(KEY_UP)) {
            
            if(line > 0) line--;
            
        } else if (IsKeyPressed(KEY_DOWN)) {
            
            if(line < spriteLines - 1) line++;
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
            DrawTexturePro(item,
                (Rectangle){
                    .x = spritePos.x + frameX, .y = spritePos.y + frameY,
                    .width = frameWidth, .height = frameHeight
                },
                (Rectangle){
                    .x = (screenWidth/2 - frameWidth*scale/2),
                    .y = (screenHeight/2 - frameHeight*scale/2),
                    .width = frameWidth*scale, .height = frameHeight*scale
                }, (Vector2){0,0}, (float)rotation, WHITE);
            
            DrawText(TextFormat("Frame speed: %i", framesSpeed), 10, 10, 10, DARKBROWN);
            DrawText(TextFormat("Current line: %i", line), 10, 25, 10, DARKBROWN);
            DrawText(TextFormat("Scale: %.1f", scale), 10, 40, 10, BROWN);
            DrawText(TextFormat("Rotation: %s", rotate ? "enabled" : "disabled"), 10, 55, 10, BROWN);
            
            DrawText("SPACE : ROTATION", 10, screenHeight - 65, 10, DARKBROWN);
            DrawText("KEYPAD PLUS/MINUS : SCALE", 10, screenHeight - 50, 10, DARKBROWN);
            DrawText("RIGHT/LEFT : SPEED", 10, screenHeight - 35, 10, DARKGRAY);
            DrawText("UP/DOWN : SPRITE", 10, screenHeight - 20, 10, DARKGRAY);
            
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
            
            DrawText("(c) The Purple Team", screenWidth - 120, screenHeight - 20, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(item);        // Texture unloading

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
