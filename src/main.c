
// This is the main program file, that is compiled to get the stable executable

#include <stdio.h>
#include <unistd.h>
#include <raylib.h>

#include "lib/raymath.h"
#include "lib/physac.h"
#include "lib/defines.c"

// Defines the global variables, structs and enums
#include "globals.h"
#include "gameplay.h"
#include "screens/menu.h"
#include "screens/controls.h"
#include "screens/credits.h"
#include "screens/select.h"
#include "screens/minigames.h"
#include "screens/victory.h"
#include "screens/gameover.h"
#include "screens/level1.h"
#include "screens/level2.h"
#include "screens/level3.h"
#include "screens/level4.h"
#include "screens/level5.h"
#include "screens/levelBonus.h"

// Function declarations
void UpdateScreen(Player *player, ScreenFX *fadeFx, ScreenFX *crossFadeFx, ScreenFX *bounceText);
void LoadResources(void);
void LoadFonts();
void UnloadResources(void);
void ToggleDebugRead();
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
        
    // Defining screen size variables (not fullscreen)
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
    
        // Static sized window
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    
        // Dynamic sized window (still issues when tested on different screen dimensions)
        //InitWindow(GetScreenWidth(), GetScreenHeight(), GAME_NAME);
    
    HideCursor();
    
    // Set the game window fullscreen by default
    //ToggleFullscreen();

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
    gp_initPlayer(&player);
    gp_initPositions(&game.levelPos);
    player.asset.version = 1;
    
    // Initialize default screen effects
    gp_initFx(&res.fx);

    PrintDebug("Game window is starting!");
    
    // Go directly to screens when developing
    if(DEBUG) {
        //game.gameScreen = MENU;
        //game.gameScreen = SELECT_PLAYER;
        //game.gameScreen = CREDITS;
        game.gameScreen = CONTROLS;
        //game.gameScreen = VICTORY;
        //game.gameScreen = GAMEOVER;
        //game.gameScreen = LEVEL_1;
        //game.gameScreen = LEVEL_2;
        //game.gameScreen = LEVEL_3;
        //game.gameScreen = LEVEL_4;
        //game.gameScreen = LEVEL_5;
        //game.gameScreen = LEVEL_BONUS;
        //player.lives = 50;
        //player.lean = 50;
    }

    // Main game loop (Detect window close button or ESC key)
    while (!WindowShouldClose() && !game.quit) {
    
        // Toggle on/off the DEBUG functions
        ToggleDebugRead();
        
        // Toggle fulscreen mode on/off (NOT WORKING YET)
        /*if(IsKeyPressed(KEY_ESCAPE) && IsWindowFullscreen()) ToggleFullscreen();
        if(IsKeyPressed(KEY_F) && !IsWindowFullscreen()) ToggleFullscreen();*/

        // Returning to the menu and re-initializing player object
        if(IsKeyPressed(KEY_F1)) game.gameScreen = MENU;
        
        // Happy X-Mas!
        if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyDown(KEY_L) && IsKeyDown(KEY_E) && (IsKeyDown(KEY_A) || IsKeyDown(KEY_Q)) && IsKeyDown(KEY_N)) {
            
            if(player.lean <= 0) player.lean += 20;
        }
        
        // And happy new year!
        else if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyDown(KEY_E) && IsKeyDown(KEY_K) && IsKeyDown(KEY_I) && IsKeyDown(KEY_P)) {
            
            if(player.lives <= 1) player.lives += 5;
        }
        
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
    res.backgrounds.splash.custom_scale  = true;
    res.backgrounds.splash.scale  = gp_autoScaleImg(&res.backgrounds.splash);
    
    res.backgrounds.menu.screen = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/menu.png"));
    res.backgrounds.menu.color  = (Color){242, 215, 255, 255};
    res.backgrounds.menu.scale  = gp_autoScaleImg(&res.backgrounds.menu);
    
    res.backgrounds.controls.screen    = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/controls.png"));
    res.backgrounds.credits.screen     = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/credits.png"));
    res.backgrounds.minigames.screen   = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/minigames.png"));
    res.backgrounds.select.screen      = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/select.png"));
    res.backgrounds.gameover.screen    = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/gameover.png"));
    res.backgrounds.victory.screen     = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/victory.png"));
    res.backgrounds.level1.screen      = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/level1.png"));
    res.backgrounds.level2.screen      = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/level2.png"));
    res.backgrounds.level3.screen      = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/level3.png"));
    res.backgrounds.level4.screen      = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/level4.png"));
    res.backgrounds.level5.screen      = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/level5.png"));
    res.backgrounds.level_bonus.screen = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/level_bonus.png"));
    
    res.sprites.player     = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "player/spritecheet_player.png"));
    res.sprites.animations = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "player/spritecheet_animation.png"));
    res.sprites.assets     = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "assets/spritecheet_assets.png"));
    //res.sprites.screens     = LoadTexture(TextFormat("%s/%s", RESOURCES_DIR, "backgrounds/spritecheet_screens.png"));

    res.songs.song_liam      = LoadMusicStream(TextFormat("%s/%s", RESOURCES_DIR, "songs/song_liam.mp3"));
    res.songs.song_character = LoadMusicStream(TextFormat("%s/%s", RESOURCES_DIR, "songs/character_select.mp3"));
    res.songs.song_credits   = LoadMusicStream(TextFormat("%s/%s", RESOURCES_DIR, "songs/credits.mp3"));
    res.songs.song_controls  = LoadMusicStream(TextFormat("%s/%s", RESOURCES_DIR, "songs/controls.mp3"));
    
    res.sounds.explosion    = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/explosion.mp3"));
    res.sounds.goblean      = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/goblean.mp3"));
    res.sounds.level_change = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/changement_lvl.mp3"));
    res.sounds.fall         = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/chute.mp3"));
    res.sounds.launch_game  = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/lancement_jeu.mp3"));
    res.sounds.monster      = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/monstre.mp3"));
    res.sounds.jump         = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/jump.mp3"));
    res.sounds.lean         = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/blaster.mp3"));
    res.sounds.drink        = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/drinking.mp3"));
    res.sounds.launchpad    = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/spring.mp3"));
    res.sounds.fire         = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/fire.mp3"));
    res.sounds.portal       = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/portal.mp3"));
    res.sounds.gameover     = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/gameover.mp3"));
    res.sounds.victory      = LoadSound(TextFormat("%s/%s", RESOURCES_DIR, "audio/victory.mp3"));
    
    SetSoundVolume(res.sounds.explosion, 5.0f);
    SetSoundVolume(res.sounds.jump, 2.0f);
    SetSoundVolume(res.sounds.launch_game, 0.8f);
    SetSoundVolume(res.sounds.drink, 5.0f);
    SetMusicVolume(res.songs.song_liam, .2f);
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
    UnloadTexture(res.backgrounds.gameover.screen);
    UnloadTexture(res.backgrounds.victory.screen);
    UnloadTexture(res.backgrounds.level1.screen);
    UnloadTexture(res.backgrounds.level2.screen);
    UnloadTexture(res.backgrounds.level3.screen);
    UnloadTexture(res.backgrounds.level4.screen);
    UnloadTexture(res.backgrounds.level5.screen);
    UnloadTexture(res.backgrounds.level_bonus.screen);
    
    UnloadTexture(res.sprites.player);
    UnloadTexture(res.sprites.animations);
    UnloadTexture(res.sprites.assets);
    UnloadTexture(res.sprites.screens);

    UnloadFont(res.fonts.pixellari);
    
    StopMusicStream(res.songs.song_character);
    StopMusicStream(res.songs.song_liam);
    StopMusicStream(res.songs.song_credits);
    StopMusicStream(res.songs.song_controls);
    
    UnloadSound(res.sounds.explosion);
    UnloadSound(res.sounds.goblean);
    UnloadSound(res.sounds.level_change);
    UnloadSound(res.sounds.fall);
    UnloadSound(res.sounds.launch_game);
    UnloadSound(res.sounds.monster);
    UnloadSound(res.sounds.jump);
    UnloadSound(res.sounds.lean);
    UnloadSound(res.sounds.drink);
    UnloadSound(res.sounds.fire);
    UnloadSound(res.sounds.launchpad);
    UnloadSound(res.sounds.portal);
    UnloadSound(res.sounds.gameover);
    UnloadSound(res.sounds.victory);
}

