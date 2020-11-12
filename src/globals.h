// Structures and variables definition (no values, values are defined in lib/defines.c)

typedef enum GameScreen {
	MENU,
	LEVEL_1
} GameScreen;

typedef struct Game {
	GameScreen gameScreen;
} Game;

Game game;

typedef struct Player {
	Vector2 position; 
	float speed;
	bool canJump;
	bool sol;
	int health_point;
} Player;




// Load them at game start in a similar struct
// Main object with sub-object for level/screen, to access them anywhere, at anytime later
typedef struct Decor {
	//PhysicsBody floorLeft, platform, floorRight, wallLeft, wallRight;	
} Decor;

int screenWidth;
int screenHeight;

Texture2D backgroundMenu;
Texture2D background_lvl1;
Texture2D background_lvl2;
Texture2D background_lvl3;
Texture2D soincReverse;
Texture2D soincPlayer;
Texture2D solin_head;
Texture2D imgPlayer;
Music soincSong;
Image img_soincPlayer;
Image img_Reverse;
Image img_solin_head;

