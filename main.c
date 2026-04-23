#define D_IMPLEMENTATION
#include"d.h"

#define DEVENTS_IMPLEMENTATION
#include"devents.h"

#define D_PLATFORM_IMPLEMENTATION

#ifdef WEB
#include"platform/webd.h"

#elif defined(NDS)
#include"platform/ndsd.h"

#else
#include"platform/sdld.h"

#endif


#ifndef NDS
/* These define statements are temporary code so
 *  the program compiles for platforms other than
 *  nds. */
#define KEY_UP 0
#define KEY_DOWN 0
#define KEY_LEFT 0
#define KEY_RIGHT 0
#define KEY_A 0
#define KEY_R 0
#define KEY_L 0
#define KEY_X 0
#define KEY_Y 0
#endif



#include"main.h"
#include"physics/physics.h"
#include"maps/maps.h"
#include"menus/menus.h"

#include"assets/mouse.h"
#include"assets/font.h"

const MWG_Player defaultProfile = {
    0, /* x */
    0, /* y */
    0, /* oldX */
    0, /* oldY */
    256, /* zoom */
    -20, /* hitBoxX */
    -20, /* hitBoxY */
    40, /* hitBoxW */
    40, /* hitBoxH */
    D_NULL, /* image */
    -mouseDataW, /* imageX */
    -mouseDataH, /* imageY */
    mouseDataW * 2, /* imageW */
    mouseDataH * 2, /* imageH */
    mouseDataW, /* rotateCentreX */
    mouseDataH, /* rotateCentreY */
    0, /* hookX */
    0, /* hookY */
    MWG_HOOK_UNATTACHED, /* hookState */
    0.0, /* angle */
    MWG_COLLISION_DIR_NONE, /* collisionDirection */
    MWG_PLAYER_NO_FLAG, /* flags */
    -1 /* drawingRect */
};


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
int MWG_DrawMap(D_Surf * s, MWG_Map * map, D_Surf * font, int cameraX, int cameraY, int zoom){

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

    /* Draw the labels */
    D_Point textPoint = {0};
    int textHeight = 0;
    int labels = MWG_MIN(map->numLabels, MWG_MAX_MAP_LABELS);
    i = 0;
    while(i < labels){

        textPoint.x = (((map->label[i].x - cameraX) * 256) / zoom) + (s->w / 2);
        textPoint.y = (((map->label[i].y - cameraY) * 256) / zoom) + (s->h / 2);
        textHeight = (map->label[i].textHeight * 256) / zoom;

        D_PrintToSurf(s, font, &textPoint, textHeight, 0, map->label[i].text);

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


        /* Is the player drawing a rectangle? */
        if(map->player[i].drawingRect >= 0){

            /* Show it onscreen */
            r.x = (((MWG_MIN(map->player[i].x, map->rect[map->player[i].drawingRect].x) - cameraX) * 256) / zoom) + (s->w / 2);
            r.y = (((MWG_MIN(map->player[i].y, map->rect[map->player[i].drawingRect].y) - cameraY) * 256) / zoom) + (s->h / 2);

            r.w = map->rect[map->player[i].drawingRect].x - map->player[i].x;
            r.h = map->rect[map->player[i].drawingRect].y - map->player[i].y;

            /* make the width and height
             *  positive */
            r.w = (MWG_MAX(r.w, -r.w) * 256) / zoom;
            r.h = (MWG_MAX(r.h, -r.h) * 256) / zoom;

            D_FillRect(s, &r, D_rgbaToFormat(s->format, 17, 127, 11, 255));
        };

        r.x = ((((map->player[i].x + map->player[i].imageX) - cameraX) * 256) / zoom) + (s->w / 2);
        r.y = ((((map->player[i].y + map->player[i].imageY) - cameraY) * 256) / zoom) + (s->h / 2);
        r.w = (map->player[i].imageW * 256) / zoom;
        r.h = (map->player[i].imageH * 256) / zoom;

        centre.x = (map->player[i].rotateCentreX * 256) / zoom;
        centre.y = (map->player[i].rotateCentreY * 256) / zoom;


        /* Is the player looking right? */
        if(map->player[i].angle <= 90 && map->player[i].angle >= -90){

            /* Draw the player looking right */
            D_SurfCopyScaleRot(map->player[i].image, D_NULL, s, &r, &centre, map->player[i].angle, 0, 0);
        }else{

            /* Draw the player looking left */
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
 * You can pass null for gameState and the
 *  function would work normally but buttons that
 *  rely on values in the gameState won't show
 *  any text (like frame rate).
 *
 * s: The surface to draw on.
 * menu: The menu to draw.
 * font: The font to draw text with (see the
 *  documentation for D_PrintToSurf()).
 * zoom: How much to zoom (256 for no zoom
 *  effect).
 * gameState: The state of the game (read above).
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_DrawMenu(D_Surf * s, MWG_Menu * menu, D_Surf * font, int zoom, MWG_GameState * gameState){

    int i = 0;
    D_Rect r = {0};
    D_Point textPoint = {0};
    char textBuf[16] = "\0";

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

        /* Draw the button (use hover colours if
         *  the button is being hovered over). */
        if(i == menu->hoveredButton){
            D_FillRect(s, &r, D_rgbaToFormat(s->format, menu->button[i].hovR, menu->button[i].hovG, menu->button[i].hovB, 255));
        }else{
            D_FillRect(s, &r, D_rgbaToFormat(s->format, menu->button[i].r, menu->button[i].g, menu->button[i].b, 255));
        };

        /* Draw the text */
        textPoint.x = r.x;
        textPoint.y = r.y;

        switch(menu->button[i].replacementText){
            case MWG_BUTTON_TEXT_ORIGINAL:
                D_PrintToSurf(s, font, &textPoint, r.h, 0, menu->button[i].text);
                break;

            case MWG_BUTTON_TEXT_FPS:
                /* Write the FPS instead */

                /* The gameState is needed to
                 *  know the frame rate */
                if(gameState == D_NULL){
                    /* gameState unavailable, so
                     *  don't draw any text */
                    break;
                };

                /* The following 4 lines are a
                 *  hack because sprintf is not
                 *  available at the moment and
                 *  should be replaced when it
                 *  is. The code converts
                 *  gameState->frameRate to a
                 *  string. */
                textBuf[3] = '\0';
                textBuf[2] = ( gameState->frameRate        % 10) + 48;
                textBuf[1] = ((gameState->frameRate / 10 ) % 10) + 48;
                textBuf[0] = ((gameState->frameRate / 100) % 10) + 48;

                D_PrintToSurf(s, font, &textPoint, r.h, 0, textBuf);
                break;

            default:
                /* This shouldn't run, if it does
                 *  it means the button has an
                 *  invalid replacement text
                 *  value and will be treated as
                 *  if it was
                 *  MWG_BUTTON_TEXT_ORIGINAL.*/
                D_PrintToSurf(s, font, &textPoint, r.h, 0, menu->button[i].text);
                break;
        };
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
     *  1 degree. */
    const D_double pR = 0.999847695156;
    const D_double pC = 0.0174524064373;

    /* Note that this pRSmall and pCSmall rotate
     *  a point by 0.01 degrees, not 1 degree. */
    const D_double pRSmall = 0.999999984769;
    const D_double pCSmall = 0.000174532924313;

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
    while(reps < (angle2 - 0.99)){

        /* Add 1 degree to degR and degC */
        temp = D_COMPLEXMULTR(hookDirectionX, hookDirectionY, pR, pC);
        hookDirectionY = D_COMPLEXMULTC(hookDirectionX, hookDirectionY, pR, pC);
        hookDirectionX = temp;

        reps += 1;
    };

    while(reps < angle2){

        /* Add 0.01 degree to degR and degC */
        temp = D_COMPLEXMULTR(hookDirectionX, hookDirectionY, pRSmall, pCSmall);
        hookDirectionY = D_COMPLEXMULTC(hookDirectionX, hookDirectionY, pRSmall, pCSmall);
        hookDirectionX = temp;

        reps += 0.01;
    };

    /* Convert hook direction to absolute
     *  coordinates. */
    hookDirectionX = hookDirectionX + player->x;
    hookDirectionY = hookDirectionY + player->y;


    /* The algorithm below looks through each
     *  rectangle and checks if they are being
     *  aimed at, one by one. If one is being
     *  aimed at, then it is checked if it is
     *  closer than the last rectangle being
     *  aimed at. If it is closer then
     *  closestHookX and closestHookY are set.
     *  Think of it like king of the hill, the
     *  closest one to the player wins. */
    int i = 0;
    int grappledRect = -1;
    int tempHookX = 0;
    int tempHookY = 0;
    int closestHookX = 0;
    int closestHookY = 0;
    int foundAHookPosition = 0;
    for(; i < map->numRects; i++){

        /* Skip rectangles that are intangible
         *  and that can't be grappled. That way
         *  grappling hooks can shoot through
         *  them. */
        if((map->rect[i].flags & MWG_MAP_RECT_INTANGIBLE) && (map->rect[i].flags & MWG_MAP_RECT_CANT_GRAPPLE)){
            continue;
        };

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
                closestHookX = tempHookX;
                closestHookY = tempHookY;
                grappledRect = i;
            };
        };
    };

    if(foundAHookPosition){

        /* If the grappling hook attached to a
         *  rectangle that can't be grappled. */
        if(map->rect[grappledRect].flags & MWG_MAP_RECT_CANT_GRAPPLE){

            /* In this case just do nothing */

        }else if(foundAHookPosition){

            player->hookX = closestHookX;
            player->hookY = closestHookY;
            player->hookState = MWG_HOOK_ATTACHED;
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
 * If a menu is shown and the player activates a
 *  button which changes the menu (for example
 *  mainMenu to levelMenu), then the menu passed
 *  into the function gets overwritten with a new
 *  menu. The same is true if the button changes
 *  the map, the map that was passed into the
 *  function gets overwritten.
 *
 * It is safe to pass null for all the
 *  parameters. If map is null, the player won't
 *  be able to shoot a grappling hook. If "p" is
 *  null, the function would do nothing and
 *  return -1.
 *
 * The frame rate passed in is used for physics
 *  so that players jump and move at the same
 *  speed regardless of the frame rate.
 *
 * p: The player to control.
 * menu: The menu to control (if there is one).
 * e: An input event.
 * keyboardState: An array of uint8s that is the
 *  length of D_NumKeys.
 * map: The map the player is in (needed when the
 *  player fires a grappling hook).
 * frameRate: The number of frames per second
 *  that the game is running at.
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_ControlPlayer(MWG_Player * p, D_Event * e, D_uint8 * keyboardState, MWG_Map * map, int frameRate){

    /* Keys held on a DS (key state) */
    D_uint32 keys = 0;

    /* Keys down on a DS (went down this frame) */
    D_uint32 keysPressed = 0;

#ifdef NDS
    /* This is a hack to get input working on a
     *  DS and should be removed after drws-lib
     *  gets controller support. */
    keys = keysHeld();
    keysPressed = keysDown();
#endif

    /* The code below only handles player input
     *  on a map (not a menu). This is why it
     *  returns -1 if p is null. */

    if(p == D_NULL){
        return -1;
    };

    if(e != D_NULL){
        if(e->type == D_KEYDOWN){
            if((e->keyboard.key == D_KSpace || (keysPressed & KEY_A)) &&
                ((p->collisionDirection & MWG_COLLISION_DIR_DOWN) || (p->flags & MWG_PLAYER_HAS_WINGS))){

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
    if(keyboardState[D_Ka] || (keys & KEY_LEFT)){
        p->oldX += (78 * (1000 / frameRate)) / 256;

        /* Is the player looking right? */
        if(p->angle <= 90 && p->angle >= -90){

            /*Flip the player to look left*/
            p->angle = 180 - p->angle;
        };
    };

    /* When d is pressed move right */
    if(keyboardState[D_Kd] || (keys & KEY_RIGHT)){
        p->oldX -= (78 * (1000 / frameRate)) / 256;

        /* Is the player looking left? */
        if(p->angle > 90 || p->angle < -90){

            /*Flip the player to look right*/
            p->angle = 180 - p->angle;
        };
    };

    /* When e is pressed change the player's
     *  angle */
    if(keyboardState[D_Ke] || (keys & KEY_R)){

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
    if(keyboardState[D_Kq] || (keys & KEY_L)){

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

    if(keyboardState[D_Kw] || (keys & KEY_UP)){

        /* If the player is flying, move up */
        if(p->flags & MWG_PLAYER_FLYING){
            p->oldY += (78 * (1000 / frameRate)) / 256;

        }else if(map != D_NULL){

            /* Otherwise fire the grappling hook,
             *  if map is safe to use. */
            MWG_FireHook(map, p);
        };
    };

    if(keyboardState[D_Ks] || (keys & KEY_DOWN)){

        /* If the player is flying, move down */
        if(p->flags & MWG_PLAYER_FLYING){
            p->oldY -= (78 * (1000 / frameRate)) / 256;

        }else{

            /* Otherwise release the grappling
             *  hook */
            p->hookState = MWG_HOOK_UNATTACHED;
        };
    };

    /* If the player is flying, move up */
    if((keyboardState[D_KSpace] || (keys & KEY_A)) && p->flags & MWG_PLAYER_FLYING){
        p->oldY += (78 * (1000 / frameRate)) / 256;
    };

    return 0;
};

/* This function adds a player to a map (it
 *  copies the structure to the next unused
 *  element).
 *
 * If the map is full, the function would do
 *  nothing and return -2.
 *
 * It is safe to pass null for all the
 *  parameters, the function would do nothing and
 *  return -1.
 *
 * map: The map to add a player to.
 * player: The player structure to copy onto the
 *  map.
 * returns: The index of the player added on
 *  success or a negative number on failure.
 */
int MWG_AddPlayer(MWG_Map * map, MWG_Player * player){

    int i = 0;

    if(map == D_NULL || player == D_NULL){
        return -1;
    };

    if(map->numPlayers >= MWG_MAX_PLAYER){
        return -2;
    };

    i = map->numPlayers;

    map->player[i] = *player;

    map->numPlayers = map->numPlayers + 1;

    return i;
};

/* This function gets a profile image for any
 *  local player.
 *
 * All this function does is write the correct
 *  width, height, pixel format, pixel data
 *  pointer and other information to a surface
 *  structure so you don't have to. This should
 *  make bugs less likely to be written.
 *
 * To use this function you declare a surface,
 *  and pass it to this function. Don't allocate
 *  memory for the pixel data. Also don't use
 *  D_CreateSurf().
 *
 * Example:
 *  D_Surf img = {D_NULL};
 *  MWG_GetProfileImage(&img, &gameState);
 *
 * After running this function you can draw to
 *  the surface either directly to pixel data or
 *  call any drws-lib function to draw for you.
 *
 * Because the surface was declared as a local
 *  variable, it gets put on the stack. This
 *  means you don't have to free it. You can use
 *  malloc() but this is not recommended. Every
 *  time you need this surface just run the
 *  function again.
 *
 * If MWGH has not allocated a buffer for the
 *  profile's image data, the function does
 *  nothing and returns -3. This may happen on
 *  platforms with little memory like GBA. This
 *  means the player always appears as the
 *  default mouse image and the image can't be
 *  personalized.
 *
 * If the number passed in for "which" is
 *  invalid, the function does nothing and
 *  returns -2.
 *
 * It is safe to pass null for s and state, the
 *  function would do nothing and return -1.
 *
 * If the game state says the image is larger
 *  than the maximum or smaller than the minimum
 *  size the function does nothing and returns
 *  -4.
 *
 * s: A surface structure to point to a profile's
 *  image data.
 * which: Which local profile to get the surface
 *  of (for now there is only one, the 0th one,
 *  so just pass 0).
 * state: The current game state.
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_GetProfileImage(D_Surf * s, int which, MWG_GameState * state){

    if(s == D_NULL || state == D_NULL){
        return -1;
    };

    /* For now there is only one profile, the 0th
     *  one. */
    if(which != 0){
        return -2;
    };

    if(state->profile1ImageData == D_NULL){
        return -3;
    };

    /* If the game state says the image is too
     *  big or too small */
    if( state->profile1ImageWidth > MWG_PLAYER_IMAGE_MAX_WIDTH ||
        state->profile1ImageHeight > MWG_PLAYER_IMAGE_MAX_HEIGHT ||
        state->profile1ImageWidth < 1 ||
        state->profile1ImageHeight < 1
    ){
        return -4;
    };


    s->pix = state->profile1ImageData;

    s->w = state->profile1ImageWidth;
    s->h = state->profile1ImageHeight;

    s->pitch = (MWG_PLAYER_IMAGE_MAX_WIDTH - s->w) * (MWG_PLAYER_IMAGE_BIT_DEPTH / 8);

    s->safeArea.x = 0;
    s->safeArea.y = 0;
    s->safeArea.w = s->w;
    s->safeArea.h = s->h;

    s->outId = -1;
    s->alphaMod = 255;
    s->blendMode = D_BLENDMODE_NORMAL;
    s->flags = 0;
    s->outSurfFlags = 0;
    s->format = MWG_PLAYER_IMAGE_FORMAT;

    return 0;
};

int main(int argc, char ** argv){

    MWG_GameState gameState = {D_NULL};

    gameState.out = D_GetOutSurf(50, 50, 640, 480, "Mice With Grappling Hooks", D_OUTSURFRESIZABLE);
    gameState.running = 1;
    gameState.fontImage = D_CreateSurfFrom(fontDataW, fontDataH, 0, D_NULL, D_FindPixFormat(0xFF, 0xFF00, 0xFF0000, 0xFF000000, 32), fontData);
    gameState.player1Index = -1;
    gameState.frameRate = 30;

    gameState.imageData = D_CALLOC(MWG_MAX_PLAYER, MWG_PLAYER_IMAGE_MAX_WIDTH * MWG_PLAYER_IMAGE_MAX_HEIGHT * (MWG_PLAYER_IMAGE_BIT_DEPTH / 8));

    /* Keys held for DS */
    D_uint32 keys = 0;

    MWG_Menu currentMenu = mainMenu;
    gameState.menu = &currentMenu;

    MWG_Map currentMap = testMap;
    gameState.map = &currentMap;

    D_Surf * mouseImage = D_CreateSurfFrom(mouseDataW, mouseDataH, 0, D_NULL, D_FindPixFormat(0xFF, 0xFF00, 0xFF0000, 0xFF000000, 32), mouseData);
    D_Event e = {0};


    /* Setup profile 1 */
    gameState.profile1 = defaultProfile;
    gameState.profile1.image = mouseImage;
    gameState.profile1ImageData = D_CALLOC(1, MWG_PLAYER_IMAGE_MAX_WIDTH * MWG_PLAYER_IMAGE_MAX_HEIGHT * (MWG_PLAYER_IMAGE_BIT_DEPTH / 8));

    gameState.profile1ImageWidth = mouseImage->w;
    gameState.profile1ImageHeight = mouseImage->h;

    /* Make sure the buffer is not null (this may
     *  be null on platforms with little memory)
     */
    if(gameState.profile1ImageData != D_NULL){
        D_Surf profile1Image = {D_NULL};
        MWG_GetProfileImage(&profile1Image, 0, &gameState);
        D_SurfCopyScale(mouseImage, D_NULL, &profile1Image, D_NULL);
    };


#ifdef NDS
    gameState.player1.zoom = 512;
#endif


    D_StartEvents();

    while(gameState.running){
        D_PumpEvents();

        while(D_GetEvent(&e) != -1){

            /* Only send input events to the
             *  player if there is no menu
             *  onscreen */
            if(gameState.menu->numButtons <= 0){
                MWG_ControlPlayer(&gameState.map->player[gameState.player1Index], &e, D_NULL, gameState.map, gameState.frameRate);
            };

            MWG_ControlMenu(gameState.menu, &e, &gameState.profile1, &gameState.player1Index, &gameState);

            switch(e.type){
                case D_QUIT:
                    gameState.running = 0;
                    break;

                case D_KEYDOWN:
                    gameState.keyboardState[e.keyboard.key] = 1;
                    break;

                case D_KEYUP:
                    gameState.keyboardState[e.keyboard.key] = 0;
                    break;

                case D_OUTSURFRESIZE:
                    gameState.out = D_GetResizedOutSurf(gameState.out);
                    break;
            };
        };

        MWG_CalcPhysics(gameState.map, &gameState);


#ifdef NDS
        /* This is a hack to get controller input
         *  working and should be removed after
         *  drws-lib gets controller support. */
        if(gameState.menu->numButtons <= 0 && gameState.player1Index >= 0){
            MWG_ControlPlayer(&gameState.map->player[gameState.player1Index], &e, D_NULL, gameState.map, gameState.frameRate);
        };

        MWG_ControlMenu(gameState.menu, &e, &gameState.player1, &gameState.player1Index, &gameState);

        keys = keysHeld();
#endif

        /* Control zoom with the i and o keys */
        if(gameState.keyboardState[D_Ki] || (keys & KEY_X)){
            gameState.profile1.zoom -= 10;

            /* Immediately copy the profile's
             *  zoom to the player's zoom */
            gameState.map->player[gameState.player1Index].zoom = gameState.profile1.zoom;
        };

        if(gameState.keyboardState[D_Ko] || (keys & KEY_Y)){
            gameState.profile1.zoom += 10;

            /* Immediately copy the profile's
             *  zoom to the player's zoom */
            gameState.map->player[gameState.player1Index].zoom = gameState.profile1.zoom;
        };


        D_FillRect(gameState.out, D_NULL, D_rgbaToFormat(gameState.out->format, 181, 233, 255, 255));



        /* If player1 is loaded (copied) into the
         *  map. */
        if(gameState.player1Index >= 0){

            /* Only send input events to the
             *  player if there is no menu
             *  onscreen */
            if(gameState.menu->numButtons <= 0){
                MWG_ControlPlayer(&gameState.map->player[gameState.player1Index], D_NULL, gameState.keyboardState, gameState.map, gameState.frameRate);
            };

            MWG_DrawMap(gameState.out, gameState.map, gameState.fontImage, gameState.map->player[gameState.player1Index].x, gameState.map->player[gameState.player1Index].y, gameState.map->player[gameState.player1Index].zoom);


            MWG_DrawMenu(gameState.out, gameState.menu, gameState.fontImage, gameState.map->player[gameState.player1Index].zoom, &gameState);
        }else{
            /* In case there is no player loaded
             *  into the map. */

            MWG_DrawMenu(gameState.out, gameState.menu, gameState.fontImage, gameState.profile1.zoom, &gameState);
        };

        D_FlipOutSurf(gameState.out);

        D_Delay(1000 / gameState.frameRate);
    };

    D_StopEvents();

    D_FreeOutSurf(gameState.out);
    gameState.out = D_NULL;

    return 0;
};
