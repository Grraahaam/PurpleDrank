#include "../../lib/raylib/raylib.h"

#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#include "../../lib/raylib/physac.h"

#define VELOCITY 0.5f

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    Image image = LoadImage("../../res/soinc.png");
    Image image3 = LoadImage("../../res/soinc_reverse.png");
    Image back = LoadImage("../../res/Level1/ECRAN1V.png");
    Image soinc_head = LoadImage("../../res/soinc_head.png");
    float vitesse = VELOCITY*0.4;
    bool boule = true;
    bool right = true;
    bool col_trou = false;
    int fall = 0;
    int nb_lives = 5;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "soinc à la recherche du lean");


    // Initialize physics and default physics bodies
    InitPhysics();

    // Create floor and walls rectangle physics body
    PhysicsBody floorLeft = CreatePhysicsBodyRectangle((Vector2){ 190, 350 }, 445, 170, 10);
    PhysicsBody platform = CreatePhysicsBodyRectangle((Vector2){ 535, 340 }, 100, 70, 10);
    PhysicsBody floorRight = CreatePhysicsBodyRectangle((Vector2){ 740, 360 }, 150, 150, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ -5, screenHeight/2 }, 10, screenHeight, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth + 5, screenHeight/2 }, 10, screenHeight, 10);

    // Disable dynamics to floor and walls physics bodies
    floorLeft->enabled = false;
    floorRight->enabled=false;
    platform->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;

    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){ 80, screenHeight/2 }, 50, 60, 1);
    body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
    
    Texture2D soinc = LoadTextureFromImage(image);
    Texture2D background = LoadTextureFromImage(back);
    Texture2D lives = LoadTextureFromImage(soinc_head);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        RunPhysicsStep();
        
        Rectangle rect_soinc = { body -> position.x - 30, body -> position.y - 30, 60, 60 };
        Rectangle trou = { 415, 700, 245, 10};
        
        // Horizontal movement input
        if (IsKeyDown(KEY_RIGHT)) {
        body->velocity.x = vitesse;
        if (boule && !right){
        soinc = LoadTextureFromImage(image);
        }
        right = true;
        }        
        else if (IsKeyDown(KEY_LEFT)) {
        body->velocity.x = -vitesse;
        if (boule && right){
        soinc = LoadTextureFromImage(image3);
        }
        right = false;
        }
        
        col_trou = CheckCollisionRecs(rect_soinc, trou);
	
	if(col_trou){
	body->position.x = 80;
	body->position.y = screenHeight/2;
	fall++;
	nb_lives--;
	}
        

        // Vertical movement input checking if player physics body is grounded
        if (IsKeyDown(KEY_UP) && body->isGrounded) body->velocity.y = -VELOCITY*3;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawFPS(screenWidth - 90, screenHeight - 30);
            
            DrawTextureEx(background, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
            
            DrawTextureEx(soinc, (Vector2){ body -> position.x - 40, body -> position.y - 30}, 0.0f, 0.15f, WHITE);
            
            DrawTextureEx(lives, (Vector2){680, 20}, 0.0f, 0.25f, WHITE);
            DrawText(TextFormat("%d", nb_lives), 750, 25, 30, WHITE);
            
            if(fall>1) DrawText("Utilisez la plateforme pour arriver de l'autre côté", 150, 100, 20, RED);
            
            //DrawRectangleRec(rect_soinc, WHITE);
            //DrawRectangleRec(trou, GOLD);

            //Draw created physics bodies
            int bodiesCount = GetPhysicsBodiesCount();
            for (int i = 0; i < bodiesCount; i++)
            {
                PhysicsBody body = GetPhysicsBody(i);

                int vertexCount = GetPhysicsShapeVerticesCount(i);
                for (int j = 0; j < vertexCount; j++)
                {
                    // Get physics bodies shape vertices to draw lines
                    // Note: GetPhysicsShapeVertex() already calculates rotation transformations
                    Vector2 vertexA = GetPhysicsShapeVertex(body, j);

                    int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
                    Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

                    DrawLineV(vertexA, vertexB, BLUE);     // Draw a line between two vertex positions
                }
            }
            

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
