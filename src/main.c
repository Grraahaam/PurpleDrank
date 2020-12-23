
// This is the main program file, that is compiled to get the stable executable

#include <stdio.h>
#include <unistd.h>
#include <raylib.h>

//#include "lib/raylib.h"
#include "lib/raymath.h"
#include "lib/physac.h"
#include "lib/defines.c"

// Defines the global variables, structs and enums
#include "globals.h"
#include "gameplay.h"
#include "screens/menu.h"
#include "screens/victory.h"
#include "screens/controls.h"
#include "screens/credits.h"
#include "screens/select.h"
#include "screens/minigames.h"
#include "screens/gameover.h"
#include "screens/level1.h"
#include "screens/level2.h"
#include "screens/level3.h"
#include "screens/level4.h"
#include "screens/levelBonus.h"

// Function declarations
void UpdateScreen(Player *player, ScreenFX *fadeFx, ScreenFX *crossFadeFx, ScreenFX *bounceText);
void LoadResources(void);
void LoadFonts();
void UnloadResources(void);
void PrintDebug(char *str);
void TogleDebugRead();
char *getcwd(char *buf, size_t size);
int GetDir();

int main(int argc, char *argv[]) {
    
    // Initialize player object
    Player player;

	// Switching ON/OFF the debug mode (./game or ./game debug)
	if(argc == 2 && strcmp(argv[1], "debug") == 0) { DEBUG = true; }
	else { DEBUG = false; }

	// Check if the program is launched from within the sources or from outside
	if(access("/usr/share/PurpleDrank", F_OK) == 0) RESOURCES_DIR = "/usr/share/PurpleDrank";
    else RESOURCES_DIR = "res";
        
	// Defining variables values
    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;
    
    // Set the game window fullscreen
    //SetConfigFlags(FLAG_FULLSCREEN_MODE);

	// Launching game start-up requirements
	if(DEBUG) GetDir();
    else {
        
        //Disable raylib engine logs
        SetTraceLogLevel(LOG_NONE);
        SetTraceLogExit(LOG_NONE);
    }

    // Init the window and hide the cursor within it
	PrintDebug("Launching window!");
	InitWindow(screenWidth, screenHeight, GAME_NAME);
    HideCursor();

	PrintDebug("Init audio device!");
	InitAudioDevice();

	PrintDebug("Init physics!");
	InitPhysics();

    // Textures must be loaded after window initialization (OpenGL context is required)
	PrintDebug("Load resources!");
	LoadResources();
    LoadFonts();
    
    PrintDebug("Process spritecheets!");
    gp_initResources(&res);

	PrintDebug("Setting FPS!");
	SetTargetFPS(GAME_FPS);

	// Initialize game initial variables
	game.gameScreen = MENU;
    game.screenLoaded = NONE;
    game.quit = false;
    
    // Initialize player object and its level positions
    gp_resetPlayer(&player);
    player.asset.version = 1;
    gp_initPositions(&game.levelPos);
    
    // Initialize default screen effects
    gp_initFx(&res.fx);

	PrintDebug("Game window is starting!");
    
    // Go directly to screens when developing
    if(DEBUG) {
        game.gameScreen = LEVEL_2;
        //game.gameScreen = CREDITS;
        //game.gameScreen = SELECT_PLAYER;
        //game.gameScreen = LEVEL_3;
        //player.can_move = false;
    }

	// Main game loop (Detect window close button or ESC key)
	while (!WindowShouldClose() && !game.quit) {
    
        // Toggle on/off the DEBUG functions
        TogleDebugRead();

        // Show corresponding screen (with corresponding effects)
		UpdateScreen(&player, &res.fx.fade, &res.fx.crossFade, &res.fx.bounceText);
	}

	PrintDebug("Unloading resources!");
	UnloadResources();

	PrintDebug("Closing audio device!");
	CloseAudioDevice();

	PrintDebug("Unloading physics!");
	ClosePhysics();

	//Close window and OpenGL context
	PrintDebug("Game window is closing!");
	CloseWindow();

	return 0;
}

