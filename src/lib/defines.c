
// Conditional definition, avoiding redeclaration warnings/errors (when including in other files)

/*** GAME CONFIG *************************************************************/
#ifndef DEFINES_C_

    #define DEFINES_C_

    #define GAME_NAME "PurpleDrank"
    #define GAME_FPS  60
    
    #define GAME_DEFAULT_LIVES   3
    #define GAME_DEFAULT_LEAN    0
    #define GAME_DEFAULT_TIMEOUT 1
    
    #define SPRITE_FRAME_SPEED   9

    #define SCREEN_WIDTH  800
    #define SCREEN_HEIGHT 450

    #define VELOCITY 0.5f

#endif

/*** PHYSAC FLAGS *************************************************************/

#ifndef PHYSAC_IMPLEMENTATION

    #define PHYSAC_IMPLEMENTATION

#endif

#ifndef PHYSAC_NO_THREADS

    #define PHYSAC_NO_THREADS

#endif
