player1/*******************************************************************************************
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

#define VELOCITY    0.5f

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1300;
    const int screenHeight = 450;
    float rotate = 0.0f;
    int doubleJump = 0;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Physac [raylib] - Physics movement");

    // Physac logo drawing position
    int logoX = screenWidth - MeasureText("Physac", 30) - 10;
    int logoY = 15;

    // Initialize physics and default physics bodies
    InitPhysics();

    // Create floor and walls rectangle physics body
    PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2){ screenWidth/2, screenHeight }, screenWidth, 100, 10);
    // PhysicsBody platformLeft = CreatePhysicsBodyRectangle((Vector2){ screenWidth*0.15f, screenHeight*0.6f }, screenWidth*0.55f, 20, 10);
    // PhysicsBody platformRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth*0.75f, screenHeight*0.6f }, screenWidth*0.25f, 10, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ -5, screenHeight/2 }, 10, screenHeight, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth + 5, screenHeight/2 }, 10, screenHeight, 10);
    PhysicsBody goalRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth - 5, screenHeight - 150 }, 10, 200, 10);
    PhysicsBody goalLeft = CreatePhysicsBodyRectangle((Vector2){ +5, screenHeight - 150 }, 10, 200, 10);

    // Disable dynamics to floor and walls physics bodies
    floor->enabled = false;
    // platformLeft->enabled = false;
    // platformRight->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;
    goalLeft->enabled = false;
    goalRight->enabled = false;

    // // Create movement physics body
    // PhysicsBody body = CreatePhysicsBodyRE((Vector2){ screenWidth/2, screenHeight/2 }, 39, 1);
    // body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
    // body->orient = rotate;
    // PhysicsBody player2 = CreatePhysicsBodyCircle((Vector2){ screenWidth/2, screenHeight/2 }, 39, 1);
    // player2->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
    // player2->orient = rotate;
    PhysicsBody player1= CreatePhysicsBodyRectangle((Vector2){ 150, 2*screenHeight/3 }, 50, 70, 1);
   player1->freezeOrient = true;
    PhysicsBody player2 = CreatePhysicsBodyRectangle((Vector2){ screenWidth -150, screenHeight/2 }, 50, 70, 1);
    player2->freezeOrient = true;

    PhysicsBody multipla_body = CreatePhysicsBodyCircle((Vector2){ screenWidth/3, screenHeight/2 }, 70, 1);
    multipla_body->freezeOrient = false ;      // Constrain body rotation to avoid little collision torque amounts
    multipla_body->dynamicFriction = 0.05;
    multipla_body->staticFriction = 0.05;
    multipla_body->mass = 0.55 ;

    void reset(){
        // Reset movement physics body position, velocity and rotation
       player1->position = (Vector2){ screenWidth/2, screenHeight/2 };
       player1->velocity = (Vector2){ 0, 0 };
        SetPhysicsBodyRotation(player1, 0);
    }



    Texture2D soinc = LoadTexture("../res/soinc.png");
    Texture2D uk = LoadTexture("../res/ugandan_knuckles.png");


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        RunPhysicsStep();

        if (IsKeyPressed('R')) reset(); // Reset physics input



        //Player1 INPUT
        if (IsKeyDown(KEY_RIGHT))player1->velocity.x = VELOCITY;
        else if (IsKeyDown(KEY_LEFT))player1->velocity.x = -VELOCITY;
        if (IsKeyDown(KEY_UP) &&player1->isGrounded)player1->velocity.y = -VELOCITY*4;

        //Player2 INPUT
        if (IsKeyDown(KEY_D)) player2->velocity.x = VELOCITY;
        else if (IsKeyDown(KEY_A)) player2->velocity.x = -VELOCITY;
        if (IsKeyDown(KEY_W) && player2->isGrounded) player2->velocity.y = -VELOCITY*4;
        // if (IsKeyPressed(KEY_UP) && doubleJump<2){player1->velocity.y = -VELOCITY*4; printf("u");doubleJump++ ;};
        // if (body->isGrounded) doubleJump = 0;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawFPS(screenWidth - 90, screenHeight - 30);

            DrawTextureEx(soinc, (Vector2){player1 -> position.x - 50,player1 -> position.y - 50}, rotate, 0.2f, WHITE);
            DrawTextureEx(uk, (Vector2){ player2 -> position.x - 50, player2 -> position.y - 50}, rotate, 0.2f, WHITE);

            // Draw created physics bodies
            int bodiesCount = GetPhysicsBodiesCount();
            for (int i = 0; i < bodiesCount; i++)
            {
                PhysicsBody payer1 = GetPhysicsBody(i);

                int vertexCount = GetPhysicsShapeVerticesCount(i);
                for (int j = 0; j < vertexCount; j++)
                {
                    // Get physics bodies shape vertices to draw lines
                    // Note: GetPhysicsShapeVertex() already calculates rotation transformations
                    Vector2 vertexA = GetPhysicsShapeVertex(player1, j);

                    int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
                    Vector2 vertexB = GetPhysicsShapeVertex(player1, jj);

                    DrawLineV(vertexA, vertexB, BLUE);     // Draw a line between two vertex positions
                }
            }

            DrawText("Use 'ARROWS' to move player", 10, 10, 10, WHITE);
            DrawText("Press 'R' to reset example", 10, 30, 10, WHITE);

            DrawText("Physac", logoX, logoY, 30, WHITE);
            DrawText("Powered by", logoX + 50, logoY - 7, 10, WHITE);
            DrawText("ballon", multipla_body->position.x - MeasureText("ballon", 20)/2, multipla_body->position.y - 7, 20, WHITE);
            DrawText("but", goalLeft->position.x - MeasureText("but", 20)/2, goalLeft->position.y - 7, 20, WHITE);
            DrawText("but", goalRight->position.x - MeasureText("but", 20)/2, goalRight->position.y - 7, 20, WHITE);

            DrawText("P1",player1->position.x - MeasureText("P1", 30)/2,player1->position.y -120, 30, GREEN);
            DrawText("V",player1->position.x - MeasureText("V", 20)/2,player1->position.y -90, 20, GREEN);

            DrawText("P2", player2->position.x - MeasureText("P2", 30)/2, player2->position.y -120, 30, GREEN);
            DrawText("V", player2->position.x - MeasureText("V", 20)/2, player2->position.y -90, 20, GREEN);


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
