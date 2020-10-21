
#include "raylib.h"

#define PLAYER_JUMP_SPD 350.0f
#define G 400

void LevelOneDraw() {

	Player soinc = { 0 };
	soinc.position = (Vector2){ (float)screenWidth/2, (float)screenHeight/1.8 };
	soinc.canJump = true;
	soinc.sol = true;

	// NOTE: Be careful, background width must be equal or bigger than screen width
	// if not, texture should be draw more than two times for scrolling effect

	Decor decor = { 0 };

	decor.scrollingBack = 0.0f;
	decor.scrollingMid = 0.0f;
	decor.scrollingFore = 0.0f;

	Camera2D camera = { 0 };
	camera.target = (Vector2){ soinc.position.x + 20, soinc.position.y + 20 };
	camera.offset = (Vector2){ screenWidth/2, screenHeight/1.5 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	float deltaTime = GetFrameTime();

	decor.scrollingBack -= 0.2f;

	UpdatePlayer(&soinc, &decor, deltaTime);

	camera.target = (Vector2){ soinc.position.x + 20, soinc.position.y + 20 };

	if(decor.scrollingBack <= -background_lvl1.width*1) decor.scrollingBack = 0;
	if(decor.scrollingMid <= -midground_lvl1.width*1) decor.scrollingMid = 0;
	if(decor.scrollingFore <= -foreground_lvl1.width*1) decor.scrollingFore = 0;

	ClearBackground(GetColor(0x052c46ff));
	BeginMode2D(camera);

	// Draw background image twice
	// NOTE: Texture is scaled twice its size
	DrawTextureEx(background_lvl1, (Vector2){ decor.scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
	DrawTextureEx(background_lvl1, (Vector2){ background_lvl1.width*2 + decor.scrollingBack, 20 }, 0.0f, 2.0f, WHITE);

	// Draw midground image twice
	DrawTextureEx(midground_lvl1, (Vector2){ decor.scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
	DrawTextureEx(midground_lvl1, (Vector2){ midground_lvl1.width*2 + decor.scrollingMid, 20 }, 0.0f, 2.0f, WHITE);

	// Draw foreground image twice
	DrawTextureEx(foreground_lvl1, (Vector2){ decor.scrollingFore, 70 }, 0.0f, 2.0f, WHITE);
	DrawTextureEx(foreground_lvl1, (Vector2){ foreground_lvl1.width*2 + decor.scrollingFore, 70 }, 0.0f, 2.0f, WHITE);

	DrawTextureEx(soincPlayer, soinc.position, 0.0f, 0.2f, WHITE);
	//DrawText(TextFormat("Position y : %f \n Position x : %f",soinc.position.y,soinc.position.x), 10, 180,15, WHITE);

    }
}

void UpdatePlayer(Player *player,Decor *decor, float delta)
{
    if (IsKeyDown(KEY_RIGHT)) player -> position.x += 2;
        else if (IsKeyDown(KEY_LEFT)) player-> position.x -= 2;

    if (IsKeyDown(KEY_SPACE) && player->canJump )
    {
        player->position.y -= 100.0f;
        player->speed = -PLAYER_JUMP_SPD;
        player->canJump = false;
    }

    if(player->position.y <= 250)
    {
        player->position.y -= player->speed*delta;
    }
    else
    {
        player->canJump = true;
    }
}
