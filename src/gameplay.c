
#include <raylib.h>

#include "lib/defines.c"
#include "lib/easings.h"
#include "globals.h"
#include "gameplay.h"


/******* DRAWING ********/

// Drawing player's info board
void gp_drawBoard(Player *player) {
    
    int fontSize = gp_perX(1.8), font_align_x = gp_perX(9);
    float scale_head = gp_perX(4.5) / res.items.player_head.width;
    float scale_lean = gp_perX(1.5) / res.items.lean.width;
    int margin = 3;
    
    // Draw notifications, if exists
    gp_drawNotification();
    
    // Lives amount
    gp_drawAsset(&res.items.player_head,
        (Vector2){
            res.items.player_head.width * scale_head / 2 + gp_perX(margin),
            res.items.player_head.height * scale_head / 2 + gp_perY(margin),
        }, scale_head);
    
    DrawText(
        TextFormat("%d", player->lives),
        font_align_x,
        gp_perY(margin) + fontSize / 2 + gp_perY(.1), // Y axis align
        fontSize,
        RAYWHITE
    );
    
    // Lean amount
    gp_drawAsset(&res.items.lean,
        (Vector2){
            res.items.lean.width * scale_lean / 2 + gp_perX(margin) + gp_perX(1.6),
            res.items.lean.height * scale_lean / 2 + res.items.player_head.height * scale_head + gp_perY(margin) + gp_perY(1.6),
        }, scale_lean);
    
    DrawText(
        TextFormat("%d", player->lean),
        font_align_x,
        fontSize * 2 + gp_perY(margin) + gp_perY(.8), // Y axis align
        fontSize,
        RAYWHITE
    );
    
    // Draw titla bigger size (only for the bonus stage)
    if(game.gameScreen == LEVEL_BONUS) {
    
        gp_drawText(
            (char*)TextFormat("%s", "BONUS STAGE"), res.fonts.pixellari,
            (Vector2){0, fontSize + gp_perY(2)},
            gp_perX(6), CENTER_X, RAYWHITE
        );
        
    // Draw title original size
    } else {
        
        gp_drawText(
            (char*)TextFormat("%s", screenNames[game.gameScreen]), res.fonts.pixellari,
            (Vector2){0, fontSize + gp_perY(2)},
            fontSize, CENTER_X, RAYWHITE
        );
    }
        
    // FPS (auto-corner text)
    DrawText(
        TextFormat("%i FPS", GetFPS()),
        GetScreenWidth() - MeasureText("XX FPS", fontSize - 5) - 10,
        GetScreenHeight() - (fontSize - 5) - 10,
        fontSize - 5,
        LIGHTGRAY
    );
    
    if(DEBUG) {
     
        gp_drawPlayerMeta(player);
        gp_drawBodyLines();
    }
}

// Function drawing a notification message on the screen (if exists)
void gp_drawNotification() {
    
    int fontSize = gp_perX(1.5);
    
    // If a notification message exists
    if(game.notification.message) {
        
        // DrawText(text, x, y, size, color)
        // Draw the notification message on the screen (auto center)
        DrawText(
            TextFormat("%s", game.notification.message),
            GetScreenWidth() / 2 - MeasureText(game.notification.message, fontSize) / 2,
            fontSize * 2 + 20,
            fontSize,
            game.notification.color
        );
    }
}

// Function drawing player's body coordinates
void gp_drawPlayerMeta(Player *player) {
    
    // Draw both X and Y coordinates
    DrawText(TextFormat("Y: %.2f", player->asset.position.y), 10, GetScreenHeight() - 25, 15, RED);
    DrawText(TextFormat("X: %.2f", player->asset.position.x), 10, GetScreenHeight() - 45, 15, WHITE);
    
    // Draw player's size, speed, direction and super mode
    DrawText(TextFormat("Frame speed: %i", player->asset.frame.speed), 10, GetScreenHeight() - 65, 15, RAYWHITE);
    DrawText(TextFormat("Speed: %.2f", player->asset.speed), 10, GetScreenHeight() - 85, 15, RAYWHITE);
    DrawText(TextFormat("Scale: %.2f", player->asset.scale), 10, GetScreenHeight() - 105, 15, RAYWHITE);
    DrawText(TextFormat("Direction: %s", player->asset.direction == RIGHT ? "RIGHT" : "LEFT"), 10, GetScreenHeight() - 125, 15, RAYWHITE);
    DrawText(TextFormat("Super: %s", player->super ? "TRUE" : "FALSE"), 10, GetScreenHeight() - 145, 15, RAYWHITE);
}

// Function drawing text with additional settings (font and auto- center)
void gp_drawText(char* str, Font font, Vector2 position, int fontSize, TextAlign align, Color color) {
    
    // If text is provided
    if(str) {
        
        // Get the string dimensions (default/custom font)
        Vector2 textSize = (font.charsCount != 0) ?
            MeasureTextEx(font, str, fontSize, 0) : // font, str, size, spacing
            (Vector2){MeasureText(str, fontSize), fontSize};
            
        // Center the string on given absis
        switch(align) {
            
            case CENTER_X: {
                position.x = GetScreenWidth() / 2 - textSize.x / 2;
            } break;
            
            case CENTER_Y: {
                position.y = GetScreenHeight() / 2 - textSize.y / 2;
            } break;
            
            case CENTER: {
                
                position.x = GetScreenWidth() / 2 - textSize.x / 2;
                position.y = GetScreenHeight() / 2 - textSize.y / 2;
            } break;
            
            default: break;
        }
        
        // If custom font given
        if(font.charsCount != 0) {
            
            DrawTextEx(
                font, str,
                position, fontSize,
                0, color // spacing, color
            );
            
        } else {
            // Draw the notification message on the screen (auto center)
            DrawText(
                str,
                position.x, position.y,
                fontSize, color
            );
        }
    }
}

