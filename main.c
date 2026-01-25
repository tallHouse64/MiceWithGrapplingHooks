#define D_IMPLEMENTATION
#include"d.h"

#define DEVENTS_IMPLEMENTATION
#include"devents.h"

#define D_PLATFORM_IMPLEMENTATION
#include"platform/sdld.h"

#include"main.h"
#include"physics/physics.h"

#include"assets/mouse.h"


/* This function draws a map onto a surface.
 *
 * You can choose what size the map should be
 *  drawn by controlling the zoom number. Set
 *  this number to 256 for no zoom effect. Make
 *  it smaller to zoom in and bigger to zoom out.
 *  If zoom is set to 0, the function would do
 *  nothing and return -2.
 *
 * s: The surface to draw onto.
 * map: The map to draw.
 * cameraX: The x position of the camera.
 * cameraY: The x position of the camera.
 * zoom: Zoom number to scale the result (read
 *  above).
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_DrawMap(D_Surf * s, MWG_Map * map, int cameraX, int cameraY, int zoom){

    if(s == D_NULL || map == D_NULL){
        return -1;
    };

    if(zoom == 0){
        return -2;
    };

    D_Rect r = {0};
    D_Point centre = {0};
    int i = 0;
    while(i < map->numRects){

        r.x = (((map->rect[i].x - cameraX) * 256) / zoom) + (s->w / 2);
        r.y = (((map->rect[i].y - cameraY) * 256) / zoom) + (s->h / 2);
        r.w = (map->rect[i].w * 256) / zoom;
        r.h = (map->rect[i].h * 256) / zoom;

        D_FillRect(s, &r, D_rgbaToFormat(s->format, map->rect[i].r, map->rect[i].g, map->rect[i].b, 255));

        i++;
    };

    i = 0;
    D_Point hook = {0};
    D_Point player = {0};
    while(i < map->numPlayers){

        /* Is the player's grappling hook
         *  attached to something? */
        if(map->player[i].hookState == MWG_HOOK_ATTACHED){

            /* Draw the grappling hook line */
            hook.x = ((((map->player[i].hookX) - cameraX) * 256) / zoom) + (s->w / 2);
            hook.y = ((((map->player[i].hookY) - cameraY) * 256) / zoom) + (s->h / 2);
            player.x = ((((map->player[i].x) - cameraX) * 256) / zoom) + (s->w / 2);
            player.y = ((((map->player[i].y) - cameraY) * 256) / zoom) + (s->h / 2);
            D_DrawLine(s, &hook, &player, (5 * 256) / zoom, D_rgbaToFormat(s->format, 70, 70, 70, 255));
        };

        r.x = ((((map->player[i].x + map->player[i].imageX) - cameraX) * 256) / zoom) + (s->w / 2);
        r.y = ((((map->player[i].y + map->player[i].imageY) - cameraY) * 256) / zoom) + (s->h / 2);
        r.w = (map->player[i].imageW * 256) / zoom;
        r.h = (map->player[i].imageH * 256) / zoom;

        centre.x = (map->player[i].rotateCentreX * 256) / zoom;
        centre.y = (map->player[i].rotateCentreY * 256) / zoom;

        /* Is the player looking right? */
        if(map->player[i].angle <= 90 && map->player[i].angle >= -90){
            D_SurfCopyScaleRot(map->player[i].image, D_NULL, s, &r, &centre, map->player[i].angle, 0, 0);
        }else{
            D_SurfCopyScaleRot(map->player[i].image, D_NULL, s, &r, &centre, map->player[i].angle, 1, 0);
        };

        i++;
    };

    return 0;
};

