#define D_IMPLEMENTATION
#include"d.h"

#define DEVENTS_IMPLEMENTATION
#include"devents.h"

#define D_PLATFORM_IMPLEMENTATION
#include"platform/sdld.h"

#include"main.h"
#include"physics/physics.h"
#include"maps/maps.h"

#include"assets/mouse.h"
#include"assets/font.h"


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

        D_SurfCopyScale(map->rect[i].image, D_NULL, s, &r);

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

/* This function draws a menu onto a surface.
 *
 * It is safe to pass null for s, font and menu,
 *  the function would do nothing and return -1.
 *
 * If zoom is 0 the function does nothing and
 *  returns -2.
 *
 * If there are too many buttons in the menu, the
 *  function returns -3.
 *
 * s: The surface to draw on.
 * menu: The menu to draw.
 * font: The font to draw text with (see the
 *  documentation for D_PrintToSurf()).
 * zoom: How much to zoom (256 for no zoom
 *  effect).
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_DrawMenu(D_Surf * s, MWG_Menu * menu, D_Surf * font, int zoom){

    int i = 0;
    D_Rect r = {0};
    D_Point textPoint = {0};

    if(s == D_NULL || font == D_NULL || menu == D_NULL){
        return -1;
    };

    if(zoom == 0){
        return -2;
    };

    if(menu->numButtons > MWG_MAX_BUTTONS){
        return -3;
    };

    for(;i < menu->numButtons; i++){
        r.x = ((menu->button[i].rect.x * 256) / zoom) + (s->w / 2);
        r.y = ((menu->button[i].rect.y * 256) / zoom) + (s->h / 2);
        r.w = (menu->button[i].rect.w * 256) / zoom;
        r.h = (menu->button[i].rect.h * 256) / zoom;

        D_FillRect(s, &r, D_rgbaToFormat(s->format, menu->button[i].r, menu->button[i].g, menu->button[i].b, 255));

        textPoint.x = r.x;
        textPoint.y = r.y;
        D_PrintToSurf(s, font, &textPoint, r.h, 0, menu->button[i].text);
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

/* This function controls a player based off
 *  input, it also handles input for a menu.
 *
 * This function takes input from keyboard state
 *  and an event struct separately. This is
 *  because some actions like moving left or
 *  right need to know weather a key is down and
 *  therefore need the state of a key, but an
 *  actions like jump only need to run on a
 *  single frame when a key goes down and needs
 *  an event.
 *
 * It is safe to pass null for all the
 *  parameters. If map is null, the player won't
 *  be able to shoot a grappling hook. If "p" is
 *  null, the function would do nothing and
 *  return -1.
 *
 * p: The player to control.
 * menu: The menu to control (if there is one).
 * e: An input event.
 * keyboardState: An array of uint8s that is the
 *  length of D_NumKeys.
 * map: The map the player is in (needed when the
 *  player fires a grappling hook).
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_ControlPlayer(MWG_Player * p, MWG_Menu * menu, D_Event * e, D_uint8 * keyboardState, MWG_Map * map){

    if(menu != D_NULL && e != D_NULL){
        switch(e->type){
            case D_KEYDOWN:

                /* When any button is pressed, if
                 *  no button is being hovered
                 *  over then hover over the
                 *  zeroth one. */
                if(menu->hoveredButton < 0){
                    menu->hoveredButton = 0;
                };

                if((e->keyboard.key == D_Kw || e->keyboard.key == D_KUp) && menu->button[menu->hoveredButton].upButton >= 0){
                    menu->hoveredButton = menu->button[menu->hoveredButton].upButton;
                };

                if((e->keyboard.key == D_Ks || e->keyboard.key == D_KDown) && menu->button[menu->hoveredButton].downButton >= 0){
                    menu->hoveredButton = menu->button[menu->hoveredButton].downButton;
                };

                if((e->keyboard.key == D_Ka || e->keyboard.key == D_KLeft) && menu->button[menu->hoveredButton].leftButton >= 0){
                    menu->hoveredButton = menu->button[menu->hoveredButton].leftButton;
                };

                if((e->keyboard.key == D_Kd || e->keyboard.key == D_KRight) && menu->button[menu->hoveredButton].rightButton >= 0){
                    menu->hoveredButton = menu->button[menu->hoveredButton].rightButton;
                };

                break;
        };
    };

    /* The code below only handles player input
     *  on a map (not a menu). This is why it
     *  returns -1 if p is null. */

    if(p == D_NULL){
        return -1;
    };

    if(e != D_NULL){
        if(e->type == D_KEYDOWN){
            if(e->keyboard.key == D_KSpace){

                /* Jump when the space key goes
                 *  down. */
                p->oldY += 70;
            };
        };
    };

    /* At this point onwards in the function, all
     *  the key press checks need keyboard state
     *  to be non-null. */

    if(keyboardState == D_NULL){
        /* Running the rest of the function at
         *  this point would crash the program.
         *  So just return. */
        return 0;
    };

    /* When a is pressed move left */
    if(keyboardState[D_Ka]){
        p->oldX += (78 * DELAY) / 256;

        /* Is the player looking right? */
        if(p->angle <= 90 && p->angle >= -90){

            /*Flip the player to look left*/
            p->angle = 180 - p->angle;
        };
    };

    /* When d is pressed move right */
    if(keyboardState[D_Kd]){
        p->oldX -= (78 * DELAY) / 256;

        /* Is the player looking left? */
        if(p->angle > 90 || p->angle < -90){

            /*Flip the player to look right*/
            p->angle = 180 - p->angle;
        };
    };

    /* When e is pressed change the player's
     *  angle */
    if(keyboardState[D_Ke]){

        /* Is the player looking right? */
        if( p->angle <= 90 && p->angle >= -90){

            /* Change the angle */
            p->angle += 15;

            /* Limit the angle to be 90 or
             *  less. */
            if(p->angle > 90){
                p->angle = 90;
            };
        }else{
            /* At this point the player must
             *  be looking left */

            /* Change the angle */
            p->angle += 15;

            /* Limit the angle to be 269 or
             *  less. */
            if(p->angle >= 269){
                p->angle = 269;
            };
        };
    };

    /* When q is pressed change the player's
     *  angle (the opposite way) */
    if(keyboardState[D_Kq]){

        /* Is the player looking right? */
        if( p->angle <= 90 && p->angle >= -90){

            /* Change the angle */
            p->angle -= 15;

            /* Limit the angle to be -90 or
             *  more. */
            if(p->angle < -90){
                p->angle = -90;
            };
        }else{
            /* At this point the player must
             *  be looking left */

            /* Change the angle */
            p->angle -= 15;

            /* Limit the angle to be 91 or
             *  more. */
            if(p->angle <= 91){
                p->angle = 91;
            };
        };
    };

    if(keyboardState[D_Kw] && map != D_NULL){
        MWG_FireHook(map, p);
    };

    if(keyboardState[D_Ks]){
        p->hookState = MWG_HOOK_UNATTACHED;
    };

    return 0;
};

