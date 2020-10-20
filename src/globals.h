
typedef enum GameScreen {
	MENU,
	LEVEL_1
} GameScreen;

typedef struct Game {
	GameScreen gameScreen;
} Game;

typedef struct Player {
	Vector2 position;
	//player;
	float speed;
	bool canJump;
	bool sol;
} Player;

typedef struct Decor {
	float scrollingBack;
	float scrollingMid;
	float scrollingFore;
	// Load them at game start in a similar struct (per level) to access them anywhere, at anytime later
	/*background;
	midground;
	foreground;*/
} Decor;

Game game;

Texture2D background;
Texture2D background_lvl1;
Texture2D foreground_lvl1;
Texture2D midground_lvl1;
Texture2D soincPlayer;

Music soincSong;