int MWG_FireHook(MWG_Map * map, MWG_Player * player){

    if(map == D_NULL || player == D_NULL){
        return -1;
    };

    /* The code below multiplies complex numbers
     *  to rotate the point degR and degC on the
     *  complex plane. */

    /* Note that this pR and pC rotate a point by
     *  0.01 degrees, not 1 degree. */
    const D_double pR = 0.999999984769;
    const D_double pC = 0.000174532924313;

    D_double temp = 0;

    D_double hookDirectionX = 100;
    D_double hookDirectionY = 0;

    /* Make angle2, non-negative */
    D_double angle2 = player->angle;
    while(angle2 < 0){
        angle2 += 360;
    };

    /* Convert angle2 from degrees to a point on
     *  the complex plane */
    D_double reps = 0;
    while(reps < angle2){

        /* Add 0.01 degree to degR and degC */
        temp = D_COMPLEXMULTR(hookDirectionX, hookDirectionY, pR, pC);
        hookDirectionY = D_COMPLEXMULTC(hookDirectionX, hookDirectionY, pR, pC);
        hookDirectionX = temp;

        reps += 0.01;
    };

    /* Convert hook direction to absolute
     *  coordinates. */
    hookDirectionX = hookDirectionX + player->x;
    hookDirectionY = hookDirectionY + player->y;

    int i = 0;
    int tempHookX = 0;
    int tempHookY = 0;
    int foundAHookPosition = 0;
    for(; i < map->numRects; i++){
        if(MWG_FindEntryPoint(hookDirectionX, hookDirectionY, player->x, player->y, &map->rect[i], &tempHookX, &tempHookY, D_NULL, D_NULL) == 0){

            /* Is the player looking right? */
            if(player->angle <= 90 && player->angle >= -90){

                /* If the player isn't looking at
                 *  the hook point, the player
                 *  didn't mean to shoot there.
                 *  Ignore this result. */
                if(tempHookX < player->x){
                    continue;
                };

            }else{
                /* The player is looking left. */

                if(tempHookX > player->x){
                    continue;
                };
            };

            /* Is the player looking down? */
            if(player->angle > 0 && player->angle <= 180){

                /* If the player isn't looking at
                 *  the hook point, the player
                 *  didn't mean to shoot there.
                 *  Ignore this result. */
                if(tempHookY < player->y){
                    continue;
                };

            }else{
                /* The player is looking up. */

                if(tempHookY > player->y){
                    continue;
                };
            };

            /* If this is the first hook position
             *  found (this function call), then
             *  set it to the player's hook
             *  position. Otherwise only set the
             *  player's hook position if it
             *  closer than the last hook
             *  position found. */
            if( !foundAHookPosition ||
                ((tempHookX     - player->x) * (tempHookX     - player->x)) + ((tempHookY     - player->y) * (tempHookY     - player->y)) <
                ((player->hookX - player->x) * (player->hookX - player->x)) + ((player->hookY - player->y) * (player->hookY - player->y))
            ){
                foundAHookPosition = 1;
                player->hookX = tempHookX;
                player->hookY = tempHookY;
                player->hookState = MWG_HOOK_ATTACHED;
            };
        };
    };

    return 0;
};

