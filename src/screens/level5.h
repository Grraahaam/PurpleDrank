#ifndef LEVEL5_H_

    #define LEVEL5_H_
    
    bool l5_collisionAssets(Asset *ast1, Asset *ast2);
    Damage l5_collisionLean(Asset *lean, Enemy *goblin);
    Damage l5_collisionPlayer(Player *player, Enemy *goblin, Asset *skate, Asset *left_hand, Asset *right_hand);
    void l5_throwLean(Player *player, Asset *lean);
    void l5_destroyLean(Asset *lean);
    void l5_readDamage(Damage dtype, Damage actor, Player *player, Asset *lean, Enemy *goblean);
    void l5_readCollisions(Player *player, Asset *lean, Enemy *goblin, Asset *skate, Asset *left_hand, Asset *right_hand);
    
    
    
    void LevelFiveRead(Player *player, Enemy *goblin, Asset *lean, Asset *skate, Asset *left_hand, Asset *right_hand);
    void LevelFiveInit(Player *player);
    void LevelFiveDraw(Player *player, ScreenFX *screenFx);

#endif
