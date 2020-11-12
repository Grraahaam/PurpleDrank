void LevelOneDraw(int *player_health);
void LevelOneRead(int *player_health, PhysicsBody body, Rectangle trou, Rectangle wall_right, Rectangle rect_solin);
void Check_Event(int *player_health, PhysicsBody body, Rectangle trou, Rectangle wall_right, Rectangle rect_solin);
bool Fallen_Hole(Rectangle trou, Rectangle rect_solin);
bool End_Level(Rectangle wall_right, Rectangle rect_solin);

