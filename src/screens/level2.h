#ifndef LEVEL2_H_
#define LEVEL2_H_


bool End_Level(Rectangle *wall_right, Rectangle *rect_solin);
void LevelTwoDraw(Player *player_Struct);
void LevelTwoRead(Player *player_Struct, PhysicsBody *body, Rectangle *wall_right, Rectangle *rect_solin);
void Check_Event(Player *player_Struct, PhysicsBody *body, Rectangle *wall_right, Rectangle *rect_solin);


#endif