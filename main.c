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
    while(i < map->numPlayers){

        r.x = ((((map->player[i].x + map->player[i].imageX) - cameraX) * 256) / zoom) + (s->w / 2);
        r.y = ((((map->player[i].y + map->player[i].imageY) - cameraY) * 256) / zoom) + (s->h / 2);
        r.w = (map->player[i].imageW * 256) / zoom;
        r.h = (map->player[i].imageH * 256) / zoom;

        centre.x = (map->player[i].rotateCentreX * 256) / zoom;
        centre.y = (map->player[i].rotateCentreY * 256) / zoom;
        D_SurfCopyScaleRot(map->player[i].image, D_NULL, s, &r, &centre, map->player[i].angle, 0, 0);

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
            100, 0,
            0, 0,
            -(MWG_PLAYER_WIDTH / 2), -MWG_PLAYER_HEIGHT, MWG_PLAYER_WIDTH, MWG_PLAYER_HEIGHT,
            mouseImage,
            -((mouseDataW * 2) / 2), -(mouseDataH * 2), (mouseDataW * 2), (mouseDataH * 2), /*Image x, y, w, h*/
            (mouseDataW * 2) / 2, (mouseDataH * 2) / 2,
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
        if(keyboardState[D_Ka]){map.player[0].oldX += (78 * DELAY) / 256;};
        /*if(keyboardState[D_Ks]){};*/
        if(keyboardState[D_Kd]){map.player[0].oldX -= (78 * DELAY) / 256;};

        /* Control zoom with the i and o keys */
        if(keyboardState[D_Ki]){zoom -= 10;};
        if(keyboardState[D_Ko]){zoom += 10;};

        if(keyboardState[D_Ke]){map.player[0].angle += 16;};
        if(keyboardState[D_Kq]){map.player[0].angle -= 16;};


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