// Function drawing physic bodies border lines
void gp_drawBodyLines() {
    
    // Draw created physics bodies
    int bodiesCount = GetPhysicsBodiesCount();
    for (int i = 0; i < bodiesCount; i++)
    {
        PhysicsBody body = GetPhysicsBody(i);

        int vertexCount = GetPhysicsShapeVerticesCount(i);
        for (int j = 0; j < vertexCount; j++)
        {
            // Get physics bodies shape vertices to draw lines
            // Note: GetPhysicsShapeVertex() already calculates rotation transformations
            Vector2 vertexA = GetPhysicsShapeVertex(body, j);

            // Get next vertex or first to close the shape
            int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);
            Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

            // Draw a line between two vertex positions
            DrawLineV(vertexA, vertexB, GREEN);
        }
    }
}

// Function drawing asset according to their properties
void gp_drawAsset(Asset *asset, Vector2 position, float scale) {
    
    Texture2D sprite;
    switch(asset->sprite) {
        
        case SP_PLAYER: {
            sprite = res.sprites.player;
        } break;
        
        case SP_ASSETS: {
            sprite = res.sprites.assets;
        } break;
        
        case SP_ANIMATION: {
            sprite = res.sprites.animations;
        } break;
        
        case SP_SCREENS: {
            sprite = res.sprites.screens;
        } break;
        
        default: {
            sprite = res.sprites.assets;
        } break;
    }
    
    // Draw only if the asset is enabled
    if(!asset->disabled) {
        
        // All except player asset (own animation process - gp_readPlayer())
        if(asset->frame.animate) {
            
            gp_animateAsset(asset);
        }
        
        DrawTexturePro(sprite,
            // Select corresponding frame from original spritecheet
            (Rectangle){
                .x = asset->frame.x, .y = asset->frame.y,
                .width = asset->width, .height = asset->height
            },
            // Draw its scaled version at the matching position
            (Rectangle){
                .x = (position.x - (asset->width * scale) / 2),
                .y = (position.y - (asset->height * scale) / 2),
                .width = asset->width * scale,
                .height = asset->height * scale
            },
            (Vector2){0,0}, asset->rotation, WHITE
        );
        
        // ISSUE: WHEN ASSET HAVE AN INITIALIZED BODY, IT NEEDS TO BE REGENERATED TO MATCH THE NEW DIMENSIONS
        if(DEBUG) {
        
            gp_drawAssetLines(asset, position, scale);
            
            if(game.gameScreen == LEVEL_1 || game.gameScreen == LEVEL_2 || game.gameScreen == LEVEL_3 || game.gameScreen == LEVEL_4 || game.gameScreen == LEVEL_5 || game.gameScreen == LEVEL_BONUS) {
            
                gp_drawText(
                    "In/Decrease size: + -", res.fonts.raylib,
                    (Vector2){0, GetScreenHeight() - 20},
                    15, CENTER_X, LIGHTGRAY
                );
                
                gp_drawText(
                    "In/Decrease speed: * /", res.fonts.raylib,
                    (Vector2){0, GetScreenHeight() - 40},
                    15, CENTER_X, LIGHTGRAY
                );
            }
        }
    }
}

// Function drawing asset's X and Y position lines and the hitbox (centered)
void gp_drawAssetLines(Asset *asset, Vector2 position, float scale) {
    
    // Draw position lines only for the player
    if(asset->sprite == SP_PLAYER) {
        
        // Draw X axis
        DrawLine(
            (int)(position.x), 0,
            (int)(position.x), GetScreenHeight(),
        LIGHTGRAY);
        
        // Draw Y axis
        DrawLine(
            0, (int)(position.y),
            GetScreenWidth(), (int)(position.y),
        LIGHTGRAY);
    }
    
    // Draw the hitbox
    DrawRectangleLinesEx(
        (Rectangle){
        .x = (position.x - (asset->width * scale) / 2),
        .y = (position.y - (asset->height * scale) / 2),
        .width = (asset->width * scale), .height = (asset->height * scale)
    }, 1, RED);
}

//Function drawing a screen image and auto-center it (on X,Y)
void gp_drawImage(ResImage *img, float scale) {
    
    // Screen spritecheet process NOT WORKING YET
    /*Texture2D sprite;
    switch(img->sprite) {
        
        case SP_PLAYER: {
            sprite = res.sprites.player;
        } break;
        
        case SP_ASSETS: {
            sprite = res.sprites.assets;
        } break;
        
        case SP_ANIMATION: {
            sprite = res.sprites.animations;
        } break;
        
        case SP_SCREENS: {
            sprite = res.sprites.screens;
        } break;
        
        default: {
            sprite = res.sprites.assets;
        } break;
    }
    
    PrintDebug(TextFormat("Frame X: %i", (int)img->frame.x));
    PrintDebug(TextFormat("Frame Y: %i", (int)img->frame.y));
    
    PrintDebug(TextFormat("Screen width: %i", img->width));
    PrintDebug(TextFormat("Screen height: %i", img->height));
    
    PrintDebug(TextFormat("Screen scale: %.2f", img->scale));
    
    PrintDebug(TextFormat("ScreenWidth: %i", GetScreenWidth()));
    PrintDebug(TextFormat("Delta: %.2f", GetScreenWidth() - img->width * scale));
    PrintDebug(TextFormat("Delta split: %i", (int)(GetScreenWidth() - img->width * scale) / 2));
    
    DrawTexturePro(sprite,
        // Select corresponding frame from original spritecheet
        (Rectangle){
            .x = (int)img->frame.x, .y = (int)img->frame.y,
            .width = img->width, .height = img->height
        },
        // Draw its scaled version at the matching position
        (Rectangle){
            .x = (GetScreenWidth() - img->width * scale) / 2,
            .y = (GetScreenHeight() - img->height * scale) / 2,
            .width = img->width * scale,
            .height = img->height * scale
        },
        (Vector2){0,0}, 0, WHITE
    );*/
    
    // Autoscale
    if(!img->custom_scale) {
        
        // Auto fit width
        scale = (float)GetScreenWidth() / (float)img->screen.width;

        // Auto fit height
        scale = GetScreenHeight() - img->screen.height * scale >= 0 ?
            (float)GetScreenHeight() / (float)img->screen.height :
            scale;
    }
        
    DrawTextureEx(
        img->screen,
        (Vector2){
            (int)(GetScreenWidth() - img->screen.width * scale) / 2,
            (int)(GetScreenHeight() - img->screen.height * scale) / 2
        },
        0, scale, WHITE
    );
}

