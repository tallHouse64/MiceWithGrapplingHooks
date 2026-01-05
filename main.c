#define D_IMPLEMENTATION
#include"d.h"

#define DEVENTS_IMPLEMENTATION
#include"devents.h"

#define D_PLATFORM_IMPLEMENTATION
#include"platform/sdld.h"

#define DELAY 1000/30
#define MWG_MAX_MAP_RECTS 256
#define MWG_MAX_PLAYER 128
#define MWG_PLAYER_WIDTH 50
#define MWG_PLAYER_HEIGHT 50

typedef struct MWG_Player {

    /* This x, y position is the bottom-middle of
     *  the player (like maro 64). */
    int x, y;

    int oldX, oldY;

    D_double angle;
} MWG_Player;

typedef struct MWG_MapRect {
    int x, y, w, h;

    /* Box colour */
    int r, g, b;
} MWG_MapRect;

typedef struct MWG_Map {
    MWG_MapRect rect[MWG_MAX_MAP_RECTS];
    int numRects;
    MWG_Player player[MWG_MAX_PLAYER];
    int numPlayers;
} MWG_Map;

/* This function draws a map onto a surface.
 *
 * s: The surface to draw onto.
 * map: The map to draw.
 * cameraX: The x position of the camera.
 * cameraY: The x position of the camera.
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_DrawMap(D_Surf * s, MWG_Map * map, int cameraX, int cameraY){

    if(s == D_NULL || map == D_NULL){
        return -1;
    };

    D_Rect r = {0};
    int i = 0;
    while(i < map->numRects){

        r.x = (map->rect[i].x - cameraX) + (s->w / 2);
        r.y = (map->rect[i].y - cameraY) + (s->h / 2);
        r.w = map->rect[i].w;
        r.h = map->rect[i].h;

        D_FillRect(s, &r, D_rgbaToFormat(s->format, map->rect[i].r, map->rect[i].g, map->rect[i].b, 255));

        i++;
    };

    i = 0;
    while(i < map->numPlayers){

        r.x = ((map->player[i].x - cameraX) + (s->w / 2)) - (MWG_PLAYER_WIDTH / 2);
        r.y = ((map->player[i].y - cameraY) + (s->h / 2)) - MWG_PLAYER_HEIGHT;
        r.w = MWG_PLAYER_WIDTH;
        r.h = MWG_PLAYER_HEIGHT;

        D_FillRect(s, &r, D_rgbaToFormat(s->format, 40, 40, 40, 255));

        i++;
    };

    return 0;
};

/* This function finds the entry point of a
 *  player into a rect if that player has just
 *  entered this frame. This is useful for
 *  collision resolution.
 *
 * This function assumes that the player was
 *  outside the rect last frame, but is inside
 *  this frame (otherwise there may be a division
 *  by zero and the program would crash).
 *
 * player: The player that just entered a
 *  rectangle.
 * rect: The rectangle that the player just
 *  entered.
 * xEntryPoint: A number that gets filled in with
 *  the x position of where the player entered.
 * yEntryPoint: A number that gets filled in with
 *  the y position of where the player entered.
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_FindEntryPoint(MWG_Player * player, MWG_MapRect * rect, int * xEntryPoint, int * yEntryPoint){

    if(player == D_NULL || rect == D_NULL || xEntryPoint == D_NULL || yEntryPoint == D_NULL){
        return -1;
    };

    /* Was the player to the left of the rect
     *  last frame? */
    if(player->oldX < rect->x){

        /* Find the y entry point, this may be
         *  wrong, check in the next if
         *  statement. */
        *yEntryPoint = ((((rect->x - player->oldX) * (player->y - player->oldY)) / (player->x - player->oldX))) + player->oldY;

        /* Is yEntryPoint on the border of the
         *  rect? */
        if(*yEntryPoint >= rect->y && *yEntryPoint < rect->y + rect->h){
            /* The yEntry point is correct, set
             *  xEnrtyPoint and return. */

            *xEntryPoint = rect->x;
            return 0;
        };
    };

    /* At this point, the entry point can't be on
     *  the left wall of the rect (the function
     *  would have returned). */

    /* Was the player to the right of the rect
     *  last frame? */
    if(player->oldX >= rect->x + rect->w){


        /* Find the y entry point (this time on
         *  the right wall, not left), this may
         *  be wrong, check in the next if
         *  statement. */
        *yEntryPoint = (((((rect->x + rect->w) - player->oldX) * (player->y - player->oldY)) / (player->x - player->oldX))) + player->oldY;

        /* Is yEntryPoint on the border of the
         *  rect? */
        if(*yEntryPoint >= rect->y && *yEntryPoint < rect->y + rect->h){
            /* The yEntry point is correct, set
             *  xEnrtyPoint and return. */

            *xEntryPoint = rect->x + rect->w;
            return 0;
        };
    };

    /* Now the entry point must be either the top
     *  or bottom wall. */

    /* Was the player above the rect last
     *  frame? */
    if(player->oldY < rect->y){

    };

    return 0;
};

