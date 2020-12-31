#ifndef LEVEL4_H_
    
    #define LEVEL4_H_

    void LevelFourDraw(Player *player, ScreenFX *screenFx);
    void LevelFourInit(Player *player);
    void LevelFourRead(Player *player, Asset *spikes);
    
    void l4_readCollisions(Player *player, Asset *spikes);
    
    bool l4_collisionPortal(Player *player);
    bool l4_collisionSpikes(Player *player, Asset *spikes);
    bool l4_collisionHoleBonus(Player *player);

#endif