/******* UTILS ********/

// Reset game notification and reset default color
void gp_resetNotification() {
    
    game.notification.message = "";
    game.notification.color = WHITE;
}

// Function returning a fixed physic body (rectangle)
void gp_createPhyRec(Vector2 pos, float width, float height) {
    
    PhysicsBody body = CreatePhysicsBodyRectangle(
        (Vector2){
            .x = pos.x + width / 2,
            .y = pos.y + height / 2
        }, width, height, 0
    ); body->enabled = false;
    
    //return body;
}

// Function returning a fixed physic body (polygon)
void gp_createPhyPoly(Vector2 pos, float radius, int sides) {
    
    PhysicsBody body = CreatePhysicsBodyPolygon(
        (Vector2){
            .x = pos.x + radius / 2,
            .y = pos.y + radius / 2
        }, radius, sides, 0
    ); body->enabled = false;
    
    //return body;
}

// Function defining physic bodies around the game screen with some margin
void gp_initializeGameBorders() {
    
    /*PhysicsBody border_bottom = CreatePhysicsBodyRectangle(
        (Vector2){
            GetScreenWidth()/2 + 50,
            GetScreenHeight() + 50
        },
        GetScreenWidth() + 100, 10, 10
    ); border_bottom->enabled = false;
     
    
    PhysicsBody border_left = CreatePhysicsBodyRectangle(
        (Vector2){
            GetScreenWidth()/2,
            290
        },
        GetScreenWidth(), 10, 10
    ); border_left->enabled = false;
     
    PhysicsBody border_right = CreatePhysicsBodyRectangle(
        (Vector2){
            GetScreenWidth()/2,
            290
        },
        GetScreenWidth(), 10, 10
    ); border_right->enabled = false;*/
}

/******* COLLISIONS ********/

// Check if given assets are outside the screen (to stop/reset their process)
bool gp_isOutScreen(Asset *ast) {
    
    //for(int i = 0; i < n; i++) {
        
        // Check if position is outside the initial screen dimensions + their own width/height + a X,Y margin
    
        // Define current dimensions
        int swidth = ast->width * ast->scale;
        int sheight = ast->height * ast->scale;
    
        if(ast->position.x < 0 - swidth - gp_perX(30) ||
            ast->position.x > GetScreenWidth() + swidth + gp_perX(30) ||
            ast->position.y < 0 - sheight - gp_perY(30) ||
            ast->position.y > GetScreenHeight() + sheight + gp_perY(30)
        
        //) return ast->type;
        ) return true;
    //}
    
    //return NONE;
    else return false;
}

// Return the collision status between two assets (auto-adapt according to asset's rotation)
bool gp_collisionAssets(Asset *ast1, Asset *ast2) {
    
    int ast1_width, ast1_height, ast2_width, ast2_height;
    
    //TODO: Support for negative rotation value
    
    // Switch width and height according to the assets' rotation
    if((ast1->rotation >= 45 && ast1->rotation < 134) ||
        (ast1->rotation >= 225 && ast1->rotation < 315)) {
     
        ast1_width = ast1->height;
        ast1_height = ast1->width;
        
    } else {
        
        ast1_width = ast1->width;
        ast1_height = ast1->height;
    }
    
    if((ast2->rotation >= 45 && ast2->rotation < 134) ||
        (ast2->rotation >= 225 && ast2->rotation < 315)) {
     
        ast2_width = ast2->height;
        ast2_height = ast2->width;
        
    } else {
        
        ast2_width = ast2->width;
        ast2_height = ast2->height;
    }
    
    // Check the collision
    return CheckCollisionRecs(
        // Asset 1
        (Rectangle){
            .x = ast1->position.x - (ast1->scale * ast1_width / 2),
            .y = ast1->position.y - (ast1->scale * ast1_height / 2),
            .width = ast1->scale * ast1_width, .height = ast1->scale * ast1_height
        },
        // Asset 2
        (Rectangle){
            .x = ast2->position.x - (ast2->scale * ast2_width / 2),
            .y = ast2->position.y - (ast2->scale * ast2_height / 2),
            .width = ast2->scale * ast2_width, .height = ast2->scale * ast2_height
        }
    );
}

/******* PLAYER ********/

// Function reseting the player's default properties
void gp_initPlayer(Player *player) {
    
    player->asset.frame.loop    = true;
    player->asset.frame.pack    = FORWARD;
    player->asset.frame.speed   = SPRITE_FRAME_SPEED;
    player->asset.frame.amount  = 9;
    player->asset.frame.lines   = 7;
    player->asset.frame.counter = 0;
    player->asset.frame.current = 0;
    player->asset.frame.x       = 0;
    player->asset.frame.xinit   = 0;
    player->asset.frame.y       = 0;
    player->asset.frame.yinit   = 0;
    player->asset.frame.loopPos.x = 0;
    
    player->asset.version_count = 3;
    
    player->asset.position  = game.levelPos.level_1;
    player->asset.sprite    = SP_PLAYER;
    player->asset.direction = RIGHT;
    player->asset.speed     = VELOCITY * gp_perX(.05);
    player->asset.density   = 10;
    player->asset.rotation  = 0;
    player->asset.width     = res.sprites.player.width / player->asset.frame.amount;
    player->asset.height    = (res.sprites.player.height / player->asset.version_count) / player->asset.frame.lines;
    player->asset.scale     = gp_perX(5) / player->asset.width; //1.4
    player->asset.swidth    = (float)player->asset.width * player->asset.scale;
    player->asset.sheight   = (float)player->asset.height * player->asset.scale;
    
    player->body            = NULL;
    
    player->lives           = GAME_DEFAULT_LIVES;
    player->lean            = GAME_DEFAULT_LEAN;
    player->dead            = false;
    player->super           = false;
    player->can_move        = true;
    player->gelano          = false;
    player->portal          = false;
    player->extraLife	     = false;
    player->jetLean         = false;
    player->slip            = false;
    player->portalPowerUsed = false;
    player->powerTimes      = 0;
    player->bonus_level_visited = false;
    
    // Set gravity force Default is { 0.0f, 9.81f }
    SetPhysicsGravity(0.0f, 6.8f);
}

