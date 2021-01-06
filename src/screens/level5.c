#include <stdio.h>
#include <raylib.h>
#include <unistd.h>
#include <pthread.h>

#include "../lib/physac.h"
#include "../lib/defines.c"
#include "../globals.h"
#include "../gameplay.h"

#include "level5.h"

Enemy goblean;
Asset goblean_flip, skate, left_hand, right_hand, lean, damage, lifebar;
Asset leans[2];
int attaque;

Damage l5_collisionLean(Asset *lean, Enemy *goblean, Asset *skate, Asset *left_hand, Asset *right_hand) {
    
    if(gp_collisionAssets(lean, &goblean->asset)) return BOSS;
    
    if(gp_collisionAssets(lean, skate) && !skate->disabled) return SKATE;
    
    if(gp_collisionAssets(lean, left_hand) && !left_hand->disabled) return HAND;
    if(gp_collisionAssets(lean, right_hand) && !right_hand->disabled) return HAND;
    
    return NONE;
}

Damage l5_collisionPlayer(Player *player, Enemy *goblean, Asset *skate, Asset *left_hand, Asset *right_hand) {
        
    if(gp_collisionAssets(&player->asset, &goblean->asset)) {
        
        if(player->super) {
            
            l5_createLean();
            return NONE;
            
        } else return PLAYER;
    }
    
    if(gp_collisionAssets(&player->asset, skate) && !skate->disabled) return PLAYER;
    
    if(gp_collisionAssets(&player->asset, left_hand) && !left_hand->disabled) return PLAYER;
    if(gp_collisionAssets(&player->asset, right_hand) && !right_hand->disabled) return PLAYER;
    
    return NONE;
}

void l5_throwLean(Player *player, Asset *lean) {

    // Only throw if lean left in inventory and no lean in the air
	if(player->lean > 0 && lean->disabled){

        // Decrement the lean amount and start throwing from player's position
        --player->lean;
        lean->position = player->asset.position;
        
        // Set the lean's inital size
        lean->swidth = lean->width * lean->scale;
        lean->sheight = lean->height * lean->scale;

        // Set the throwed lean's direction
        lean->direction = player->asset.direction == RIGHT ? RIGHT : LEFT;
        
        // Enable drawing
        lean->disabled = false;
        
        PlaySound(res.sounds.lean);
    }
}

// Function dropping lean on the floor
void l5_createLean() {

    // Drop 2 leans
    for(int i = 0; i < 2; i++) {
        
        if(leans[i].disabled) {
            
            leans[i].disabled = false;
            leans[i].position = (Vector2){
                goblean.asset.position.x + gp_perX(5) + i * gp_perX(2.5),
                goblean.asset.position.y - gp_perY(18)
            };
        }
    }
}

void l5_destroyLean(Asset *lean) {
    
    // Destroy throwed lean
    lean->disabled = true;
    lean->position = (Vector2){0,0};
}

// Function showing an explosion animation on given asset (+ sound fx)
void l5_playDamage(Asset *ast) {
    
    PlaySound(res.sounds.explosion);
    
    damage.position.x = ast->position.x;
    damage.position.y = ast->position.y;
    damage.frame.current = 0;
    damage.disabled = false;
    damage.frame.animate = true;
}

void l5_readDamage(Damage dtype, Damage actor, Player *player, Asset *lean, Enemy *goblean, Asset *skate, Asset *left_hand, Asset *right_hand) {
    
    // If gob or goblean hit by a throwed lean
    switch(dtype) {
        
        case BOSS: {
            
            // Decrement goblean's lives
            --goblean->lives;
            
            // If lean throwed, destroy it
            if(actor == LEAN) l5_destroyLean(lean);
            
            damage.type = BOSS;
            l5_playDamage(lean);
            
        } break;
        
        case PLAYER: {
            
            // Decrement player's lives
            --player->lives;
            
            // Replace player
            player->body->position = game.levelPos.level_5;
            
            // Disable skate
            skate->disabled = true;
            
            // Disable hands
            left_hand->disabled = true;
            right_hand->disabled = true;
            
        } break;
        
        case SKATE: {
            
            // Disable skate
            skate->disabled = true;
            
            // Destroy lean
            l5_destroyLean(lean);
            
            damage.type = SKATE;
            l5_playDamage(skate);
            
        } break;
        
        case HAND: {
            
            // Disable hands
            left_hand->disabled = true;
            right_hand->disabled = true;
            
            // Destroy lean
            l5_destroyLean(lean);
            
            damage.type = HAND;
            l5_playDamage(left_hand);
            
        } break;
        
       // Default action
       default : break;
    }
}

void l5_readCollisions(Player *player, Asset *lean, Enemy *goblean, Asset *skate, Asset *left_hand, Asset *right_hand) {
    
    // Read and process collision damage type
    l5_readDamage(
        l5_collisionLean(lean, goblean, skate, left_hand, right_hand),
        LEAN, player, lean, goblean, skate, left_hand, right_hand
    );
    
    l5_readDamage(
        l5_collisionPlayer(player, goblean, skate, left_hand, right_hand),
        PLAYER, player, lean, goblean, skate, left_hand, right_hand
    );
    
    for(int i = 0; i < 2; i++) {
        
        if(gp_collisionAssets(&player->asset, &leans[i]) && !leans[i].disabled) {
            
            leans[i].disabled = true;
            ++player->lean;
        }
    }
}

