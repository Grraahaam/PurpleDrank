
#include <stdio.h>
#include "raylib.h"
#include "../structure.h"
#include "menu.h"

extern int screenWidth, screenHeight;
extern Texture2D backgroundMenu;

// Function reading user ENTER input
void MenuRead() {
		
	if (IsKeyDown(KEY_ENTER)) {
		puts("Pressed ENTER, starting level one!");
		game.gameScreen = LEVEL_1;		
	}
}

// Function drawing the menu screen
void MenuDraw() {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawTexture(backgroundMenu, screenWidth/2 - backgroundMenu.width/2, screenHeight/2 - backgroundMenu.height/2, WHITE);
	DrawText("PRESS ENTER TO PLAY", 220, 80, 30, WHITE);
	EndDrawing();
	MenuRead();
}
