
#include <stdio.h>
#include <raylib.h>

#include "../globals.h"
#include "../lib/defines.c"
#include "../screens/success.h"

// Function reading user ENTER input
void SuccessRead() {
	if (IsKeyPressed(KEY_ENTER)) {
		PrintDebug("Pressed ENTER, returning to the menu!");
		game.gameScreen = MENU;
	}
}

// Function drawing the menu screen
void SuccessDraw() {

	BeginDrawing();

	ClearBackground(RAYWHITE);

	DrawTexture(backgroundSuccess, screenWidth/2 - backgroundSuccess.width/2, screenHeight/2 - backgroundSuccess.height/2, WHITE);

	EndDrawing();

	SuccessRead();
}
