// Structures and variables definition (no values, values are defined in lib/defines.c)

bool DEBUG;

int screenWidth;
int screenHeight;

typedef enum GameScreen {
	MENU,
	LEVEL_1,
	LEVEL_2,
  LEVEL_3,
	SUCCESS,
	GAME_OVER
} GameScreen;

typedef struct Game {
	GameScreen gameScreen;
} Game;

Game game;

typedef struct Player {
	Vector2 position;
	int health_point;
	int nb_lean;
} Player;

// Load them at game start in a similar struct
// Main object with sub-object for level/screen, to access them anywhere, at anytime later
typedef struct ResourcesScreens {
  Texture2D background;
} ResourcesScreens;

typedef struct ResourcesPlayer {
  Texture2D forward;
  Texture2D backward;
  Texture2D dead;
  Texture2D head;
  Texture2D imgPlayer;
} ResourcesPlayer;

typedef struct ResourcesItems {
  Texture2D lean;
} ResourcesItems;

typedef struct Resources {
  ResourcesScreens menu;
  ResourcesScreens gameover;
  ResourcesScreens sucess;
  ResourcesScreens level1;
  ResourcesScreens level2;
  ResourcesScreens level3;
  ResourcesPlayer player;
} Resources;

Texture2D backgroundMenu;
Texture2D backgroundGameover;
Texture2D backgroundSuccess;
Texture2D background_lvl1;
Texture2D background_lvl2;
Texture2D background_lvl3;
Texture2D solinReverse;
Texture2D solinPlayer;
Texture2D solinDead;
Texture2D solinBall;

Texture2D gobleanDead, gobleanAlive, goblean_ennemy;
Texture2D mainG_empty, mainD_empty, mainG, mainD, goblean_mainG, goblean_mainD;
Texture2D car_destroyed, car_new, car_ennemy;
Texture2D lean_launched, lean_destroyed, lean;

Texture2D solin_head;
Texture2D imgPlayer;
Texture2D lean1_available;
Texture2D lean2_available;
Texture2D lean3_available;
Texture2D lean1_taken;
Texture2D lean2_taken;
Texture2D lean3_taken;
Texture2D tremplin;
Texture2D teleportation_OK;
Texture2D teleportation_NOK;
Texture2D spikes;
Texture2D inventory_lean;
Texture2D lean1;
Texture2D lean2;
Texture2D lean3;
Texture2D teleportation;


Music soincSong;