// Function managing the screen
void UpdateScreen(Player *player, ScreenFX *fadeFx, ScreenFX *crossFadeFx, ScreenFX *bounceText) {

    // Play the song
    //PlayMusicStream(res.songs.song_main);
   
    // Draw the corresponding screen
    switch(game.gameScreen) {

        case MENU: {
            MenuDraw(player, crossFadeFx, bounceText);
        } break;
        
        case CONTROLS: {
            ControlsDraw(fadeFx);
            if(!IsMusicPlaying(res.songs.song_controls)){
        	PlayMusicStream(res.songs.song_controls);
            }
            UpdateMusicStream(res.songs.song_controls);
        } break;
        
        case CREDITS: {
            CreditsDraw(fadeFx);
            if(!IsMusicPlaying(res.songs.song_credits)){
        	PlayMusicStream(res.songs.song_credits);
            }
            UpdateMusicStream(res.songs.song_credits);
        } break;
        
        case SELECT_PLAYER: {
            SelectDraw(player, fadeFx);
            if(!IsMusicPlaying(res.songs.song_character)){
        	PlayMusicStream(res.songs.song_character);
            }
            UpdateMusicStream(res.songs.song_character);
        } break;
        
        case MINIGAMES: {
            MinigamesDraw(fadeFx);
        } break;

        case LEVEL_1: {
            LevelOneDraw(player, fadeFx);
            if(!IsMusicPlaying(res.songs.song_liam)){
        	PlayMusicStream(res.songs.song_liam);
            }
            UpdateMusicStream(res.songs.song_liam);
        } break;

        case LEVEL_2: {
            LevelTwoDraw(player, fadeFx);
            UpdateMusicStream(res.songs.song_liam);
        } break;

        case LEVEL_3: {
            LevelThreeDraw(player, fadeFx);
            UpdateMusicStream(res.songs.song_liam);
        } break;

        case LEVEL_4: {
            LevelFourDraw(player, fadeFx);
            UpdateMusicStream(res.songs.song_liam);
        } break;

        case LEVEL_BONUS: {
            LevelBonusDraw(player, fadeFx);
            UpdateMusicStream(res.songs.song_liam);
        } break;
        
        case LEVEL_5: {
            LevelFiveDraw(player, fadeFx);
            UpdateMusicStream(res.songs.song_liam);
        } break;

        case VICTORY: {
            VictoryDraw(player, fadeFx, bounceText);
        } break;

        case GAMEOVER: {
            GameoverDraw(player, fadeFx, bounceText);
        } break;

        // Default action, if screen not handled
        default : {
            MenuDraw(player, crossFadeFx, bounceText);
        } break;
    }  
}

// Function printing console debug string if the DEBUG switch is on
void PrintDebug(char *str) {

    if(DEBUG) { printf("[DEBUG] %s\n", str); }
}

// Function en/disabling debug mode
void ToggleDebugRead() {
    
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
