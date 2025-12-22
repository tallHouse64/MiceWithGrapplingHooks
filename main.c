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