void LevelFiveRead(Player *player, Enemy *goblean, Asset *lean, Asset *skate, Asset *left_hand, Asset *right_hand, Asset *lifebar) {
    
    gp_readPlayer(player);
    l5_readCollisions(player, lean, goblean, skate, left_hand, right_hand);
    
    // If ended level
    if(goblean->lives <= 0) {
        
        goblean->asset.disabled = true;
        
        game.levelPassed = LEVEL_5;
        game.gameScreen = VICTORY;
    
    } else {
    
        // Throw lean
        if(IsKeyPressed(KEY_Q)) l5_throwLean(player, lean);
        
        // Update the lifebar
        lifebar->frame.x = lifebar->frame.xinit + lifebar->width * (6 - goblean->lives);
    
        if(goblean->asset.direction == LEFT) {
                    
            goblean->asset.position.x -= goblean->asset.speed;
            
            // If goblean reached the middle and is facing left
            if(goblean->asset.position.x < (GetScreenWidth() / 2.5 + goblean->asset.swidth)) {
                
                // Invert direction
                goblean->asset.direction = RIGHT;
                                
                switch(attaque) {
                    
                    case 1: {
                        skate->position.x = goblean->asset.position.x;
                        skate->disabled = false;
                        
                        goblean_flip.disabled = false;
                        goblean->asset.disabled = true;
                    } break;
                    
                    case 2: {
                        left_hand->disabled = false;
                        left_hand->position = (Vector2){
                            .x = goblean->asset.position.x + gp_perX(7.5),
                            .y = goblean->asset.position.y - gp_perY(4.4)
                        };
                        
                        right_hand->disabled = false;
                        right_hand->position = (Vector2){
                            .x = goblean->asset.position.x + gp_perX(2.5),
                            .y = goblean->asset.position.y - gp_perY(4.4)
                        };
                    } break;
                    
                    default: break;
                }
            }
            
        } else {
                
            goblean->asset.position.x += goblean->asset.speed * 3;
            
            // If goblean reached the right side and is facing right (right - goblean width)
            if(goblean->asset.position.x > (GetScreenWidth() - goblean->asset.swidth)) {
                
                // Invert direction
                goblean->asset.direction = LEFT;
            
                switch(attaque) {
                
                    case 1: {
                        goblean_flip.disabled = true;
                        goblean->asset.disabled = false;
                        attaque = 2;
                    } break;
                    
                    case 2: {
                        attaque = 1;
                    } break;
                    
                    default: break;
                }
            }
        }
        
        // Handle damage, clean it when done and follow boss when moving (explosion animation)
        if(damage.frame.current >= 9) damage.disabled = true;
        if(!damage.disabled && damage.type == BOSS) {
            
            damage.position.x = goblean->asset.position.x;
            damage.position.y = goblean->asset.position.y;
        }
        
        // Increment dropped leans if enabled
        for(int i = 0; i < 2; i++) {
        
            if(!leans[i].disabled) {
                if(leans[i].position.y < gp_perY(80)) {
                    leans[i].position.x -= gp_perX(1.25);
                    leans[i].position.y += gp_perY(1.1);
                }
            }
        }
        
        // SKATE movements
        if(!skate->disabled) {
            
            //TODO: if facing RIGHT/LEFT +/- speed
            skate->position.x -= gp_perX(.5);
        }
        
        if(gp_isOutScreen(skate) && !skate->disabled) {
            
            PrintDebug("Skate destroyed");
            skate->disabled = true;
        }
        
        // HANDS movements
        if(!left_hand->disabled) {
            
            //TODO: if facing RIGHT/LEFT +/- speed
            left_hand->position.x -= gp_perX(.5);
            left_hand->position.y += gp_perY(.2);
        }
        
        if(gp_isOutScreen(left_hand) && !left_hand->disabled) {
            
            PrintDebug("Left hand destroyed");
            left_hand->disabled = true;
        }
        
        if(!right_hand->disabled) {
            
            //TODO: if facing RIGHT/LEFT +/- speed
            right_hand->position.x -= gp_perX(.5);
            right_hand->position.y += gp_perY(.2);
        }
        
        if(gp_isOutScreen(right_hand) && !right_hand->disabled) {
            
            PrintDebug("Right hand destroyed");
            right_hand->disabled = true;
        }
        
        // If there's a lean being throwed
        if(!lean->disabled) {
            
            if(lean->direction == RIGHT) lean->position.x += lean->speed;
            else lean->position.x -= lean->speed;
        }
        
        // Destroy lean if outside the screen
        if(gp_isOutScreen(lean) && !lean->disabled) {
        
            PrintDebug("Lean destroyed");
            l5_destroyLean(lean);
        }
	}
}
    