void gp_resetPlayer(Player *player) {
    
    player->asset.speed     = VELOCITY * gp_perX(.05);
    player->asset.density   = 10;
    player->asset.rotation  = 0;
    player->dead            = false;
    player->can_move        = true;
    player->slip            = false;
    
    SetPhysicsGravity(0.0f, 6.8f);
}

// Read player movement inputs and control its interactions
void gp_readPlayer(Player *player) {

    
    // Check events when player can move
    if(player->can_move) {
        
        player->asset.frame.speed = SPRITE_FRAME_SPEED;
        
        // Apply ice simulation
        if(player->slip) {
            
            if(player->asset.direction == LEFT)
                player->body->position.x -= gp_perX(0.1);
            
            if(player->asset.direction == RIGHT)
                player->body->position.x += gp_perX(0.1);
        }
        
        // Moving on Y axis, when player jump, and its physic body is grounded (placed at the top for improved input detection)
        if(IsKeyDown(KEY_UP) && player->body->isGrounded) {
        
            PlaySound(res.sounds.jump);
            
            // Invert Y axis velocity to simulate a jump
            if(game.gameScreen == LEVEL_BONUS || game.gameScreen == LEVEL_5)
                player->body->velocity.y = -1 * gp_perY(.24);
            else
                player->body->velocity.y = -1 * gp_perY(.2);
        }
        
        // If moving on X axis
        if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) {
            
            if(IsKeyDown(KEY_RIGHT)) {
                
                player->asset.direction = RIGHT;
                
                if(IsKeyDown(KEY_R)) {
            
                    player->asset.frame.pack = SUPER;
                    player->body->velocity.x = player->slip ?
                    // Adapt if player's is slipping
                    player->asset.speed * 2 : player->asset.speed * 2;
                    player->super = true;
                
                } else {
                
                    // Set player's speed and select the corresponding sprite cheet pack
                    player->body->velocity.x = player->slip ?
                        // Adapt if player's is slipping
                        player->asset.speed : player->asset.speed;
                    player->asset.frame.pack = FORWARD;
                    player->super = false;
                }
            
            } else if (IsKeyDown(KEY_LEFT)) {
                
                player->asset.direction = LEFT;
                
                if (IsKeyDown(KEY_R)) {
                
                    player->asset.frame.pack = SUPER;
                    player->body->velocity.x = player->slip ?
                        // Adapt if player's is slipping
                        -player->asset.speed * 2 : -player->asset.speed * 2;
                    player->super = true;
                
                } else {
                    
                    // Set player's speed and select the corresponding sprite cheet pack
                    player->body->velocity.x = player->slip ?
                        // Adapt if player's is slipping
                        -player->asset.speed * 1 : -player->asset.speed;
                    player->asset.frame.pack = BACKWARD;
                    player->super = false;
                }
            }
        
        // If player's not moving, idle
        } else {
            
            player->asset.frame.speed = 1;
            
            if(player->asset.direction == RIGHT) player->asset.frame.pack = FORWARD_IDLE;
            else player->asset.frame.pack = BACKWARD_IDLE;
        }
        
        
        // Duplicate player body position into its asset position (drawing purpose)
        player->asset.position = player->body->position;
        
        player->asset.frame.line = player->asset.frame.pack;
        player->asset.frame.loopPos.y = player->asset.frame.line;
        player->asset.frame.loop = true;
        
        gp_animateAsset(&player->asset);
    }
    
    // Check if player died (but still lives left)
    if(player->dead) {
        
        PrintDebug("Player is dead");
        
        // Set corresponding sprite cheet line
        player->asset.frame.pack = SPECIAL;
        
        // Set player in dead mode (for now only not moving, facing last direction)
        player->asset.frame.line = player->asset.frame.pack;
        player->asset.frame.loopPos.y = player->asset.frame.line;
        
        // Reset to the first frame/sprite
        player->asset.frame.loop = false;
        player->asset.frame.current = 0;
        player->asset.frame.line_frame = 0;
        
        gp_animateAsset(&player->asset);
    }
    
    // If player have no lives left
    if(player->lives <= 0) {
        
        PrintDebug("Player's dead => Gameover");
        
        game.gameScreen = GAMEOVER;
    }
    
    // Detect if player is outside the screen, and reset its position
    if(gp_isOutScreen(&player->asset)) {
        
        // Decrement one live
        --player->lives;
    
        // Detect which screen
        switch(game.gameScreen) {
            
            case LEVEL_1: {
                player->body->position = game.levelPos.level_1;
            } break;
            
            case LEVEL_2: {
                player->body->position = game.levelPos.level_2;
            } break;
            
            case LEVEL_3: {
                player->body->position = game.levelPos.level_3;
            } break;
            
            case LEVEL_4: {
                player->body->position = game.levelPos.level_4;
            } break;
            
            case LEVEL_5: {
                player->body->position = game.levelPos.level_5;
            } break;
            
            case LEVEL_BONUS: {
                player->body->position = game.levelPos.level_bonus;
            } break;
            
            default: break;
        }
    }
    
    // Edit player's asset in-game properties
    if(DEBUG) gp_editAsset(&player->asset);
}

// Function initializing an asset body
void gp_initializeBody(Player *player, Vector2 pos) {
    
    // Initialize player's body
    player->body = CreatePhysicsBodyRectangle(
        pos,
        player->asset.swidth,
        player->asset.sheight,
        player->asset.density
    );
    
    // Constrain body rotation to avoid little collision torque amounts
    player->body->freezeOrient = true;
}

/******* ASSET CONTROL ********/

