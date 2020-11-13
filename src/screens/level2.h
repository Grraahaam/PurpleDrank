#ifndef LEVEL2_H_
#define LEVEL2_H_

void LevelTwoDraw(Player *player_Struct);
void LevelTwoRead(Player *player_Struct,PhysicsBody body, Rectangle trou, Rectangle wall_right, Rectangle rect_solin);
void Check_EventLvl2(Player *player_Struct,PhysicsBody body, Rectangle trou, Rectangle wall_right, Rectangle rect_solin);
bool Fallen_Hole(Rectangle trou, Rectangle rect_solin);
bool End_Level(Rectangle wall_right, Rectangle rect_solin);

#endif