#include "../../lib/raylib/raylib.h"
#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#include "../../lib/raylib/physac.h"
#include "../../lib/raylib/physac.h"
#include "variables.h"
#include "MyPhysic.h"


int main(void)
{

    Image image = LoadImage("../../res/solin.png");
    Image image3 = LoadImage("../../res/solin_reverse.png");
    Image back = LoadImage("../../res/Level1/ECRAN1V.png");
    Image solin_head = LoadImage("../../res/solin_head.png");


    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight, "s/o'lin à la recherche du lean");

    
    MyPhysic Maphysique;

    // Disable dynamics to floor and walls physics bodies
    MaPhysique.floorLeft->enabled = false;
    MaPhysique.floorRight->enabled=false;
    MaPhysique.platform->enabled = false;
    MaPhysique.wallLeft->enabled = false;
    MaPhysique.wallRight->enabled = false;

    
    MaPhysique.body->freezeOrient = true;      // Constrain MaPhysique.body rotation to avoid little collision torque amounts
    


    Texture2D solin = LoadTextureFromImage(image);
    Texture2D background = LoadTextureFromImage(back);
    Texture2D lives = LoadTextureFromImage(solin_head);
    
    Rectangle trou = { 415, 700, 245, 10};
    Rectangle wall_right = { 800, 200, 10, 200};

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        RunPhysicsStep();
        
        Rectangle rect_solin = { MaPhysique.body -> position.x - 30, MaPhysique.body -> position.y - 30, 60, 60 };
        
        // Horizontal movement input
        if (IsKeyDown(KEY_RIGHT)) {
        MaPhysique.body->velocity.x = vitesse;
        if (boule && !right){
        solin = LoadTextureFromImage(image);
        }
        right = true;
        }        
        else if (IsKeyDown(KEY_LEFT)) {
        MaPhysique.body->velocity.x = -vitesse;
        if (boule && right){
        solin = LoadTextureFromImage(image3);
        }
        right = false;
        }
        
        col_trou = CheckCollisionRecs(rect_solin, trou);
	
	if(col_trou){
	MaPhysique.body->position.x = 80;
	MaPhysique.body->position.y = screenHeight/2;
	fall++;
	nb_lives--;
	}
	
	col_wall_right = CheckCollisionRecs(rect_solin, wall_right);
        
        if(col_wall_right) victory = true;
        

        // Vertical movement input checking if player physics body is grounded
        if (IsKeyDown(KEY_UP) && MaPhysique.bod->isGrounded) MaPhysique.bod->velocity.y = -VELOCITY*3;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawFPS(screenWidth - 90, screenHeight - 30);
            
            DrawTextureEx(background, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
            
            DrawTextureEx(solin, (Vector2){ MaPhysique.bod -> position.x - 40, MaPhysique.body -> position.y - 30}, 0.0f, 0.15f, WHITE);
            
            DrawTextureEx(lives, (Vector2){10, 20}, 0.0f, 0.25f, WHITE);
            DrawText(TextFormat("%d", nb_lives), 90, 25, 30, WHITE);
            
            if(fall>1) DrawText("Utilisez la plateforme pour arriver de l'autre côté", 150, 100, 20, RED);
            
         
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    ClosePhysics();       // Unitialize physics

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}