// Animate an asset with its matching sprites
void gp_animateAsset(Asset *asset) {
    
    Texture2D sprite;
    switch(asset->sprite) {
        
        case SP_PLAYER: {
            sprite = res.sprites.player;
        } break;
        
        case SP_ASSETS: {
            sprite = res.sprites.assets;
        } break;
        
        case SP_ANIMATION: {
            sprite = res.sprites.animations;
        } break;
        
        default: {
            sprite = res.sprites.assets;
        } break;
    }
    
    // Incrementing the frame counter to display the sprite from the current pack
    ++asset->frame.counter;

    // Show <frame.speed> frame (sprite) per second
    if (asset->frame.counter >= (GAME_FPS / asset->frame.speed)) {
        
        // If reached maximum frame per second, reset counter and increment sprite frame position
        asset->frame.counter = 0;
        ++asset->frame.current;
        
        // Handle animations only
        if(asset->sprite == SP_ANIMATION) {
            
            ++asset->frame.line_frame;
            
            // Automatically move to the next line if reached the end of the current one (if animation is on more than one line, except for player spritecheet which is handled in gp_readPlayer())
            if((asset->width * asset->frame.line_frame) / sprite.width >= 1 ||
                sprite.width - (asset->width * asset->frame.line_frame) < asset->width
            ) {
            
                asset->frame.line_frame = 0;
                
                if(asset->frame.line < asset->frame.lines)
                    ++asset->frame.line;
            }
        }

        // If reached the end of the sprite cheet line, reset to the first
        if (asset->frame.current > asset->frame.amount - 1) {
            
            // If the spritecheet should loop
            if(asset->frame.loop) {
                
                asset->frame.current = asset->frame.loopPos.x;
                asset->frame.line_frame = asset->frame.current;
                
                asset->frame.line = asset->frame.loopPos.y;
            
            // Stop the animation
            } else {
                
                asset->frame.animate = false;
                --asset->frame.current;
            }
        }
        
        // Assign new frame position
        asset->frame.x = (float)asset->frame.current * asset->width + asset->frame.xinit;
        
        asset->frame.y = (float)asset->frame.line * asset->height;
        asset->frame.y += (float)(asset->frame.lines * asset->version) * asset->height;
        asset->frame.y += asset->frame.yinit;
    }
}

// Debugging function to live edit assets properties (support speed and scale)
void gp_editAsset(Asset *asset) {
    
    // Edit asset's size
    if(IsKeyPressed(KEY_KP_ADD) || IsKeyPressed(KEY_KP_SUBTRACT)) {
        
        if(IsKeyPressed(KEY_KP_ADD)) asset->scale += 0.05;
        else asset->scale -= 0.05;
        
        asset->swidth = asset->width * asset->scale;
        asset->sheight = asset->height * asset->scale;
    
    // Edit asset's speed
    } else if(IsKeyPressed(KEY_KP_MULTIPLY)) asset->speed += VELOCITY * 0.1;
    else if(IsKeyPressed(KEY_KP_DIVIDE)) asset->speed -= VELOCITY * 0.1;
}

/******* DIMENSION CONTROL ********/

// Function returning a scaling rate to fit the given image to the screen
float gp_autoScaleImg(ResImage *img) {
    
    float scale;
    
    // Auto fit width
    scale = (float)GetScreenWidth() / (float)img->screen.width;
        
    // Auto fit height
    scale = GetScreenHeight() > (img->screen.height * scale) ?
        (float)GetScreenHeight() / (float)img->screen.height :
        scale;
        
    return scale;
}

// Get X axis pixel position corresponding to a percentage (responsive positioning)
float gp_perX(float percent) {
    
    return (float)GetScreenWidth() / 100 * percent;
}

// Get Y axis pixel position corresponding to a percentage (responsive positioning)
float gp_perY(float percent) {
    
    return (float)GetScreenHeight() / 100 * percent;
}

/******* FX ********/