int main(int argc, char ** argv){
    D_Surf * out = D_GetOutSurf(50, 50, 640, 480, "Mice With Grappling Hooks", 0);
    int running = 1;
    int cameraX = 0;
    int cameraY = 0;
    D_Event e = {0};
    D_uint8 keyboardState[D_NumKeys] = {0};
    int zoom = 256;
    D_Surf * mouseImage = D_CreateSurfFrom(mouseDataW, mouseDataH, 0, D_NULL, D_FindPixFormat(0xFF, 0xFF00, 0xFF0000, 0xFF000000, 32), mouseData);

    MWG_Map map = {
        /* MapRects */
        {
            {
                -250, 30, 500, 40,
                17, 127, 11
            },
            {
                -200, -210, 100, 100,
                17, 127, 11
            }
        },
        2, /* numRects */

        /* Players */
        {
            100, 0, /* x, y */
            0, 0, /* oldX, oldY */

            /* Hitbox x, y, w, h */
            -(MWG_PLAYER_WIDTH / 2), -(MWG_PLAYER_HEIGHT / 2), MWG_PLAYER_WIDTH, MWG_PLAYER_HEIGHT,

            /* Surface */
            mouseImage,

            /* Image x, y, w, h */
            -((mouseDataW * 2) / 2), -((mouseDataH * 2) / 2), (mouseDataW * 2), (mouseDataH * 2),

            /* Center of rotation */
            (mouseDataW * 2) / 2, (mouseDataH * 2) / 2,

            /* Grappling hook x, y */
            0, 0,

            MWG_HOOK_UNATTACHED,

            /* Angle */
            0.0
        },

        1 /* numPlayers*/
    };

    D_StartEvents();

    while(running){
        D_PumpEvents();

        while(D_GetEvent(&e) != -1){
            switch(e.type){
                case D_QUIT:
                    running = 0;
                    break;

                case D_KEYDOWN:
                    keyboardState[e.keyboard.key] = 1;

                    if(e.keyboard.key == D_KSpace){
                        map.player[0].oldY += 70;
                    };

                    break;

                case D_KEYUP:
                    keyboardState[e.keyboard.key] = 0;
                    break;
            };
        };

        MWG_CalcPhysics(&map);

        /* When a is pressed move left */
        if(keyboardState[D_Ka]){
            map.player[0].oldX += (78 * DELAY) / 256;

            /* Is the player looking right? */
            if(map.player[0].angle <= 90 &&
                map.player[0].angle >= -90){

                /*Flip the player to look left*/
                map.player[0].angle = 180 - map.player[0].angle;
            };
        };

        /* When d is pressed move right */
        if(keyboardState[D_Kd]){
            map.player[0].oldX -= (78 * DELAY) / 256;

            /* Is the player looking left? */
            if(map.player[0].angle > 90 ||
                map.player[0].angle < -90){

                /*Flip the player to look right*/
                map.player[0].angle = 180 - map.player[0].angle;
                };
        };

        /* Control zoom with the i and o keys */
        if(keyboardState[D_Ki]){zoom -= 10;};
        if(keyboardState[D_Ko]){zoom += 10;};

        /* When e is pressed change the player's
         *  angle */
        if(keyboardState[D_Ke]){

            /* Is the player looking right? */
            if( map.player[0].angle <= 90 &&
                map.player[0].angle >= -90
                ){

                /* Change the angle */
                map.player[0].angle += 15;

                /* Limit the angle to be 90 or
                 *  less. */
                if(map.player[0].angle > 90){
                    map.player[0].angle = 90;
                };
            }else{
                /* At this point the player must
                 *  be looking left */

                /* Change the angle */
                 map.player[0].angle += 15;

                 /* Limit the angle to be 269 or
                  *  less. */
                 if(map.player[0].angle >= 269){
                     map.player[0].angle = 269;
                 };
            };
        };

        /* When q is pressed change the player's
         *  angle (the opposite way) */
        if(keyboardState[D_Kq]){

            /* Is the player looking right? */
            if( map.player[0].angle <= 90 &&
                map.player[0].angle >= -90
            ){

                /* Change the angle */
                map.player[0].angle -= 15;

                /* Limit the angle to be -90 or
                 *  more. */
                if(map.player[0].angle < -90){
                    map.player[0].angle = -90;
                };
            }else{
                /* At this point the player must
                 *  be looking left */

                /* Change the angle */
                map.player[0].angle -= 15;

                /* Limit the angle to be 91 or
                 *  more. */
                if(map.player[0].angle <= 91){
                    map.player[0].angle = 91;
                };
            };
        };

        if(keyboardState[D_Kw]){
            MWG_FireHook(&map, &map.player[0]);
        };

        if(keyboardState[D_Ks]){
            map.player[0].hookState = MWG_HOOK_UNATTACHED;
        };


        D_FillRect(out, D_NULL, D_rgbaToFormat(out->format, 181, 233, 255, 255));

        MWG_DrawMap(out, &map, map.player[0].x, map.player[0].y, zoom);

        D_FlipOutSurf(out);

        D_Delay(DELAY);
    };

    D_StopEvents();

    D_FreeOutSurf(out);
    out = D_NULL;

    return 0;
};
