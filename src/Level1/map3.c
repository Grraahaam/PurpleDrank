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
    Image solin_right = LoadImage("../../res/solin.png");
    Image solin_left = LoadImage("../../res/solin_reverse.png");
    Image solin_ball = LoadImage("../../res/boule.png");
    Image back = LoadImage("../../res/Level1/ECRAN3V.png");
    Image solin_head = LoadImage("../../res/solin_head.png");
    Image solin_dead = LoadImage("../../res/solin_dead.png");
    Image img_spikes = LoadImage("../../res/Level1/spikes.png");
    Image img_tremplin = LoadImage("../../res/Level1/tremplin.png");
    Image img_teleportation = LoadImage("../../res/Level1/tp.png");
    Image img_tp_desac = LoadImage("../../res/Level1/tp_desac.png");
    Image img_lean = LoadImage("../../res/Level1/lean.png");
    Image empty = LoadImage("../../res/Level1/empty.png");
    Image img_car = LoadImage("../../res/Level1/carV2.png");
    Image img_goblean = LoadImage("../../res/Level1/goblean.png");
    Image goblean_dead = LoadImage("../../res/Level1/goblean_dead.png");
    Image img_mainD = LoadImage("../../res/Level1/main_droite.png");
    Image img_mainG = LoadImage("../../res/Level1/main_gauche.png");
    float vitesse = VELOCITY*0.4;
    bool boule = false, right = true, col_piques = false, canMove = false, saut_tremplin = false, col_lean1 = false, col_lean2 = false, col_lean3 = false, l1 = false, l2 = false, l3 = false, col_tp = false, activate = false, desactivate = false, victory = false, start = true, lancer = false, lean_right = false, lean_left = false, destroy = true, col_car_left = false, col_car_right = false, col_car_top = false, col_lean_car_left = false, col_lean_car_right = false, lean_activate = false, col_goblean_lean = false, col_goblean_solin = false, col_skate_solin = false, col_skate_lean = false, end = false, lance_mainD = true, lance_mainG = true;
    int nb_lives = 5, time_spikes = 0, nb_lean = 3, time_tp = 0, time_killed = 0, time_kill = 0, car_killed = 0, goblean_hurt = 0;
    float leanX, leanY, mainGx, mainGy, mainDx, mainDy;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "s/o'lin à la recherche du lean");


    // Initialize physics and default physics bodies
    InitPhysics();

    // Create floor and walls rectangle physics body
    PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2){ 435, 380 }, 730, 190, 10);
    PhysicsBody platform = CreatePhysicsBodyRectangle((Vector2){ 35, 330 }, 70, 240, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ -5, screenHeight/2 }, 10, screenHeight*2, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth + 5, screenHeight/2 }, 10, screenHeight*2, 10);
    PhysicsBody car = CreatePhysicsBodyRectangle((Vector2){ 1000, 260}, 140, 50, 10);
    PhysicsBody goblean = CreatePhysicsBodyRectangle((Vector2){ 1000, 180}, 120, 210, 10);

    // Disable dynamics to floor and walls physics bodies
    floor->enabled = false;
    platform->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;
    car-> enabled = false;
    goblean -> enabled = false;

    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){ 40, screenHeight/2 }, 50, 60, 1);
    body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
        
    
    Texture2D solin = LoadTextureFromImage(solin_right);
    Texture2D lives = LoadTextureFromImage(solin_head);
    Texture2D background = LoadTextureFromImage(back);
    Texture2D teleportation = LoadTextureFromImage(img_teleportation);
    Texture2D inventory_lean = LoadTextureFromImage(img_lean);
    Texture2D lean = LoadTextureFromImage(empty);
    Texture2D car_ennemy = LoadTextureFromImage(img_car);
    Texture2D goblean_boss = LoadTextureFromImage(img_goblean);
    Texture2D goblean_mainD = LoadTextureFromImage(empty);
    Texture2D goblean_mainG = LoadTextureFromImage(empty);

    
    Rectangle rect_tp = { 15, 140, 10, 70};
        
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        RunPhysicsStep();
        
        if(lance_mainD){
        mainDx -= 5.0f;
        mainDy += 0.8f;
        }
        
        if(lance_mainG) {
        mainGx -= 5.0f;
        mainGy += 0.8f;
        }
        
        if(lean_right) leanX += 5.0f;
        
        if(lean_left) leanX -= 5.0f;
        
        if(car_killed < 4) car -> position.x -= 0.9f;
        
        if(car_killed >= 4){
        car -> position.x = 1500;
        if(goblean -> position.x > 500 && !victory){
        goblean -> position.x -= 1.0f;
        }
        }
        
        if(goblean -> position.x == 600 && !victory){
        mainDx = goblean -> position.x;
        mainDy = goblean -> position.y;
        goblean_mainD = LoadTextureFromImage(img_mainD);
        lance_mainD = true;
        }
        
        if(goblean -> position.x == 550 && !victory){
        mainGx = goblean -> position.x;
        mainGy = goblean -> position.y;
        goblean_mainG = LoadTextureFromImage(img_mainG);
        lance_mainG = true;
        }
        
        Rectangle rect_solin = { body -> position.x - 30, body -> position.y - 30, 60, 60 };
        
        Rectangle rect_lean = { leanX, leanY - 20, 25, 40 };
        
        Rectangle rect_car_top = { car-> position.x - 60, car-> position.y - 30, 120, 10 };  
        Rectangle rect_car_left = { car-> position.x - 80, car-> position.y, 10, 20 };      
        Rectangle rect_car_right = { car-> position.x + 70, car-> position.y, 10, 20 }; 
        
        Rectangle rect_goblean = { goblean-> position.x - 60, goblean-> position.y - 100, 120, 160 };
        Rectangle rect_skate = { goblean-> position.x - 95, goblean-> position.y + 62, 180, 40 };
        
        // Horizontal movement input
        if (IsKeyDown(KEY_RIGHT) && canMove) {
        body->velocity.x = vitesse;
        if (!boule && !right){
        solin = LoadTextureFromImage(solin_right);
        }
        right = true;
        }        
        else if (IsKeyDown(KEY_LEFT) && canMove) {
        body->velocity.x = -vitesse;
        if (!boule && right){
        solin = LoadTextureFromImage(solin_left);
        }
        right = false;
        }
        
        else if (IsKeyPressed(KEY_DOWN) && !boule) {
        solin = LoadTextureFromImage(solin_ball);
        vitesse = vitesse*3;
        boule = true;
        }
        
        else if (IsKeyPressed(KEY_DOWN) && boule && vitesse > 0.5 ) {
        solin = LoadTextureFromImage(solin_right);
        vitesse = vitesse/3;
        boule = false;
        }
        
        if (IsKeyPressed(KEY_B) && nb_lean > 0 && destroy && !lean_activate){
        leanX = body -> position.x;
        leanY = body -> position.y;
        lean = LoadTextureFromImage(img_lean);
        nb_lean --;
        lean_activate = true;
	if(right) lean_right = true;
	if(!right) lean_left = true;
        }        

        

        // Vertical movement input checking if player physics body is grounded
        if (IsKeyDown(KEY_UP) && body->isGrounded && canMove) {
        body->velocity.y = -VELOCITY*3;
        }
            
            
       col_tp = CheckCollisionRecs(rect_solin, rect_tp);
	
	if(col_tp && start) time_tp++;
	
	if(time_tp> 60 && start){
	canMove = true;
	teleportation = LoadTextureFromImage(img_tp_desac);
	start = false;
	}
	
	col_car_left = CheckCollisionRecs(rect_solin, rect_car_left);
	col_car_right = CheckCollisionRecs(rect_solin, rect_car_right);
	col_car_top = CheckCollisionRecs(rect_solin, rect_car_top);
	col_lean_car_left = CheckCollisionRecs(rect_lean, rect_car_left);
	col_lean_car_right = CheckCollisionRecs(rect_lean, rect_car_right);
	
	if(col_car_left || col_car_right) time_killed++;
	
	if(time_killed > 10){
	body->position.x = 40;
	body->position.y = screenHeight/2;
	nb_lives--;	
	time_killed = 0;
	time_kill = 0;
	}
	
	if((col_car_left || col_car_right) && boule){
	car-> position.x = 1000;
	car_killed ++;
	time_kill = 0;
	time_killed = 0;
	}	
	
	if(col_car_top) time_kill++;
	
	if(time_kill > 5){
	car-> position.x = 1000;
	car_killed ++;
	time_kill = 0;
	time_killed = 0;
	}
	
	if((col_lean_car_left || col_lean_car_right) && lean_activate){
	car-> position.x = 1000;
	car_killed ++;
	time_kill = 0;
	time_killed = 0;
	lean = LoadTextureFromImage(empty);
	lean_left = false;
	lean_right = false;
	lean_activate = false;
	destroy = true;
	}
	
	if(car-> position.x < -50) car -> position.x = 1000;
	
	if((leanX < -50) || (leanX > 850)) {
	lean_left = false;
	lean_right = false;
	lean_activate = false;
	destroy = true;
	}
	
	col_goblean_lean = CheckCollisionRecs(rect_lean, rect_goblean);
	col_goblean_solin = CheckCollisionRecs(rect_solin, rect_goblean);
	col_skate_lean = CheckCollisionRecs(rect_lean, rect_skate);
	col_skate_solin = CheckCollisionRecs(rect_solin, rect_skate);
	
	if(!victory){
	if(col_goblean_lean && lean_activate){
	goblean_hurt += 2;
	goblean -> position.x = 750;
	lean = LoadTextureFromImage(empty);
	lean_left = false;
	lean_right = false;
	lean_activate = false;
	destroy = true;
	}
	
	if((col_goblean_solin || col_skate_solin) && boule){
	goblean_hurt += 1;
	goblean -> position.x = 650;
	body -> position.x -= 200;
	}
	
	if((col_goblean_solin || col_skate_solin) && !boule) time_killed++;
	
	if(col_skate_lean && lean_activate){
	lean = LoadTextureFromImage(empty);
	lean_left = false;
	lean_right = false;
	lean_activate = false;
	destroy = true;
	}
	}
	
	if(goblean_hurt == 6){
	victory = true;
	goblean -> position.x = 550;
	goblean -> position.y = 270;
	}
	
	if(victory && !end){
	goblean_boss = LoadTextureFromImage(goblean_dead);
	end = true;
	}
	
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            
            ClearBackground(BLACK);
            
            
            DrawTextureEx(background, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
            
            DrawTextureEx(solin, (Vector2){ body -> position.x - 40, body -> position.y - 30}, 0.0f, 0.15f, WHITE);
            
            DrawTextureEx(lives, (Vector2){10, 20}, 0.0f, 0.25f, WHITE);
            DrawText(TextFormat("%d", nb_lives), 90, 25, 30, WHITE);
            
            DrawTextureEx(teleportation, (Vector2){5, 125}, 0.0f, 0.1f, WHITE);
            
            DrawTextureEx(inventory_lean, (Vector2){30, 60}, 0.0f, 0.08f, WHITE);
            DrawText(TextFormat("%d", nb_lean), 90, 70, 30, WHITE);
            
            DrawTextureEx(lean, (Vector2){leanX, leanY - 20}, 0.0f, 0.08f, WHITE); 
            
            DrawTextureEx(car_ennemy, (Vector2){ car-> position.x - 75, car-> position.y - 40 }, 0.0f, 0.1f, WHITE );  
            
            DrawTextureEx(goblean_boss, (Vector2){ goblean-> position.x - 115, goblean-> position.y - 110 }, 0.0f, 0.2f, WHITE );   
            
            DrawTextureEx(goblean_mainD, (Vector2){mainDx - 80, mainDy - 40}, 0.0f, 0.2f, WHITE);
            DrawTextureEx(goblean_mainG, (Vector2){mainGx + 20, mainGy - 40}, 0.0f, 0.2f, WHITE); 
            
            if(victory){
            DrawText("VICTOIRE!!!", 300, 100, 20, RED);   
            }
            
            //DrawRectangleRec(rect_solin, WHITE);
	    
	    //DrawRectangleRec(rect_tp, WHITE);
	    
	    //DrawRectangleRec(rect_car_top, GOLD);
	    //DrawRectangleRec(rect_car_left, GOLD);
	    //DrawRectangleRec(rect_car_right, GOLD);	    
	    //DrawRectangleRec(rect_lean, GOLD);
	    
	    //DrawRectangleRec(rect_goblean, RED);
	    //DrawRectangleRec(rect_skate, GOLD);

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
