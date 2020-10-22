/*******************************************************************************************
*
*   Physac - Physics movement
*
*   NOTE 1: Physac requires multi-threading, when InitPhysics() a second thread is created to manage physics calculations.
*   NOTE 2: Physac requires static C library linkage to avoid dependency on MinGW DLL (-static -lpthread)
*
*   Use the following line to compile:
*
*   gcc -o $(NAME_PART).exe $(FILE_NAME) -s -static  /
*       -lraylib -lpthread -lglfw3 -lopengl32 -lgdi32 -lopenal32 -lwinmm /
*       -std=c99 -Wl,--subsystem,windows -Wl,-allow-multiple-definition
*
*   Copyright (c) 2016-2018 Victor Fisac
*
********************************************************************************************/

#include "../lib/raylib/raylib.h"

#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#include "../lib/raylib/physac.h"

#define VELOCITY 0.5f

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    Image image = LoadImage("../res/soinc.png");
    Image image2 = LoadImage("../res/boule.png");
    Image image3 = LoadImage("../res/soinc_reverse.png");
    Image platform = LoadImage("../res/lvl1/platform.png");
    Image car = LoadImage("../res/lvl1/car.png");
    Image gob_lean = LoadImage("../res/lean.png");
    Image empty = LoadImage("../res/lvl1/empty.png");
    float vitesse = VELOCITY*0.4;
    float carX = 900;
    float carY = 310;
    float leanX;
    float leanY;
    bool boule = true;
    bool collision = false;
    bool col_lean1 = false, col_lean2 = false, col_lean3 = false;
    bool l1 = false, l2 = false, l3 = false;
    bool right = true;
    int nb_lean = 0;
    bool lancer = false;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Physac [raylib] - Physics movement");

    // Physac logo drawing position
    int logoX = screenWidth - MeasureText("Physac", 30) - 10;
    int logoY = 15;

    // Initialize physics and default physics bodies
    InitPhysics();

    // Create floor and walls rectangle physics body
    PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2){ screenWidth/2, screenHeight }, screenWidth, 100, 10);
    PhysicsBody platformLeft = CreatePhysicsBodyRectangle((Vector2){ 170, 270 }, 100, 40, 10);
    PhysicsBody platformRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth*0.75f, screenHeight*0.6f }, screenWidth*0.25f, 10, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ -5, screenHeight/2 }, 10, screenHeight, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth + 5, screenHeight/2 }, 10, screenHeight, 10);

    // Disable dynamics to floor and walls physics bodies
    floor->enabled = false;
    platformLeft->enabled = false;
    platformRight->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;

    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){ screenWidth/2, screenHeight/2 }, 50, 70, 1);
    body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
    
    Texture2D soinc = LoadTextureFromImage(image);
    Texture2D leftPlat = LoadTexture("../res/lvl1/platform.png");
    Texture2D car_ennemy = LoadTextureFromImage(car);
    Texture2D lean = LoadTextureFromImage(gob_lean);
    Texture2D lean1 = LoadTextureFromImage(gob_lean);
    Texture2D lean2 = LoadTextureFromImage(gob_lean);
    Texture2D lean3 = LoadTextureFromImage(gob_lean);
    
    Rectangle rect_platLeft = { 120, 250, 100, 40 };
    Rectangle rect_lean1 = { 600, 220, 25, 40 };
    Rectangle rect_lean2 = { 500, 360, 25, 40 };
    Rectangle rect_lean3 = { 150, 200, 25, 40 };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        RunPhysicsStep();
        
        Rectangle rect_soinc = { body -> position.x - 35, body -> position.y - 45, 70, 80 };
        
        carX -= 0.3f;
        if(lancer){
        leanX += 3.0f;
        }

        if (IsKeyPressed('R'))    // Reset physics input
        {
            // Reset movement physics body position, velocity and rotation
            body->position = (Vector2){ screenWidth/2, screenHeight/2 };
            body->velocity = (Vector2){ 0, 0 };
            SetPhysicsBodyRotation(body, 0);
        }


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
        
        else if (IsKeyDown(KEY_DOWN) && boule) {
        soinc = LoadTextureFromImage(image2);
        vitesse = vitesse*3;
        boule = false;
        }
        
        else if (IsKeyDown(KEY_DOWN) && !boule && vitesse > 0.5 ) {
        soinc = LoadTextureFromImage(image);
        vitesse = vitesse/3;
        boule = true;
        }

        // Vertical movement input checking if player physics body is grounded
        if (IsKeyDown(KEY_UP) && body->isGrounded) body->velocity.y = -VELOCITY*4;
        //----------------------------------------------------------------------------------

	collision = CheckCollisionRecs(rect_soinc, rect_platLeft);
	col_lean1 = CheckCollisionRecs(rect_soinc, rect_lean1);
	col_lean2 = CheckCollisionRecs(rect_soinc, rect_lean2);
	col_lean3 = CheckCollisionRecs(rect_soinc, rect_lean3);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawFPS(screenWidth - 90, screenHeight - 30);
            
            DrawTextureEx(soinc, (Vector2){ body -> position.x - 50, body -> position.y - 50}, 0.0f, 0.2f, WHITE);
            
            DrawTextureEx(leftPlat, (Vector2){ 90, 220}, 0.0f, 0.08f, WHITE);
            
            DrawTextureEx(car_ennemy, (Vector2){ carX, carY }, 0.0f, 0.4f, WHITE );
            
            if(collision) DrawText("COLLISION", screenWidth/2, screenHeight/2, 20, WHITE);
            
            if(col_lean1 && !l1){ 
            nb_lean ++;
            lean1 = LoadTextureFromImage(empty);
            l1 = true;
            }
            
            if(col_lean2 && !l2){
            nb_lean ++;
            lean2 = LoadTextureFromImage(empty);
            l2 = true;
            }
            
            if(col_lean3 && !l3){
            nb_lean ++;
            lean3 = LoadTextureFromImage(empty);
            l3 = true;
            }
            
            if (IsKeyDown(KEY_SPACE) && nb_lean > 0){
            leanX = body -> position.x;
            leanY = body -> position.y;
            nb_lean = nb_lean - 1;
            lancer = true;

            }
            
            if(lancer){
            DrawTextureEx(lean, (Vector2){leanX, leanY - 20}, 0.0f, 0.08f, WHITE);
            }
            
            if(leanX == carX) lancer = false;
            
            //DrawRectangleRec(rect_soinc, WHITE);
            //DrawRectangleRec(rect_platLeft, GOLD);

            // Draw created physics bodies
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
            
            DrawTextureEx(lean, (Vector2){10, 10}, 0.0f, 0.08f, WHITE);
            DrawTextureEx(lean1, (Vector2){600, 220}, 0.0f, 0.08f, WHITE);
            DrawTextureEx(lean2, (Vector2){500, 360}, 0.0f, 0.08f, WHITE);
            DrawTextureEx(lean3, (Vector2){150, 200}, 0.0f, 0.08f, WHITE);

            DrawText(TextFormat("%d", nb_lean), 60, 25, 30, BLUE);

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
