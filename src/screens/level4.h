#ifndef LEVEL4_H_
    
    #define LEVEL4_H_

    void LevelFourDraw(Player *player, ScreenFX *screenFx);
    void LevelFourInit(Player *player);
    void LevelFourRead(Player *player);
    void l4_readCollisions(Player *player);
    bool l4_collisionPortal(Player *player);
    bool l4_collisionSpikes(Player *player);
    void l4_readCollisions(Player *player);
    bool l4_collisionHoleBonus(Player *player);
    bool l4_collisionHole(Player *player);

#endif
