#ifndef LEVELBONUS_H_

    #define LEVELBONUS_H_

        void LevelBonusDraw(Player *player, ScreenFX *screenFx);
        void LevelBonusInit(Player *player);
        void LevelBonusRead(Player *player);
        //bool lb_collisionBarrel(Player *player);
        bool lb_collisionPortal(Player *player);
        //bool lb_collisionGelano(Player *player);
        //bool lb_collisionLean(Player *player, Asset *lean);
        void lb_readCollisions(Player *player);

#endif
