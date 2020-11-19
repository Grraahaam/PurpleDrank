#ifndef LEVEL2_H_
#define LEVEL2_H_

void reset_lean();
void saut_tremplin(PhysicsBody *body, Rectangle *rect_solin, Rectangle *rect_tremplin);

bool use_teleporter(Player *player, Rectangle *rect_solin, Rectangle *rect_tp);
bool GetLean(Rectangle *rect_lean1, Rectangle *rect_lean2, Rectangle *rect_lean3 , Rectangle *rect_solin);
bool Fallen_Spikes(Rectangle *rect_piques, Rectangle *rect_solin);
bool LevelTwoEnd(Player *player, Rectangle *rect_tp, Rectangle *rect_solin);

void LevelTwoDraw(Player *player);
void LevelTwoRead(Player *player, PhysicsBody *body, Rectangle *rect_piques, Rectangle *rect_tp, Rectangle *rect_solin, Rectangle *rect_lean1, Rectangle *rect_lean2, Rectangle *rect_lean3, Rectangle *rect_tremplin);
void LevelTwoCheck_Event(Player *player, PhysicsBody *body, Rectangle *rect_piques, Rectangle *rect_tp, Rectangle *rect_solin, Rectangle *rect_lean1, Rectangle *rect_lean2, Rectangle *rect_lean3);

void PrintDebug(char *str);

#endif
