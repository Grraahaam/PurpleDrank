#ifndef LEVEL3_H_
#define LEVEL3_H_

void LeanAvance();
void LaunchLean(Player *player_Struct, PhysicsBody *body);
void LevelThreeDraw(Player *player_Struct);
void LevelThreeRead(Player *player_Struct, PhysicsBody *body, Rectangle *rect_goblean, Rectangle *rect_skate, Rectangle *rect_solin, Rectangle *rect_lean, Rectangle *rect_car_top, Rectangle *rect_car_left, Rectangle *rect_car_right);
void LevelThreeCheck_Event(Player *player_Struct, PhysicsBody *body, Rectangle *rect_piques, Rectangle *rect_tp, Rectangle *rect_solin, Rectangle *rect_lean1, Rectangle *rect_lean2, Rectangle *rect_lean3);
void PrintDebug(char *str);

#endif
