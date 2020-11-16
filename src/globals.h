// Structures and variables definition (no values, values are defined in lib/defines.c)

bool DEBUG;

typedef enum GameScreen {
	MENU,
	LEVEL_1,
	LEVEL_2,
	SUCCESS,
	GAME_OVER
} GameScreen;

typedef struct Game {
	GameScreen gameScreen;
} Game;

Game game;

typedef struct Player {
	Vector2 position; 
	//float speed;
	//bool canJump;
	//bool sol;
	int health_point;
	int nb_lean;
} Player;

// Load them at game start in a similar struct
// Main object with sub-object for level/screen, to access them anywhere, at anytime later
typedef struct Decor {
	//PhysicsBody floorLeft, platform, floorRight, wallLeft, wallRight;	
} Decor;

int screenWidth;
int screenHeight;

Texture2D backgroundMenu;
Texture2D backgroundGameover;
Texture2D backgroundSuccess;
Texture2D background_lvl1;
Texture2D background_lvl2;
Texture2D background_lvl3;
Texture2D solinReverse;
Texture2D solinPlayer;
Texture2D solinDead;
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