/* This function adds a player to a map.
 *
 * If the map is full, the function would do
 *  nothing and return -2.
 *
 * It is safe to pass null for image, bear in
 *  mind that this may cause a crash elsewhere in
 *  the program (but it should be possible to set
 *  this to null for a player without an image).
 *
 * It is safe to pass null for map, it would do
 *  nothing and return -1.
 *
 * map: The map to add a player to.
 * x: The x position of where the player should
 *  appear.
 * y: The y position of where the player should
 *  appear.
 * hitboxW: The width of the hit box.
 * hitboxH: The height of the hit box.
 * image: The image to set the player.
 * imageW: The width the image should be drawn,
 *  this does NOT have to match the width of the
 *  image itself.
 * imageH: The height the image should be drawn,
 *  this does NOT have to match the width of the
 *  image itself.
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_AddPlayer(MWG_Map * map, int x, int y, int hitboxW, int hitboxH, D_Surf * image, int imageW, int imageH){

    int i = 0;

    if(map == D_NULL){
        return -1;
    };

    if(map->numPlayers >= MWG_MAX_PLAYER){
        return -2;
    };

    i = map->numPlayers;

    map->player[i].x = x;
    map->player[i].y = y;
    map->player[i].oldX = x;
    map->player[i].oldY = y;

    map->player[i].hitboxX = -(hitboxW / 2);
    map->player[i].hitboxY = -(hitboxH / 2);
    map->player[i].hitboxW = hitboxW;
    map->player[i].hitboxH = hitboxH;

    map->player[i].image = image;

    map->player[i].imageX = -(imageW / 2);
    map->player[i].imageY = -(imageH / 2);
    map->player[i].imageW = imageW;
    map->player[i].imageH = imageH;

    map->player[i].rotateCentreX = imageW / 2;
    map->player[i].rotateCentreY = imageH / 2;

    map->player[i].hookX = 0;
    map->player[i].hookY = 0;

    map->player[i].hookState = MWG_HOOK_UNATTACHED;

    map->player[i].angle = 0.0;

    map->numPlayers = map->numPlayers + 1;

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
    D_Surf * fontImage = D_CreateSurfFrom(fontDataW, fontDataH, 0, D_NULL, D_FindPixFormat(0xFF, 0xFF00, 0xFF0000, 0xFF000000, 32), fontData);

    MWG_Menu mainMenu = {
        -1,
        {
        {
            {-210, -50, 420, 30},
            "Start crashing",
            14,
            -1,
            1,
            -1,
            -1,
            D_NULL,
            D_NULL,
            0, 0, 0, /* Colour */
            100, 100, 100 /* Hover colour */
        },
        {
            {-210, -10, 120, 30},
            "Exit",
            14,
            0,
            -1,
            -1,
            -1,
            D_NULL,
            D_NULL,
            0, 0, 0, /* Colour */
            100, 100, 100 /* Hover colour */
        }
        },
        2 /* Num buttons */
    };

    MWG_Map map = testMap;

    MWG_AddPlayer(&map, 0, 0, 40, 40, mouseImage, mouseDataW * 2, mouseDataH * 2);


    D_StartEvents();

    while(running){
        D_PumpEvents();

        while(D_GetEvent(&e) != -1){

            MWG_ControlPlayer(&map.player[0], &mainMenu, &e, D_NULL, &map);

            switch(e.type){
                case D_QUIT:
                    running = 0;
                    break;

                case D_KEYDOWN:
                    keyboardState[e.keyboard.key] = 1;
                    break;

                case D_KEYUP:
                    keyboardState[e.keyboard.key] = 0;
                    break;
            };
        };

        MWG_CalcPhysics(&map);

        MWG_ControlPlayer(&map.player[0], &mainMenu, D_NULL, keyboardState, &map);

        /* Control zoom with the i and o keys */
        if(keyboardState[D_Ki]){zoom -= 10;};
        if(keyboardState[D_Ko]){zoom += 10;};


        D_FillRect(out, D_NULL, D_rgbaToFormat(out->format, 181, 233, 255, 255));

        MWG_DrawMap(out, &map, map.player[0].x, map.player[0].y, zoom);
        MWG_DrawMenu(out, &mainMenu, fontImage, zoom);

        D_FlipOutSurf(out);

        D_Delay(DELAY);
    };

    D_StopEvents();

    D_FreeOutSurf(out);
    out = D_NULL;

    return 0;
};
