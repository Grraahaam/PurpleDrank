
#include <stdio.h>
#include <raylib.h>

#include "../globals.h"
#include "../lib/defines.c"
#include "../screens/gameover.h"

// Function reading user ENTER input
void GameoverRead() {

	if (IsKeyDown(KEY_ENTER)) {
		PrintDebug("Pressed ENTER, returning to the menu!");
		game.gameScreen = MENU;		
	}
}

// Function drawing the menu screen
void GameoverDraw() {

	BeginDrawing();

	ClearBackground(RAYWHITE);

	DrawTexture(backgroundGameover, screenWidth/2 - backgroundGameover.width/2, screenHeight/2 - backgroundGameover.height/2, WHITE);
	//DrawText("PRESS ENTER TO RETRY", 220, 80, 30, WHITE);

	EndDrawing();

	MenuRead();
}
