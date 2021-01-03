
#include "lib/physac.h"

#ifndef MAIN_H_

    #define MAIN_H_

// Structures and variables definition (no values, values are defined in lib/defines.c)

bool DEBUG;
char* RESOURCES_DIR;

int screenWidth;
int screenHeight;

char* screenNames[] = {
    "NONE",
    "MENU",
    "SELECT PLAYER",
    "CONTROLS",
    "CREDITS",
    "MINIGAMES",
    "VICTORY",
    "GAMEOVER",
    "SPECIAL",
    "LEVEL: 1",
    "LEVEL: 2",
    "LEVEL: 3",
    "LEVEL: 4",
    "LEVEL: 5",
    "LEVEL: BONUS"
};

typedef enum GameScreen {
    NONE,
    MENU,
    SELECT_PLAYER,
    CONTROLS,
    CREDITS,
    MINIGAMES,
    VICTORY,
    GAMEOVER,
    GS_SPECIAL,
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
    LEVEL_5,
    LEVEL_BONUS
} GameScreen;

typedef enum {RIGHT, LEFT} Direction;
typedef enum {CENTER_X, CENTER_Y, CENTER, CUSTOM} TextAlign;
typedef enum {ZERO, PLAYER, GOB, BOSS, LEAN, SKATE, HAND, FIRE} Damage;

typedef struct Notification {
	char* message;
    Color color;
    int x;
    int y;
    int size;
} Notification;

typedef struct LevelPosition {
	Vector2 level_1;
    Vector2 level_2;
    Vector2 level_3;
    Vector2 level_4;
    Vector2 level_5;
    Vector2 level_bonus;
} LevelPosition;

typedef struct Game {
	GameScreen gameScreen;
    GameScreen screenLoaded;
    GameScreen levelPassed;
    Notification notification;
    LevelPosition levelPos;
    bool quit;
} Game;

// Define the corresponding sprite cheet lines
typedef enum FramePack {
	FORWARD       = 0,
	BACKWARD      = 1,
	SUPER         = 2,
	FORWARD_IDLE  = 3,
	BACKWARD_IDLE = 4,
    PROUD         = 5,
    SPECIAL       = 6
} FramePack;

typedef enum SpritePack {
	SP_PLAYER,
    SP_ASSETS,
    SP_ANIMATION,
    SP_SCREENS
} SpritePack;

typedef struct Frame {
    FramePack pack;

    int amount; // Total frame amount for the current sprite
    int lines; // Total amount of lines for the current sprite
    int line; // Current line on total lines (offset can be set in implementation functions)
    int line_frame; // Current frame on current line (rarely used except for multiple lines sprite animation)
    int current; // Current frame on total amount
    int counter; // Framer timer to apply speed within the GAME_FPS
    int speed; // Frame speed
    
    float xinit; // Initial X position on the spritecheet
    float yinit; // Initial Y position on the spritecheet
    float x; // Current frame X position on the spritecheet
    float y; // Current frame Y position on the spritecheet
    
    bool animate; // Sprite animation toggle (if true, will show total frame amount with the given speed)
    
    bool loop; // Sprite looping toggle (if restart animation once reached the total frame amount)
    Vector2 loopPos; // Defines where the loop should restart (which line (y) and frame (x), not coordinates)
} Frame;

// Define minimal asset structure (works for pretty much every game asset, items, screens)
typedef struct Asset {
    SpritePack sprite;
    Vector2 position;
    Frame frame;
    Direction direction;
    Damage type;
    Color color;
    
    float scale;
    float width;
    float height;
    float swidth; // Scaled width
    float sheight; // Scaled height
    float speed;
    float density;
    int rotation;
    int version; // if several color/version of a same asset (Last is version_count - 1 array-like))
    int version_count;
    bool disabled;
} Asset;

typedef struct Enemy {
    Asset asset;
    
    int lives;
    int amount;
} Enemy;

typedef struct Player {
    Asset asset;
    PhysicsBody body;

    int lives;
    int lean;
    bool dead;
    bool super;
    bool can_move;
    
    bool gelano;
    bool jetLean;
    bool slip;
    bool portalPowerUsed;
} Player;

typedef struct ResImage {
    Texture2D screen;
    Color color;
    float scale;
    bool custom_scale;
} ResImage;

// Main structure with nested struct, to access game properties anywhere, at anytime later
typedef struct ResourcesBackgrounds {
    ResImage splash;
    ResImage menu;
    ResImage controls;
    ResImage credits;
    ResImage select;
    ResImage minigames;
    ResImage gameover;
    ResImage victory;
    ResImage special;
    ResImage level1;
    ResImage level2;
    ResImage level3;
    ResImage level4;
    ResImage level5;
    ResImage level_bonus;
} ResourcesBackgrounds;

/*
typedef struct ResourcesBackgrounds {
    Asset splash;
    Asset menu;
    Asset controls;
    Asset credits;
    Asset select;
    Asset minigames;
    Asset gameover;
    Asset victory;
    Asset special;
    Asset level1;
    Asset level2;
    Asset level3;
    Asset level4;
    Asset level5;
    Asset level_bonus;
} ResourcesBackgrounds;
*/

typedef struct ResourcesSprites {
    Texture2D player;
    Texture2D assets;
    Texture2D animations;
    Texture2D screens;
} ResourcesSprites;

typedef struct ResourcesItems {
    Asset player_head;
    Asset lean;
    Asset spikes;
    Asset launchpad;
    Asset portal;
    Asset mouse;
    Asset gob_walking;
    Asset gob_dying;
    Asset dark_sonic;
    Asset damage;
    Asset fireball;
    Asset goblean_skate;
    Asset goblean_flip;
    Asset skate_flip;
    Asset left_hand;
    Asset right_hand;
    Asset fire_columns;
    
    Asset kb_arrows;
    Asset kb_spacebar;
    
    Asset lifebar;
    
    Asset player_idle;
    Asset player_anim_flip;
    Asset player_anim_sleep;
    Asset player_anim_clap;
    Asset player_anim_transform;
    
    Asset gelano;
    Asset barrel_full;
    Asset barrel_broken;
} ResourcesItems;

typedef struct ResourcesMusics {
    Music song_main;
} ResourcesMusics;

typedef struct ResourcesFonts {
    Font pixellari;
    Font raylib;
} ResourcesFonts;

typedef enum {CUBIC, SINE, EXPO} EffectMethod;

typedef struct ScreenFX {
    int timer;
    int duration;
    int timeout;
    int rotation;
    float alpha;
    float scale;
    float scaleBase;
    float scaleFinal;
    float increment;
    bool invert;
    bool enabled;
    
    Color color;
    EffectMethod effect;
} ScreenFX;

typedef struct ResourcesFX {
    ScreenFX fade;
    ScreenFX crossFade;
    ScreenFX bounceText;
} ResourcesFX;

typedef struct ResourcesSounds {
    Sound explosion;
    Sound goblean;
} ResourcesSounds;

typedef struct Resources {
    ResourcesBackgrounds backgrounds;
    ResourcesSprites     sprites;
    ResourcesItems       items;
    ResourcesMusics      songs;
    ResourcesFonts       fonts;
    ResourcesFX          fx;
    ResourcesSounds      sounds;
} Resources;

// Initialize the two main global game variables (globally available)
Game game;
Resources res;

#endif
