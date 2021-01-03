#ifndef LEVEL3_H_

    #define LEVEL3_H_
    
    Damage l3_collisionLean(Asset *lean, Asset *fire, Enemy *gob, Enemy *dark_solin);
    Damage l3_collisionPlayer(Player *player, Enemy *gob, Enemy *dark_solin, Asset *fire);
    
    void l3_throwLean(Player *player, Asset *lean);
    void l3_resetGob(Asset *asset);
    
    void l3_playDamage(Asset *ast);
    
    void l3_readDamage(Damage dtype, Damage actor, Player *player, Asset *lean, Asset *fire, Enemy *gob, Enemy *dark_solin);
    void l3_readCollisions(Player *player, Asset *lean, Asset *fire, Enemy *gob, Enemy *dark_solin);
    
    void LevelThreeRead(Player *player, Asset *lean, Asset *fire, Enemy *gob, Enemy *dark_solin);
    void LevelThreeInit(Player *player);
    void LevelThreeDraw(Player *player, ScreenFX *screenFx);

#endif
