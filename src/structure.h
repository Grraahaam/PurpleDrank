//#include "../lib/raylib/physac.h"

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


typedef struct Decor {		// Load them at game start in a similar struct (per level) to access them anywhere, at anytime later
	PhysicsBody floorLeft, platform, floorRight, wallLeft, wallRight;	
	
} Decor;



