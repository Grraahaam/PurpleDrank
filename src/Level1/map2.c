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
    Image soinc_right = LoadImage("../../res/soinc.png");
    Image soinc_left = LoadImage("../../res/soinc_reverse.png");
    Image back2 = LoadImage("../../res/Level1/ECRAN2V.png");
    Image soinc_head = LoadImage("../../res/soinc_head.png");
    Image soinc_dead = LoadImage("../../res/soinc_dead.png");
    Image img_spikes = LoadImage("../../res/Level1/spikes.png");
    Image img_tremplin = LoadImage("../../res/Level1/tremplin.png");
    Image img_teleportation = LoadImage("../../res/Level1/tp.png");
    Image img_tp_desac = LoadImage("../../res/Level1/tp_desac.png");
    Image img_lean = LoadImage("../../res/Level1/lean.png");
    Image empty = LoadImage("../../res/Level1/empty.png");
    float vitesse = VELOCITY*0.4;
    bool boule = true, right = true, col_piques = false, canMove = true, saut_tremplin = false, col_lean1 = false, col_lean2 = false, col_lean3 = false, l1 = false, l2 = false, l3 = false, col_tp = false, activate = false, desactivate = false, victory = false;
    int fall = 0;
    int nb_lives = 5;
    int time_spikes = 0;
    int nb_lean = 0;
    int time_tp = 0;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "soinc à la recherche du lean");


    // Initialize physics and default physics bodies
    InitPhysics();

    // Create floor and walls rectangle physics body
    PhysicsBody floor1 = CreatePhysicsBodyRectangle((Vector2){ 35, 360 }, 70, 190, 10);
    PhysicsBody floor2 = CreatePhysicsBodyRectangle((Vector2){ 115, 405 }, 90, 50, 10);
    PhysicsBody floor3 = CreatePhysicsBodyRectangle((Vector2){ 218, 390 }, 116, 70, 10);
    PhysicsBody floor4 = CreatePhysicsBodyRectangle((Vector2){ 341, 402 }, 130, 50, 10);
    PhysicsBody floor5 = CreatePhysicsBodyPolygon((Vector2){ 462, 420 }, 70.0f, 5, 10); 
    PhysicsBody floor6 = CreatePhysicsBodyRectangle((Vector2){ 520, 388.5 }, 70, 70, 10); 
    PhysicsBody floor7 = CreatePhysicsBodyPolygon((Vector2){ 552, 427 }, 75.0f, 8, 10);   
    PhysicsBody platform11 = CreatePhysicsBodyRectangle((Vector2){ 225 , 55 }, 150, 110, 10);
    PhysicsBody platform12 = CreatePhysicsBodyRectangle((Vector2){ 335, 43 }, 70, 86, 10);
    PhysicsBody platform13 = CreatePhysicsBodyPolygon((Vector2){ 370, 10 }, 75.0f, 4, 10);   
    PhysicsBody platform2 = CreatePhysicsBodyRectangle((Vector2){ 705, 245 }, 180, 350, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ -5, screenHeight/2 }, 10, screenHeight*2, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth + 5, screenHeight/2 }, 10, screenHeight*2, 10);

    // Disable dynamics to floor and walls physics bodies
    floor1->enabled = false;
    floor2->enabled = false;
    floor3->enabled = false;
    floor4->enabled = false;
    floor5->enabled = false;
    floor6->enabled = false;
    floor7->enabled = false;
    platform11->enabled = false;
    platform12->enabled = false;
    platform13->enabled = false;
    platform2->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;

    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){ 40, screenHeight/2 }, 50, 60, 1);
    body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
        
    
    Texture2D soinc = LoadTextureFromImage(soinc_right);
    Texture2D background = LoadTextureFromImage(back2);
    Texture2D lives = LoadTextureFromImage(soinc_head);
    Texture2D spikes = LoadTextureFromImage(img_spikes);
    Texture2D tremplin = LoadTextureFromImage(img_tremplin);
    Texture2D teleportation = LoadTextureFromImage(img_tp_desac);
    Texture2D inventory_lean = LoadTextureFromImage(img_lean);
    Texture2D lean1 = LoadTextureFromImage(img_lean);
    Texture2D lean2 = LoadTextureFromImage(img_lean);
    Texture2D lean3 = LoadTextureFromImage(img_lean);
    
        
    Rectangle rect_lean1 = { 650, 30, 25, 40 };
    Rectangle rect_lean2 = { 700, 30, 25, 40 };
    Rectangle rect_lean3 = { 750, 30, 25, 40 };
    Rectangle rect_tremplin = {310, 365, 10, 10};
    Rectangle piques = { 70, 370, 80, 20 };
    Rectangle rect_tp = { 600, 300, 10, 70};
        
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        void reset_lean(){
            lean1 = LoadTextureFromImage(img_lean);
            lean2 = LoadTextureFromImage(img_lean);
            lean3 = LoadTextureFromImage(img_lean);
            l1 = false;
            l2 = false;
            l3 = false;
            nb_lean = 0;
            }
        
        RunPhysicsStep();
        
        Rectangle rect_soinc = { body -> position.x - 30, body -> position.y - 30, 60, 60 };
        
        // Horizontal movement input
        if (IsKeyDown(KEY_RIGHT) && canMove) {
        body->velocity.x = vitesse;
        if (boule && !right){
        soinc = LoadTextureFromImage(soinc_right);
        }
        right = true;
        }        
        else if (IsKeyDown(KEY_LEFT) && canMove) {
        body->velocity.x = -vitesse;
        if (boule && right){
        soinc = LoadTextureFromImage(soinc_left);
        }
        right = false;
        }
        
        col_piques = CheckCollisionRecs(rect_soinc, piques);
	
	if(col_piques){
		if(time_spikes == 0){
			soinc = LoadTextureFromImage(soinc_dead);
			canMove = false;
			time_spikes++;
		}
		else{
			time_spikes++;
		}
	}
	
	
	if(time_spikes> 30){
	body->position.x = 40;
	body->position.y = screenHeight/2;
	nb_lives--;
	time_spikes = 0;
	soinc = LoadTextureFromImage(soinc_right);
	canMove = true;
	reset_lean();
	}
	
	saut_tremplin = CheckCollisionRecs(rect_soinc, rect_tremplin);
	
	if(saut_tremplin && IsKeyDown(KEY_SPACE)){
	body->velocity.y = -VELOCITY*5.5;
	body->velocity.x = +VELOCITY*2;
	}
        

        // Vertical movement input checking if player physics body is grounded
        if (IsKeyDown(KEY_UP) && body->isGrounded && canMove) {
        body->velocity.y = -VELOCITY*3;
        }
        
        col_lean1 = CheckCollisionRecs(rect_soinc, rect_lean1);
	col_lean2 = CheckCollisionRecs(rect_soinc, rect_lean2);
	col_lean3 = CheckCollisionRecs(rect_soinc, rect_lean3);
            
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
            
            col_tp = CheckCollisionRecs(rect_soinc, rect_tp);
	
	if(col_tp) time_tp++;
	
	if(!col_tp) time_tp = 0;
	
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            
            DrawTextureEx(background, (Vector2){0, 0}, 0.0f, 0.85f, WHITE);
            
            DrawTextureEx(soinc, (Vector2){ body -> position.x - 40, body -> position.y - 30}, 0.0f, 0.15f, WHITE);
            
            DrawTextureEx(lives, (Vector2){10, 20}, 0.0f, 0.25f, WHITE);
            DrawText(TextFormat("%d", nb_lives), 90, 25, 30, WHITE);
            
            DrawTextureEx(spikes, (Vector2){70, 370}, 0.0f, 0.13f, WHITE);
            
            DrawTextureEx(tremplin, (Vector2){300, 330}, 38.8f, 0.13f, WHITE);
            
            DrawTextureEx(teleportation, (Vector2){555, 280}, 0.0f, 0.1f, WHITE);
            
            DrawTextureEx(inventory_lean, (Vector2){30, 60}, 0.0f, 0.08f, WHITE);
            DrawText(TextFormat("%d", nb_lean), 90, 70, 30, WHITE);
            
            DrawTextureEx(lean1, (Vector2){650, 30}, 0.0f, 0.08f, WHITE);
            DrawTextureEx(lean2, (Vector2){700, 30}, 0.0f, 0.08f, WHITE);
            DrawTextureEx(lean3, (Vector2){750, 30}, 0.0f, 0.08f, WHITE);
            
            if(fall>1) DrawText("Utilisez la plateforme pour arriver de l'autre côté", 150, 100, 20, RED);
            
            //DrawRectangleRec(rect_soinc, WHITE);
            //DrawRectangleRec(piques, GOLD);
            //DrawRectangleRec(rect_tremplin, WHITE);

	    //DrawRectangleRec(rect_lean1, WHITE);
	    //DrawRectangleRec(rect_lean2, WHITE);
	    //DrawRectangleRec(rect_lean3, WHITE);
	    
	    //DrawRectangleRec(rect_tp, WHITE);

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
            
        if(time_tp> 30 && nb_lean == 3){
        if(!victory){
        teleportation = LoadTextureFromImage(img_teleportation);
        victory = true;
        }
        DrawText("Bien joué, vous avez activé le téléporteur!", 100, 100, 20, RED);
	canMove = false;
	}
	else if (time_tp> 30 && nb_lean < 3){
        DrawText("Vous n'avez pas assez de lean pour activer le téléporteur!", 100, 100, 20, RED);
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
