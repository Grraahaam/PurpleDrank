
#include <stdio.h>
#include "raylib.h"

extern int screenWidth, screenHeight;
extern Texture2D background;

// Function reading user ENTER input
void MenuRead() {
	if (IsKeyPressed('ENTER')) {
		puts("Pressed ENTER, starting level one!");
		//LevelOneDraw();
	}
}

// Function drawing the menu screen
void MenuDraw() {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	DrawTexture(background, screenWidth/2 - background.width/2, screenHeight/2 - background.height/2, WHITE);
	DrawText("PRESS ENTER TO PLAY", 220, 80, 30, WHITE);

	EndDrawing();

}
