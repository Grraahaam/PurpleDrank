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

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    
    /*int screenWidth;
    int screenHeight;*/
    
    // Setting game config flags
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    SetTraceLogLevel(LOG_NONE);
    SetTraceLogExit(LOG_NONE);

    InitWindow(0, 0, "PurpleDrank - Font drawing");

    // NOTE: Textures/Fonts MUST be loaded after Window initialization (OpenGL context is required)

    const char msg[50] = "Purple Drank!";
    int charsPadding = 0;

    // Default font generation from TTF font
    Font fontDefault; // = { 0 };
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
            charsPadding, 0
        )
    );

    Vector2 fontPosition = { 40, GetScreenHeight()/2 - 50 };
    Vector2 textSize;
    
    float fontSize = 16.0f;
    
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        fontSize += GetMouseWheelMove() * 8.0f;

        if (fontSize < 10) fontSize = 10;

        textSize = MeasureTextEx(fontDefault, msg, fontSize, 0);

        fontPosition.x = GetScreenWidth()/2 - textSize.x/2;
        fontPosition.y = GetScreenHeight()/2 - textSize.y/2 + 15;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextEx(fontDefault, msg, fontPosition, fontSize, 0, BLACK);
        DrawTexture(fontDefault.texture, 10, 10, BLACK);

        DrawText("Font size: 16", GetScreenWidth() - 240, 10, 20, DARKGRAY);
        DrawText(TextFormat("Render size: %2.f", fontSize), GetScreenWidth() - 240, 35, 20, DARKGRAY);
        
        DrawText(TextFormat("Screen width: %i", GetScreenWidth()), GetScreenWidth() - 240, 60, 20, MAROON);
        DrawText(TextFormat("Screen height: %i", GetScreenHeight()), GetScreenWidth() - 240, 85, 20, MAROON);
        
        DrawText("Use MOUSE WHEEL to SCALE text!", 10, GetScreenHeight() - 30, 20, DARKGRAY);
        

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadFont(fontDefault);    // Default font unloading
    
    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