// Loading all the ressources
void LoadResources() {

    res.backgrounds.splash.screen = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/splash.png"));
    res.backgrounds.splash.color  = BLACK;
    res.backgrounds.splash.scale  = 0.85f;
    
    res.backgrounds.menu.screen = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/menu.png"));
    res.backgrounds.menu.color  = (Color){242, 215, 255, 255};
    res.backgrounds.menu.scale  = 0.85f;
    
    res.backgrounds.controls   = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/controls.png"));
    res.backgrounds.credits    = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/credits.png"));
    res.backgrounds.minigames  = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/controls.png"));
    res.backgrounds.select     = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/select.png"));
    res.backgrounds.gameover   = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/gameover.png"));
    res.backgrounds.victory    = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/victory.png"));
    res.backgrounds.level1     = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/level1.png"));
    res.backgrounds.level2     = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/level2.png"));
    res.backgrounds.level3     = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/level3.png"));
    res.backgrounds.level4     = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/level4.png"));
    res.backgrounds.levelBonus = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/levelBonus.png"));
    
    res.sprites.player     = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "player/spritecheet_player.png"));
    res.sprites.animations = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "player/spritecheet_animation.png"));
    res.sprites.assets     = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "assets/spritecheet_assets.png"));

    res.songs.song_main = LoadMusicStream(TextFormat("%s/%s", RESOURCES_DIR, "songs/song_main.mp3"));
}

// Function loading the given font path into the resource struct
void LoadFonts() {
    
    // Font loading : Pixellari ------------------------------------------
    
    res.fonts.pixellari.baseSize = 16;
    res.fonts.pixellari.charsCount = 95;
    
    // Chars array initialization
    res.fonts.pixellari.chars = LoadFontData(
        TextFormat("%s/%s", RESOURCES_DIR, "/font/Pixellari.ttf"),
        res.fonts.pixellari.baseSize, 0,
        res.fonts.pixellari.charsCount, FONT_DEFAULT
    );
    
    // Chars texture initialization
    res.fonts.pixellari.texture = LoadTextureFromImage(
        GenImageFontAtlas(
            res.fonts.pixellari.chars, &res.fonts.pixellari.recs,
            res.fonts.pixellari.charsCount, res.fonts.pixellari.baseSize,
            0, 0
        )
    );
}

// Unloading all the ressources (will be less after putting all asset in a sprite cheet)
void UnloadResources() {

    UnloadTexture(res.backgrounds.splash.screen);
    UnloadTexture(res.backgrounds.menu.screen);

    UnloadTexture(res.backgrounds.gameover);
    UnloadTexture(res.backgrounds.victory);
    UnloadTexture(res.backgrounds.level1);
    UnloadTexture(res.backgrounds.level2);
    UnloadTexture(res.backgrounds.level3);
    UnloadTexture(res.backgrounds.level4);
    UnloadTexture(res.backgrounds.levelBonus);
    UnloadTexture(res.sprites.player);
    UnloadTexture(res.sprites.assets);

    UnloadFont(res.fonts.pixellari);

    StopMusicStream(res.songs.song_main);
}

// Function managing the screen
void UpdateScreen(Player *player, ScreenFX *fadeFx, ScreenFX *crossFadeFx, ScreenFX *bounceText) {

    // Play the song
    //PlayMusicStream(res.songs.song_main);
   
    // Draw the corresponding screen
    switch(game.gameScreen) {

        case MENU: {
            MenuDraw(crossFadeFx, bounceText);
        } break;
        
        case CONTROLS: {
            ControlsDraw(fadeFx);
        } break;
        
        case CREDITS: {
            CreditsDraw(fadeFx);
        } break;
        
        case SELECT_PLAYER: {
            SelectDraw(player, fadeFx);
        } break;
        
        case MINIGAMES: {
            MinigamesDraw(fadeFx);
        } break;

        case LEVEL_1: {
            LevelOneDraw(player, fadeFx);
        } break;

        case LEVEL_2: {
            LevelTwoDraw(player, fadeFx);
        } break;

        case LEVEL_3: {
            LevelThreeDraw(player, fadeFx);
        } break;

        case LEVEL_4: {
            LevelFourDraw(player, fadeFx);
        } break;

        case LEVEL_BONUS: {
            LevelBonusDraw(player, fadeFx);
        } break;

        case VICTORY: {
            VictoryDraw(player, fadeFx, bounceText);
        } break;

        case GAMEOVER: {
            GameoverDraw(player, fadeFx, bounceText);
        } break;

        // Default action, if screen not handled
        default : {
            MenuDraw(crossFadeFx, bounceText);
        } break;
    }  
}

// Function printing console debug string if the DEBUG switch is on
void PrintDebug(char *str) {

    if(DEBUG) { printf("[DEBUG] %s\n", str); }
}

// Function en/disabling debug mode
void TogleDebugRead() {
    
    // Just switching state
    if(IsKeyPressed(KEY_F3)) DEBUG = DEBUG ? false : true;
}

int GetDir() {
   char cwd[255];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("[DEBUG] Current working directory : %s\n", cwd);
   } else {
       perror("getcwd() error");
       return 1;
   }
   return 0;
}