// Function drawing a cross-fade effect between two images
void gp_drawCrossFade(ResImage *imgAbove, ResImage *imgBelow, ScreenFX *screenFx) {
    
    int subDuration = (int)(screenFx->duration / 5) * GAME_FPS;
    
    if(screenFx->enabled) {
        
        // Increment the frame counter, to know the current animation state (until duration seconds)
        if(screenFx->timer < screenFx->duration * GAME_FPS) ++screenFx->timer;
            
        // Wait screenFx->timeout seconds on the imgAbove
        if(screenFx->timer <= subDuration * screenFx->timeout) {
            
            // Ease[Sine,Expo,Cubic,Quad,Circle,Bounce][In,Out,InOut]
            // Easing functions arguments Ease*(currentTime, startPositionX, finalPositionX, duration)
            
            // Fading in/out the imgAbove with its color
            screenFx->alpha = EaseCubicInOut(
                screenFx->timer,
                1.0f, -1.0f,
                subDuration
            );
            
        } else {
            
            if(screenFx->timer > subDuration * 4) {
                
                screenFx->scale = imgBelow->scale;
                
                screenFx->invert = false;
                
                screenFx->alpha = EaseCubicInOut(
                    screenFx->timer - subDuration * 4,
                    1.0f, -1.0f,
                    subDuration
                ); 
            
            } else {
                
                if(screenFx->timer > subDuration * 2.5) {
                    
                    screenFx->invert = true;
                    
                    screenFx->alpha = EaseCubicInOut(
                        screenFx->timer - subDuration * 2.5,
                        1.0f, -1.0f,
                        subDuration
                    );
                }
                
                // Zoom effect on the imgAbove
                screenFx->scale = EaseExpoInOut(
                    screenFx->timer - subDuration * 3,
                    imgAbove->scale, imgAbove->scale + 15.0f,
                    subDuration
                );
            }
        }
        
        // Correct alpha according the current effect direction
        screenFx->alpha = screenFx->invert ? 1.0f - screenFx->alpha : screenFx->alpha;
        
        gp_drawImage(
            screenFx->timer > subDuration * 4 ?
            imgBelow : imgAbove,
            screenFx->scale
        );
        
        // Draw the rectangle fading in imgAbove and cross fading both images, colored with the imgBelow's color 
        DrawRectanglePro(
            (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
            (Vector2){0, 0},
            0, Fade(
                screenFx->timer > subDuration * 2.5 ?
                imgBelow->color : imgAbove->color,
                screenFx->alpha
            )
        );
    }
}

// Function drawing a fullscreen fading animation (either fading in or out)
void gp_drawFade(ScreenFX *screenFx) {
    
    // Define the animation total number of frames
    int duration = screenFx->duration * GAME_FPS;
    
    // Only draw if enabled
    if(screenFx->enabled) {
    
        if (screenFx->timer < duration) ++screenFx->timer;
        else screenFx->enabled = false;
        
        // easing functions arguments Ease*(currentTime, startPositionX, finalPositionX, duration) (often 
        
        // Apply corresponding effect
        switch(screenFx->effect) {
            
            case CUBIC: {
                screenFx->alpha = EaseCubicInOut(screenFx->timer, 1.0f, -1.0f, duration);
            } break;
            
            case SINE: {
                screenFx->alpha = EaseSineOut(screenFx->timer, 1.0f, -1.0f, duration);
            } break;
            
            case EXPO: {
                screenFx->alpha = EaseExpoInOut(screenFx->timer, 1.0f, -1.0f, duration);
            } break;
        }
        
        // Correct alpha according the current direction
        screenFx->alpha = screenFx->invert ? 1.0f - screenFx->alpha : screenFx->alpha;
        
        // Draw the fade effect
        DrawRectanglePro(
            (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
            (Vector2){0, 0},
            screenFx->rotation,
            Fade(screenFx->color, screenFx->alpha)
        );
    }
}

// Function playing bouncing text animation
void gp_drawBounceText(char *str, Font font, Vector2 position, TextAlign align, ScreenFX *screenFx) {
    
    // Define the animation total number of frames and split it (animation bacl-and-forth)
    int subDuration = (int)(screenFx->duration / 2) * GAME_FPS;
    float incrementSize = (screenFx->scaleFinal - screenFx->scaleBase) / subDuration;
    
    // Only draw if enabled
    if(screenFx->enabled) {
        
        // Increment/Reset frame timer according the effect duration
        if(screenFx->timer < screenFx->duration * GAME_FPS) ++screenFx->timer;
        else screenFx->timer = 0;
        
        // Either scale up/down font size (screenFx->scale) according the duration
        if(screenFx->timer <= subDuration && screenFx->scale <= screenFx->scaleFinal) {
        
            screenFx->scale += incrementSize;
        
        } else if(screenFx->timer > subDuration && screenFx->scale >= screenFx->scaleBase) {
            
            screenFx->scale -= incrementSize;
        }
        
        // Draw the animated text
        gp_drawText(
            str, font,
            position, screenFx->scale,
            align, screenFx->color
        );
    }
}

// Function reseting a screen effect object
void gp_resetFx(ScreenFX *screenFx) {
    
    screenFx->timer = 0;
    
    screenFx->alpha = 1.0f;
    screenFx->scale = 1.0f;
    screenFx->rotation = 0;
    screenFx->duration = 1;
    screenFx->timeout = 1;
    
    screenFx->effect = CUBIC;
    screenFx->color = BLACK;
    
    screenFx->invert = false;
    screenFx->enabled = true;
}

/******* INITIALIZERS ********/

// Function initializing resources from their position on the corresponding spritecheet
void gp_initResources(Resources *res) {
    
    // items drawing position will be defined just before, allowing several position to be drawn
    //res->items.ASSET.position
    //res->items.ASSET.scale
    
    // Player's head (lives icon)
    gp_initResourcesAssets(
        &res->items.player_head,
        SP_ASSETS,
        (Vector2){270,424}, // sprite frame position
        1, 1, // amount of animated frames, number of frame lines
        233, 129, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );
    
    // Extra Life Bonus
    gp_initResourcesAssets(
        &res->items.extraLife,
        SP_ASSETS,
        (Vector2){270,424}, // sprite frame position
        1, 1, // amount of animated frames, number of frame lines
        233, 129, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );

    // Lean
    gp_initResourcesAssets(
        &res->items.lean,
        SP_ASSETS,
        (Vector2){503,404}, // sprite frame position
        1, 1, // amount of animated frames, number of frame lines
        99, 168, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );    
    
    // Spikes
    gp_initResourcesAssets(
        &res->items.spikes,
        SP_ASSETS,
        (Vector2){0,679}, // sprite frame position
        1, 1, // amount of animated frames, number of frame lines
        246, 69, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );
    
    // Launchpad (spring, animated)
    gp_initResourcesAssets(
        &res->items.launchpad,
        SP_ASSETS,
        (Vector2){0,0}, // sprite frame position
        8, 1, // amount of animated frames, number of frame lines
        180, 174, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 15 // animate, loop animation, loop position options, frame speed
    );
    
    // Portal (animated)
    gp_initResourcesAssets(
        &res->items.portal,
        SP_ASSETS,
        (Vector2){0,404}, // sprite frame position
        5, 1, // amount of animated frames, number of frame lines
        54, 168, // width, height
        1, 0, // scale, rotation
        true, true, (Vector2){0,0}, 10 // animate, loop animation, loop position options, frame speed
    );
    
    // Portal (non-animated)
    gp_initResourcesAssets(
        &res->items.portal_bonus,
        SP_ASSETS,
        (Vector2){0,404}, // sprite frame position
        5, 1, // amount of animated frames, number of frame lines
        54, 168, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 10 // animate, loop animation, loop position options, frame speed
    );
    
    // Mouse (credits to Guilaume, teacher)
    gp_initResourcesAssets(
        &res->items.mouse,
        SP_ASSETS,
        (Vector2){246,679}, // sprite frame position
        1, 1, // amount of animated frames, number of frame lines
        56, 60, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );
    
    // Gob walking (animated)
    gp_initResourcesAssets(
        &res->items.gob_walking,
        SP_ASSETS,
        (Vector2){0,174}, // sprite frame position
        6, 1, // amount of animated frames, number of frame lines
        129, 110, // width, height
        gp_perX(12) / 129, 0, // scale, rotation
        true, true, (Vector2){0,0}, 9 // animate, loop animation, loop position options, frame speed
    );
    
    // Gob dying (animated)
    gp_initResourcesAssets(
        &res->items.gob_dying,
        SP_ASSETS,
        (Vector2){0,284}, // sprite frame position
        7, 1, // amount of animated frames, number of frame lines
        158, 120, // width, height
        gp_perX(12) / 158, 0, // scale, rotation
        true, false, (Vector2){0,0}, 8 // animate, loop animation, loop position options, frame speed
    );
    
    // Dark sonic
    gp_initResourcesAssets(
        &res->items.dark_sonic,
        SP_ASSETS,
        (Vector2){1110,174}, // sprite frame position
        1, 1, // amount of animated frames, number of frame lines
        214, 506, // width, height
        gp_perX(15) / 214, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );
    
    // Damage (hit, animated)
    gp_initResourcesAssets(
        &res->items.damage,
        SP_ASSETS,
        (Vector2){0,572}, // sprite frame position
        10, 1, // amount of animated frames, number of frame lines
        111, 107, // width, height
        0.5, 0, // scale, rotation
        true, false, (Vector2){0,0}, 10 // animate, loop animation, loop position options, frame speed
    );
    
    // Firebal (animated)
    gp_initResourcesAssets(
        &res->items.fireball,
        SP_ASSETS,
        (Vector2){302,679}, // sprite frame position
        4, 1, // amount of animated frames, number of frame lines
        39, 55, // width, height
        1, 0, // scale, rotation
        true, true, (Vector2){0,0}, 10 // animate, loop animation, loop position options, frame speed
    );
    
    // Arrow keys
    gp_initResourcesAssets(
        &res->items.kb_arrows,
        SP_ASSETS,
        (Vector2){602,404}, // sprite frame position
        1, 1, // amount of animated frames, number of frame lines
        221, 168, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );
    
    // Spacebar
    gp_initResourcesAssets(
        &res->items.kb_spacebar,
        SP_ASSETS,
        (Vector2){803,488}, // sprite frame position
        1, 1, // amount of animated frames, number of frame lines
        246, 84, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );
    
    // Gelano
    gp_initResourcesAssets(
        &res->items.gelano,
        SP_ASSETS,
        (Vector2){458,679}, // sprite frame position
        1, 1, // amount of animated frames, number of frame lines
        40, 49, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );

    // Barrel FULL
    gp_initResourcesAssets(
        &res->items.barrel_full,
        SP_ASSETS,
        (Vector2){503,679}, // sprite frame position
        1, 0, // amount of animated frames, number of frame lines
        35, 32, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );
    
    // Barrel BROKEN
    gp_initResourcesAssets(
        &res->items.barrel_broken,
        SP_ASSETS,
        (Vector2){539,679}, // sprite frame position
        1, 0, // amount of animated frames, number of frame lines
        35, 32, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );
    
    // Player IDLE (menu screen)
    gp_initResourcesAssets(
        &res->items.player_idle,
        SP_PLAYER,
        (Vector2){0,90}, // sprite frame position
        9, 1, // amount of animated frames, number of frame lines
        30, 30, // width, height
        gp_perX(6) / 30, 0, // scale, rotation
        true, true, (Vector2){0,0}, 2 // animate, loop animation, loop position options, frame speed
    );
    
    // Player animation FLIP
    gp_initResourcesAssets(
        &res->items.player_anim_flip,
        SP_ANIMATION,
        (Vector2){0,0}, // sprite frame position
        56, 3, // amount of animated frames, number of frame lines
        45, 49, // width, height
        1.9, 0, // scale, rotation
        true, false, (Vector2){13,0}, 8 // animate, loop animation, loop position options, frame speed
    );
    
    // Player animation TRANSFORM
    gp_initResourcesAssets(
        &res->items.player_anim_transform,
        SP_ANIMATION,
        (Vector2){0,441}, // sprite frame position
        33, 2, // amount of animated frames, number of frame lines
        43, 43, // width, height
        1.5, 0, // scale, rotation
        true, false, (Vector2){0,0}, 5 // animate, loop animation, loop position options, frame speed
    );
    
    // Player animation SLEEP
    gp_initResourcesAssets(
        &res->items.player_anim_sleep,
        SP_ANIMATION,
        (Vector2){0,699}, // sprite frame position
        23, 1, // amount of animated frames, number of frame lines
        40, 40, // width, height
        gp_perX(6) / 40, 0, // scale, rotation
        true, true, (Vector2){18,0}, 4 // animate, loop animation, loop position options, frame speed
    );
    
    // Player animation CLAP
    gp_initResourcesAssets(
        &res->items.player_anim_clap,
        SP_ANIMATION,
        (Vector2){0,819}, // sprite frame position
        11, 1, // amount of animated frames, number of frame lines
        40, 40, // width, height
        gp_perX(6) / 40, 0, // scale, rotation
        true, false, (Vector2){0,0}, 4 // animate, loop animation, loop position options, frame speed
    );
    
    // Goblean + Skate
    gp_initResourcesAssets(
        &res->items.goblean_skate,
        SP_ASSETS,
        (Vector2){1324,284}, // sprite frame position
        1, 1, // amount of animated frames, number of frame lines
        262, 289, // width, height
        gp_perX(16) / 262, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );
    
    // Goblean flip
    gp_initResourcesAssets(
        &res->items.goblean_flip,
        SP_ASSETS,
        (Vector2){1583,282}, // sprite frame position
        9, 1, // amount of animated frames, number of frame lines
        330, 293, // width, height
        gp_perX(16) / 330, 0, // scale, rotation
        true, true, (Vector2){0,0}, 8 // animate, loop animation, loop position options, frame speed
    );
    
    // Skate flip
    gp_initResourcesAssets(
        &res->items.skate_flip,
        SP_ASSETS,
        (Vector2){1584,0}, // sprite frame position
        9, 1, // amount of animated frames, number of frame lines
        195, 158, // width, height
        gp_perX(11) / 195, 0, // scale, rotation
        true, true, (Vector2){0,0}, 9 // animate, loop animation, loop position options, frame speed
    );
    
    // Left hand
    gp_initResourcesAssets(
        &res->items.left_hand,
        SP_ASSETS,
        (Vector2){1362,616}, // sprite frame position
        1, 1, // amount of animated frames, number of frame lines
        85, 59, // width, height
        gp_perX(4) / 85, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );
    
    // Right hand
    gp_initResourcesAssets(
        &res->items.right_hand,
        SP_ASSETS,
        (Vector2){1470,616}, // sprite frame position
        1, 1, // amount of animated frames, number of frame lines
        85, 59, // width, height
        gp_perX(4) / 85, 0, // scale, rotation
        false, false, (Vector2){0,0}, 0 // animate, loop animation, loop position options, frame speed
    );
    
    // Lifebar
    gp_initResourcesAssets(
        &res->items.lifebar,
        SP_ASSETS,
        (Vector2){14,784}, // sprite frame position
        6, 1, // amount of animated frames, number of frame lines
        186, 44, // width, height
        1, 0, // scale, rotation
        false, false, (Vector2){0,0}, 5 // animate, loop animation, loop position options, frame speed
    );
    
    // Fire columns
    gp_initResourcesAssets(
        &res->items.fire_columns,
        SP_ASSETS,
        (Vector2){1160,720}, // sprite frame position
        11, 1, // amount of animated frames, number of frame lines
        60, 106, // width, height
        gp_perX(15) / 60, 0, // scale, rotation
        false, true, (Vector2){0,0}, 8 // animate, loop animation, loop position options, frame speed
    );
    
    /********* SCREENS ********/
    /*
    // SPLASH screen
    gp_initResourceScreens(
        &res->backgrounds.splash,
        SP_SCREENS,
        (Vector2){0,0},
        950, 540,
        0.85f, BLACK
    );
    
    // MENU screen
    gp_initResourceScreens(
        &res->backgrounds.menu,
        SP_SCREENS,
        (Vector2){0,550},
        950, 540,
        0.85f, (Color){242, 215, 255, 255}
    );
    
    // CONTROLS screen
    gp_initResourceScreens(
        &res->backgrounds.controls,
        SP_SCREENS,
        (Vector2){0,1100},
        950, 540,
        0.85f, WHITE
    );
    
    // VICTORY screen
    gp_initResourceScreens(
        &res->backgrounds.victory,
        SP_SCREENS,
        (Vector2){0,1650},
        950, 540,
        0.85f, WHITE
    );
    
    // GAMEOVER screen
    gp_initResourceScreens(
        &res->backgrounds.gameover,
        SP_SCREENS,
        (Vector2){0,2200},
        950, 540,
        0.85f, WHITE
    );
    
    // CREDITS screen
    gp_initResourceScreens(
        &res->backgrounds.credits,
        SP_SCREENS,
        (Vector2){961,0},
        1280, 720,
        0.65f, WHITE
    );
    
    // SELECT screen
    gp_initResourceScreens(
        &res->backgrounds.select,
        SP_SCREENS,
        (Vector2){961,730},
        1280, 720,
        0.65f, WHITE
    );
    
    // LEVEL 3 screen
    gp_initResourceScreens(
        &res->backgrounds.level3,
        SP_SCREENS,
        (Vector2){1126,1600},
        950, 534,
        0.85f, WHITE
    );
    
    // LEVEL 5 screen
    gp_initResourceScreens(
        &res->backgrounds.level5,
        SP_SCREENS,
        (Vector2){1126,2200},
        950, 534,
        0.85f, WHITE
    );
    
    // SPECIAL screen
    gp_initResourceScreens(
        &res->backgrounds.special,
        SP_SCREENS,
        (Vector2){2251,0},
        950, 540,
        0.85f, WHITE
    );
    
    // LEVEL 1 screen
    gp_initResourceScreens(
        &res->backgrounds.level1,
        SP_SCREENS,
        (Vector2){2251,550},
        952, 536,
        0.85f, WHITE
    );
    
    // LEVEL 2 screen
    gp_initResourceScreens(
        &res->backgrounds.level2,
        SP_SCREENS,
        (Vector2){2251,1100},
        950, 534,
        0.85f, WHITE
    );
    
    // LEVEL BONUS screen
    gp_initResourceScreens(
        &res->backgrounds.level_bonus,
        SP_SCREENS,
        (Vector2){2251,1644},
        950, 534,
        0.85f, WHITE
    );
    
    // LEVEL 4 screen
    gp_initResourceScreens(
        &res->backgrounds.level4,
        SP_SCREENS,
        (Vector2){2251,2200},
        950, 534,
        0.85f, WHITE
    );
    */
}

// Function generating default values for initial items/assets 
void gp_initResourcesAssets(Asset *asset, SpritePack sprite, Vector2 framePos, int frameAmount, int frameLines, float width, float height, float scale, int rotation, bool animate, bool loop, Vector2 loopPos, int frameSpeed) {
    
    // Initializing current asset sprite frame position and dimensions
    asset->sprite = sprite;
    
    asset->frame.xinit = framePos.x;
    asset->frame.yinit = framePos.y;
    asset->frame.x = framePos.x;
    asset->frame.y = framePos.y;
    
    asset->frame.amount = frameAmount;
    asset->frame.lines = frameLines;
    asset->frame.speed = frameSpeed;
    
    asset->scale = scale;
    asset->rotation = rotation;
    
    asset->frame.animate = animate;
    asset->frame.loop = loop;
    asset->frame.loopPos = loopPos;
    
    asset->width = width;
    asset->height = height;
    asset->swidth = scale * width;
    asset->sheight = scale * height;
}

void gp_initResourceScreens(Asset *asset, SpritePack sprite, Vector2 pos, int width, int height, float scale, Color color) {
    
    asset->sprite = sprite;
    
    asset->frame.x = pos.x;
    asset->frame.y = pos.y;
    
    asset->scale = scale;
    
    asset->width = width;
    asset->height = height;
    asset->swidth = scale * width;
    asset->sheight = scale * height;
    
    asset->color = color;
    asset->rotation = 0;
}

// Function initializing player initial levels position
void gp_initPositions(LevelPosition *levelPos) {
    
    Vector2 defaultPos = (Vector2){
        gp_perX(5),
        gp_perY(40)
    };
    
    levelPos->level_1 = defaultPos;
    levelPos->level_2 = defaultPos;
    levelPos->level_3 = (Vector2){gp_perX(5), gp_perY(25)};
    levelPos->level_4 = defaultPos;
    levelPos->level_5 = defaultPos;
    levelPos->level_bonus = (Vector2){gp_perX(30), gp_perY(60)};
}

// Function initializing screen/text effect objects
void gp_initFx(ResourcesFX *fx) {
    
    // Set default values for each effect object
    gp_resetFx(&fx->fade);
    gp_resetFx(&fx->crossFade);
    gp_resetFx(&fx->bounceText);
    
}
