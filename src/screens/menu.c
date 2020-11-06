
#include "raylib.h"

// Function reading user ENTER input
void MenuRead() {
	if (IsKeyPressed('ENTER')) {
		LevelOneDraw();
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
