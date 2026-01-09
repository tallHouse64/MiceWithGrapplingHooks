#define D_IMPLEMENTATION
#include"d.h"

#define DEVENTS_IMPLEMENTATION
#include"devents.h"

#define D_PLATFORM_IMPLEMENTATION
#include"platform/sdld.h"

#include"main.h"
#include"physics/physics.h"


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
            100, 0,
            0, 0,
            -(MWG_PLAYER_WIDTH / 2), -MWG_PLAYER_HEIGHT, MWG_PLAYER_WIDTH, MWG_PLAYER_HEIGHT,
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

        /*if(keyboardState[D_Kw]){};*/
        if(keyboardState[D_Ka]){map.player[0].oldX += 10;};
        /*if(keyboardState[D_Ks]){};*/
        if(keyboardState[D_Kd]){map.player[0].oldX -= 10;};


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