int MWG_CalcPhysics(MWG_Map * map){

    int i = 0;
    int j = 0;
    int temp1 = 0;
    int temp2 = 0;
    int xEntryPoint = 0;
    int yEntryPoint = 0;
    while(i < map->numPlayers){

        /* Apply gravity */
        map->player[i].oldY = map->player[i].oldY - 5;

        /* Apply air resistance (halve the
         *  speed) */
        map->player[i].oldX = (map->player[i].oldX + map->player[i].x) / 2;
        map->player[i].oldY = (map->player[i].oldY + map->player[i].y) / 2;

#if 0
        /* Loop through all the map rectangles. */
        j = 0;
        xEntryPoint = 0;
        yEntryPoint = 0;
        while(j < map->numRects){

            /* Is the player in this rect?
             *  (collision detection). */
            if( map->player[i].x >= map->rect[j].x &&
                map->player[i].x <  map->rect[j].x + map->rect[j].w &&
                map->player[i].y >= map->rect[j].y &&
                map->player[i].y <  map->rect[j].y + map->rect[j].h
            ){

                /* Collision detected, now do
                 *  collision resolution. */

                /* Is the old position in the
                 *  rect? */
                if( map->player[i].oldX >= map->rect[j].x &&
                    map->player[i].oldX <  map->rect[j].x + map->rect[j].w &&
                    map->player[i].oldY >= map->rect[j].y &&
                    map->player[i].oldY <  map->rect[j].y + map->rect[j].h
                ){
                    /* For now don't bother doing
                     *  collision resolution in
                     *  this case. */

                    j++;
                    continue;
                };

                /* Was the player player to the
                 *  left of the rect last frame? */
                if(map->player[i].oldX < map->rect[j].x){
                    /* Original equation (only
                     *  works for floats and
                     *  doubles, not ints):
                     *  (((map->rect[j].x - map->player[i].oldX) / (map->player[i].x - map->player[i].oldX)) * (map->player[i].y - map->player[i].oldY)) + map->player[i].oldY
                     */
                    yEntryPoint = ((((map->rect[j].x - map->player[i].oldX) * (map->player[i].y - map->player[i].oldY)) / (map->player[i].x - map->player[i].oldX))) + map->player[i].oldY;

                    /* The yEntry point may be
                     *  wrong, check if it's on
                     *  the border of the rect.*/
                    if(yEntryPoint < map->rect[j].y){
                        /* yEntryPoint is wrong,
                         *  don't bother
                         *  correcting it, at
                         *  this point in the
                         *  logic the player must
                         *  have collided with
                         *  the top of the rect.
                         */

                        map->player[i].y = map->rect[j].y - 1;

                    }else if(yEntryPoint >= map->rect[j].y + map->rect[j].h){
                        /*yEntryPoint is wrong,
                         *  don't bother
                         *  correcting it,
                         *  the player must have
                         *  collided with the
                         *  bottom.
                         */

                        map->player[i].y = map->rect[j].y + map->rect[j].h;
                    };

                    /* Was the player to the
                     *  right of the rect last
                     *  frame? */
                }else if(map->player[i].oldX >= map->rect[j].x + map->rect[j].w){

                };

            };

            j++;
        };

#endif


        /* Move player */

        temp1 = map->player[i].x;
        temp2 = map->player[i].y;

        map->player[i].x = map->player[i].x + (map->player[i].x - map->player[i].oldX);
        map->player[i].y = map->player[i].y + (map->player[i].y - map->player[i].oldY);

        map->player[i].oldX = temp1;
        map->player[i].oldY = temp2;

        i++;
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

    MWG_Map map = {
        /* MapRects */
        {
            {
                -250, 0, 500, 40,
                17, 127, 11
            }
        },
        1, /* numRects */

        /* Players */
        {
            0, 0,
            0, 0,
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
                    break;

                case D_KEYUP:
                    keyboardState[e.keyboard.key] = 0;
                    break;
            };
        };

        if(keyboardState[D_Kw]){cameraY -= 1;};
        if(keyboardState[D_Ka]){cameraX -= 1;};
        if(keyboardState[D_Ks]){cameraY += 1;};
        if(keyboardState[D_Kd]){cameraX += 1;};

        MWG_CalcPhysics(&map);


        D_FillRect(out, D_NULL, D_rgbaToFormat(out->format, 181, 233, 255, 255));

        MWG_DrawMap(out, &map, cameraX, cameraY);

        D_FlipOutSurf(out);

        D_Delay(DELAY);
    };

    D_StopEvents();

    D_FreeOutSurf(out);
    out = D_NULL;

    return 0;
};
