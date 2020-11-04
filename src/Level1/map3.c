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
    Image back = LoadImage("../../res/Level1/ECRAN3V.png");
    Image soinc_head = LoadImage("../../res/soinc_head.png");
    Image soinc_dead = LoadImage("../../res/soinc_dead.png");
    Image img_spikes = LoadImage("../../res/Level1/spikes.png");
    Image img_tremplin = LoadImage("../../res/Level1/tremplin.png");
    Image img_teleportation = LoadImage("../../res/Level1/tp.png");
    Image img_tp_desac = LoadImage("../../res/Level1/tp_desac.png");
    Image img_lean = LoadImage("../../res/Level1/lean.png");
    Image empty = LoadImage("../../res/Level1/empty.png");
    Image img_car = LoadImage("../../res/Level1/carV2.png");
    float vitesse = VELOCITY*0.4;
    bool boule = true, right = true, col_piques = false, canMove = false, saut_tremplin = false, col_lean1 = false, col_lean2 = false, col_lean3 = false, l1 = false, l2 = false, l3 = false, col_tp = false, activate = false, desactivate = false, victory = false, start = true, lancer = false, lean_right = false, lean_left = false, destroy = true, col_car_left = false, col_car_right = false, col_car_top = false, col_lean_car_left = false, col_lean_car_right = false;
    int fall = 0;
    int nb_lives = 5;
    int time_spikes = 0;
    int nb_lean = 5;
    int time_tp = 0;
    int time_killed = 0, time_kill = 0;
    float leanX, leanY;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "soinc à la recherche du lean");


    // Initialize physics and default physics bodies
    InitPhysics();

    // Create floor and walls rectangle physics body
    PhysicsBody floor1 = CreatePhysicsBodyRectangle((Vector2){ screenWidth/2, 380 }, screenWidth, 190, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ -5, screenHeight/2 }, 10, screenHeight*2, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth + 5, screenHeight/2 }, 10, screenHeight*2, 10);
    PhysicsBody car = CreatePhysicsBodyRectangle((Vector2){ 600, 260}, 140, 50, 10);

    // Disable dynamics to floor and walls physics bodies
    floor1->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;
    car-> enabled = false;

    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){ 40, screenHeight/2 }, 50, 60, 1);
    body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
        
    
    Texture2D soinc = LoadTextureFromImage(image);
    Texture2D lives = LoadTextureFromImage(soinc_head);
    Texture2D background = LoadTextureFromImage(back);
    Texture2D teleportation = LoadTextureFromImage(img_teleportation);
    Texture2D inventory_lean = LoadTextureFromImage(img_lean);
    Texture2D lean = LoadTextureFromImage(empty);
    Texture2D car_ennemy = LoadTextureFromImage(img_car);

    
    Rectangle rect_tp = { 30, 220, 10, 70};
        
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        RunPhysicsStep();
        
        if(lean_right) leanX += 5.0f;
        else if (!lean_right) leanX = body-> position.x;
        
        if(lean_left) leanX -= 5.0f;
        else if(!lean_left) leanX = body-> position.x;
        
        car -> position.x -= 0.9f;
        
        Rectangle rect_soinc = { body -> position.x - 30, body -> position.y - 30, 60, 60 };
        
        Rectangle rect_lean = { leanX, leanY - 20, 25, 40 };
        
        Rectangle rect_car_top = { car-> position.x - 70, car-> position.y - 30, 140, 10 };  
        Rectangle rect_car_left = { car-> position.x - 80, car-> position.y, 10, 20 };      
        Rectangle rect_car_right = { car-> position.x + 70, car-> position.y, 10, 20 };      
        
        // Horizontal movement input
        if (IsKeyDown(KEY_RIGHT) && canMove) {
        body->velocity.x = vitesse;
        if (boule && !right){
        soinc = LoadTextureFromImage(image);
        }
        right = true;
        }        
        else if (IsKeyDown(KEY_LEFT) && canMove) {
        body->velocity.x = -vitesse;
        if (boule && right){
        soinc = LoadTextureFromImage(image3);
        }
        right = false;
        }
        
        if (IsKeyPressed(KEY_B) && nb_lean > 0 && !lean_right && !lean_left){
        leanX = body -> position.x;
        leanY = body -> position.y;
        lean = LoadTextureFromImage(img_lean);
        nb_lean --;
	if(right) lean_right = true;
	if(!right) lean_left = true;
        }        

        

        // Vertical movement input checking if player physics body is grounded
        if (IsKeyDown(KEY_UP) && body->isGrounded && canMove) {
        body->velocity.y = -VELOCITY*3;
        }
            
            
       col_tp = CheckCollisionRecs(rect_soinc, rect_tp);
	
	if(col_tp && start) time_tp++;
	
	if(time_tp> 60 && start){
	canMove = true;
	teleportation = LoadTextureFromImage(img_tp_desac);
	start = false;
	}
	
	col_car_left = CheckCollisionRecs(rect_soinc, rect_car_left);
	col_car_right = CheckCollisionRecs(rect_soinc, rect_car_right);
	col_car_top = CheckCollisionRecs(rect_soinc, rect_car_top);
	col_lean_car_left = CheckCollisionRecs(rect_lean, rect_car_left);
	col_lean_car_right = CheckCollisionRecs(rect_lean, rect_car_right);
	
	if(col_car_left || col_car_right) time_killed++;
	
	if(time_killed > 10){
	body->position.x = 80;
	body->position.y = screenHeight/2;
	nb_lives--;	
	time_killed = 0;
	time_kill = 0;
	}
	
	if(col_car_top ) time_kill++;
	
	if(time_kill > 10){
	car-> position.x = 1000;
	time_kill = 0;
	time_killed = 0;
	}
	
	if(col_lean_car_left || col_lean_car_right){
	car-> position.x = 1000;
	time_kill = 0;
	time_killed = 0;
	lean = LoadTextureFromImage(empty);
	lean_left = false;
	lean_right = false;
	}
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            
            ClearBackground(BLACK);
            
            
            DrawTextureEx(background, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
            
            DrawTextureEx(soinc, (Vector2){ body -> position.x - 40, body -> position.y - 30}, 0.0f, 0.15f, WHITE);
            
            DrawTextureEx(lives, (Vector2){10, 20}, 0.0f, 0.25f, WHITE);
            DrawText(TextFormat("%d", nb_lives), 90, 25, 30, WHITE);
            
            DrawTextureEx(teleportation, (Vector2){20, 200}, 0.0f, 0.1f, WHITE);
            
            DrawTextureEx(inventory_lean, (Vector2){30, 60}, 0.0f, 0.08f, WHITE);
            DrawText(TextFormat("%d", nb_lean), 90, 70, 30, WHITE);
            
            DrawTextureEx(lean, (Vector2){leanX, leanY - 20}, 0.0f, 0.08f, WHITE); 
            
            DrawTextureEx(car_ennemy, (Vector2){ car-> position.x - 75, car-> position.y - 40 }, 0.0f, 0.1f, WHITE );       
            
            //DrawRectangleRec(rect_soinc, WHITE);
	    
	    //DrawRectangleRec(rect_tp, WHITE);
	    
	    //DrawRectangleRec(rect_car_top, GOLD);
	    //DrawRectangleRec(rect_car_left, GOLD);
	    //DrawRectangleRec(rect_car_right, GOLD);	    
	    //DrawRectangleRec(rect_lean, GOLD);

            //Draw created physics bodies
            /*int bodiesCount = GetPhysicsBodiesCount();
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
            }*/

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
