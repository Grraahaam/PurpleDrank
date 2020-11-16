#ifndef LEVEL2_H_
#define LEVEL2_H_

void reset_lean();
bool use_teleporter(Player *player_Struct, Rectangle *rect_solin, Rectangle *rect_tp);
void saut_tremplin(Player *player_Struct, PhysicsBody *body, Rectangle *rect_solin, Rectangle *rect_tremplin);
bool GetLean(Rectangle *rect_lean1, Rectangle *rect_lean2, Rectangle *rect_lean3 , Rectangle *rect_solin);
bool Fallen_Spikes(Rectangle *rect_piques, Rectangle *rect_solin);
bool LevelTwoEnd(Player *player_Struct, Rectangle *rect_tp, Rectangle *rect_solin);
void LevelTwoDraw(Player *player_Struct);
void LevelTwoRead(Player *player_Struct, PhysicsBody *body, Rectangle *rect_piques, Rectangle *rect_tp, Rectangle *rect_solin, Rectangle *rect_lean1, Rectangle *rect_lean2, Rectangle *rect_lean3, Rectangle *rect_tremplin);
void LevelTwoCheck_Event(Player *player_Struct, PhysicsBody *body, Rectangle *rect_piques, Rectangle *rect_tp, Rectangle *rect_solin, Rectangle *rect_lean1, Rectangle *rect_lean2, Rectangle *rect_lean3, Rectangle *rect_tremplin);


#endif