void LevelFiveInit(Player *player) {
    
    ResetPhysics();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    PrintDebug(TextFormat("Initializing: %s", screenNames[game.gameScreen]));
    
    gp_resetNotification();
    
    gp_resetPlayer(player);
    
    /** CUSTOM ****************************************************************************/
    
    // Edit gravity to jump over the HANDS (not enough to avoid SKATE to force player to use lean to destroy it)
    SetPhysicsGravity(0.0f, 6.0f);
    
    // Left wall
    gp_createPhyRec((Vector2){
        .x = gp_perX(-1),
        .y = gp_perY(0)
    }, gp_perX(0.5), gp_perY(100));
    
    // Right wall
    gp_createPhyRec((Vector2){
        .x = GetScreenWidth() + gp_perX(1),
        .y = 0
    }, gp_perX(0.5), gp_perY(100));
    
    // Main floor
    gp_createPhyRec((Vector2){
        .x = 0,
        .y = gp_perY(83)
    }, gp_perX(100), gp_perY(10));
    
    // Initialize Lean object (to throw)
    lean = res.items.lean;
    lean.scale = gp_perX(1.5) / res.items.lean.width;
    lean.speed = gp_perX(.5);
    lean.position = (Vector2){0,0};
    lean.disabled = true;
    
    // Initialize Goblean asset
    goblean.asset = res.items.goblean_skate;
    goblean.asset.speed = gp_perX(.2);
    goblean.asset.direction = LEFT;
    goblean.lives = 6;
    goblean.asset.disabled = false;
    
    goblean.asset.position = (Vector2){
        .x = GetScreenWidth() + goblean.asset.swidth,
        .y = gp_perY(66)
    };
    
    goblean_flip = res.items.goblean_flip;
    goblean_flip.disabled = true;
    
    skate = res.items.skate_flip;
    skate.disabled = true;
    skate.position = (Vector2){
        .x = goblean.asset.position.x,
        .y = goblean.asset.position.y + gp_perY(11)
    };
    
    left_hand = res.items.left_hand;
    left_hand.disabled = true;
    
    right_hand = res.items.right_hand;
    right_hand.disabled = true;
    
    damage = res.items.damage;
    damage.scale = gp_perX(10) / res.items.damage.width;
    damage.disabled = true;
    
    lifebar = res.items.lifebar;
    lifebar.scale = gp_perX(15) / res.items.lifebar.width;
    lifebar.position = (Vector2){
        .x = GetScreenWidth() / 2,
        .y = gp_perY(20)
    };
    
    for(int i = 0; i < 2; i++) {
        
        leans[i] = res.items.lean;
        leans[i].scale = gp_perX(1.5) / res.items.lean.width;
        leans[i].disabled = true;
    }
    
    PlaySound(res.sounds.goblean);   
    attaque = 1;
    
    /************************************************************************************/
    
    // Initialize player's body and default position
    gp_initializeBody(player, game.levelPos.level_5);
}

void LevelFiveDraw(Player *player, ScreenFX *screenFx) {

     // Initialize the level only if not loaded
    if(game.screenLoaded != LEVEL_5) {
        
        LevelFiveInit(player);
        PrintDebug(TextFormat("Drawing: %s", screenNames[game.gameScreen]));
        
        game.screenLoaded = LEVEL_5;
        game.levelPassed = LEVEL_4;
        
        // Set default fade properties
        gp_resetFx(screenFx);
        
        res.items.portal.disabled = false;
    }
    
    // Read user input and interact
    LevelFiveRead(player, &goblean, &lean, &skate, &left_hand, &right_hand, &lifebar);
    
    RunPhysicsStep();
    BeginDrawing();

    ClearBackground(BLACK);

    // Draw level's background
    gp_drawImage(&res.backgrounds.level5, res.backgrounds.level5.scale);
    
    /** CUSTOM ****************************************************************************/
    
    // Draw game assets
    gp_drawAsset(&goblean.asset, goblean.asset.position, goblean.asset.scale);
    gp_drawAsset(&skate, skate.position, skate.scale);
    
    gp_drawAsset(&right_hand, right_hand.position, right_hand.scale);
    gp_drawAsset(&left_hand, left_hand.position, left_hand.scale);
    
    gp_drawAsset(&goblean_flip, goblean.asset.position, goblean_flip.scale);
    
    gp_drawAsset(&lean, lean.position, lean.scale);
    
    gp_drawAsset(&damage, damage.position, damage.scale);
    
    gp_drawAsset(&lifebar, lifebar.position, lifebar.scale);
    
    // Draw the dropped leans (when active)
    for(int i = 0; i < 2; i++) {
        
        gp_drawAsset(&leans[i], leans[i].position, leans[i].scale);
    }

    /**************************************************************************************/
    
    // Player's info board (lifes, lean, level) and notification
    gp_drawBoard(player);
    
    // Draw player
    gp_drawAsset(&player->asset, player->asset.position, player->asset.scale);
    
    // Drawing the fade in effect
    gp_drawFade(screenFx);
    
    EndDrawing();
}
