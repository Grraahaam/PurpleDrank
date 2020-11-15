#ifndef MAIN_H_
#define MAIN_H_

// Function declarations
void UpdateScreen();
void MenuDraw();
void GameoverDraw();
void LevelOneDraw(Player *player_Struct);
void LevelTwoDraw(Player *player_Struct);
void LoadResources();
void UnloadResources();

int getdir();
char *getcwd(char *buf, size_t size);

void PrintDebug(char *str);

#endif

