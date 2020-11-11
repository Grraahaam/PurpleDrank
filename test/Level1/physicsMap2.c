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