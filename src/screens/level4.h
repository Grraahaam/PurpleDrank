
#ifndef LEVEL4_H_
    
    #define LEVEL4_H_

    bool l4_collisionSpikes(Player *player, Asset *spikes);
    
    void l4_readCollisions(Player *player, Asset *spikes, Asset *portalBonus);

    void LevelFourRead(Player *player, Asset *spikes, Asset *portalBonus);
    void LevelFourInit(Player *player);
    void LevelFourDraw(Player *player, ScreenFX *screenFx);    

#endif
