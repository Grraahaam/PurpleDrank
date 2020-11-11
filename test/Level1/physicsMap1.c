

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
    