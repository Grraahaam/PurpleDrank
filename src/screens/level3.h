#ifndef LEVEL3_H_

    #define LEVEL3_H_
    
    bool l3_collisionAssets(Asset *ast1, Asset *ast2);
    Damage l3_collisionLean(Asset *lean, Enemy *gob, Enemy *goblean);
    Damage l3_collisionPlayer(Player *player, Enemy *gob, Enemy *goblean);
    
    void l3_throwLean(Player *player, Asset *lean);
    void l3_resetAsset(Asset *asset);
    
    void l3_readDamage(Damage dtype, Damage actor, Player *player, Asset *lean, Enemy *gob, Enemy *goblean);
    void l3_readCollisions(Player *player, Asset *lean, Enemy *gob, Enemy *goblean);
    
    void LevelThreeRead(Player *player, Asset *lean, Enemy *gob, Enemy *goblean);
    void LevelThreeInit(Player *player);
    void LevelThreeDraw(Player *player, ScreenFX *screenFx);

#endif
