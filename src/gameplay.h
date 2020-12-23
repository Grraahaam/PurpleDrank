
#ifndef GAMEPLAY_H_
    
    #define GAMEPLAY_H_

    void gp_drawBoard(Player *player);
    void gp_drawNotification();
    void gp_drawPlayerMeta(Player *player);
    void gp_drawText(char* str, Font font, Vector2 position, int fontSize, TextAlign center, Color color);
    
    void gp_resetNotification();
    
    bool gp_isOutScreen(Asset *asset);
    void gp_initializeBody(Player *player);
    void gp_initializeGameBorders();
    void gp_drawBodyLines();
    
    void gp_animateAsset(Asset *asset);
    void gp_drawAsset(Asset *asset, Vector2 position, float scale);
    void gp_drawAssetLines(Asset *asset, Vector2 position, float scale);
    
    void gp_resetPlayer(Player *player);
    void gp_readPlayer(Player *player);
    void gp_editAsset(Asset *asset);
    
    void gp_drawImage(ResImage *img, float scale);
    void gp_drawFade(ScreenFX *screenFx);
    void gp_drawCrossFade(ResImage *imgAbove, ResImage *imgBelow, ScreenFX *screenFx);
    void gp_drawBounceText(char *str, Font font, Vector2 position, TextAlign align, ScreenFX *screenFx);
    void gp_resetFx(ScreenFX *screenFx);
    
    void gp_initResources(Resources *res);
    void gp_initResourcesAssets(Asset *asset, SpritePack sprite, Vector2 framePos, int frameAmount, int frameLines, float width, float height, float scale, int rotation, bool animate, bool loop, Vector2 loopPos, float frameSpeed);
    void gp_initPositions(LevelPosition *levelPos);
    void gp_initFx(ResourcesFX *fx);

#endif
