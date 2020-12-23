/*******************************************************************************************
*
*   raylib [text] example - TTF loading and usage
*
*   This example has been created using raylib 1.3.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <raylib.h>
#include "../src/lib/easings.h"

#define GAME_FPS 60

int main(void)
{
    // Initialization
    
    // Setting game config flags
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    SetTraceLogLevel(LOG_NONE);
    SetTraceLogExit(LOG_NONE);

    InitWindow(0, 0, "PurpleDrank - Font effect");

    // NOTE: Textures/Fonts MUST be loaded after Window initialization (OpenGL context is required)

    const char msg[50] = "Purple Drank!";
    int atlasPadding = 5;
    int charPadding = 0;
    
    int timer = 0, duration = 2;
    float subDuration = duration / 2 * GAME_FPS;

    Vector2 fontPosition = { 40, GetScreenHeight()/2 - 50 };
    Vector2 textSize;
    
    float baseSize = 35.0f;
    float fontSize = baseSize;
    float scaledSize = 40.0f;
    float incrementSize = (scaledSize - baseSize) / subDuration;
    
    // Default font generation from TTF font
    Font fontDefault;
    fontDefault.baseSize = 16;
    fontDefault.charsCount = 95;
    
    // Chars array initialization
    fontDefault.chars = LoadFontData(
        "../res/font/Pixellari.ttf",
        fontDefault.baseSize, 0,
        fontDefault.charsCount, FONT_DEFAULT
    );
    
    // Chars texture initialization
    fontDefault.texture = LoadTextureFromImage(
        GenImageFontAtlas(
            fontDefault.chars, &fontDefault.recs,
            fontDefault.charsCount, fontDefault.baseSize,
            atlasPadding, 0
        )
    );
    
    SetTargetFPS(GAME_FPS);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------

        if(timer < duration * GAME_FPS) ++timer;
        else timer = 0;
        
        if(timer <= subDuration && fontSize <= scaledSize) fontSize += incrementSize;
        if(timer > subDuration && fontSize >= baseSize) fontSize -= incrementSize;
        
        textSize = MeasureTextEx(fontDefault, msg, fontSize, charPadding);

        fontPosition.x = GetScreenWidth()/2 - textSize.x/2;
        fontPosition.y = GetScreenHeight()/2 - textSize.y/2 + 15;

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextEx(fontDefault, msg, fontPosition, fontSize, charPadding, BLACK);
        DrawTexture(fontDefault.texture, 10, 10, BLACK);

        DrawText(TextFormat("Font size: %.0f", baseSize), GetScreenWidth() - 240, 10, 20, DARKGRAY);
        DrawText(TextFormat("Render size: %2.f", fontSize), GetScreenWidth() - 240, 35, 20, DARKGRAY);
        
        DrawText(TextFormat("Screen width: %i", GetScreenWidth()), GetScreenWidth() - 240, 60, 20, MAROON);
        DrawText(TextFormat("Screen height: %i", GetScreenHeight()), GetScreenWidth() - 240, 85, 20, MAROON);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadFont(fontDefault);    // Default font unloading
    
    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
