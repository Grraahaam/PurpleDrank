
#ifndef LEVELBONUS_H_

    #define LEVELBONUS_H_

        void LevelBonusDraw(Player *player, ScreenFX *screenFx);
        void LevelBonusInit(Player *player);
        void LevelBonusRead(Player *player, Asset *portal, Asset *gelano, Asset *portal_bonus, Asset *extraLife);
        void lb_readCollisions(Player *player, Asset *portal, Asset *gelano, Asset *portal_bonus, Asset *extraLife);

#endif
