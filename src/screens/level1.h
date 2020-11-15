#ifndef LEVEL1_H_
#define LEVEL1_H_

bool Fallen_Hole(Rectangle *trou, Rectangle *rect_solin);
bool End_Level(Rectangle *wall_right, Rectangle *rect_solin);
void LevelOneDraw(Player *player_Struct);
void LevelOneRead(Player *player_Struct, PhysicsBody *body, Rectangle *trou, Rectangle *wall_right, Rectangle *rect_solin);
void Check_Event(Player *player_Struct, PhysicsBody *body, Rectangle *trou, Rectangle *wall_right, Rectangle *rect_solin);

void PrintDebug(char *str);

#endif
