#define D_IMPLEMENTATION
#include"../d.h"

#define DEVENTS_IMPLEMENTATION
#include"../devents.h"

#define D_PLATFORM_IMPLEMENTATION
#include"../platform/sdld.h"

#include"../main.h"
#include"../physics/physics.h"

#define DELAY 1000/30

int main(int argc, char ** argv){

    D_Surf * out = D_GetOutSurf(50, 50, 640, 480, "MWG_DetectCollision() Test", 0);
    D_Event e = {0};
    MWG_Player player = {
        90, 100, /* x, y*/
        90, 100, /* oldX, oldY */
        -35, -70, 70, 70, /* Hitbox */
        0.0 /* Angle */
    };
    MWG_MapRect mapRect = {250, 100, 70, 70};
    D_Rect tempRect = {0};
    D_uint32 col = 0; /* Colour of the player */
    int running = 1;

    D_StartEvents();

    while(running){
        D_PumpEvents();

        while(D_GetEvent(&e) != -1){
            switch(e.type){
                case D_QUIT:
                    running = 0;
                    break;

                case D_MOUSEMOVE:
                    if(e.mouse.button & D_LEFTBUTTON){
                        player.x = e.mouse.x;
                        player.y = e.mouse.y;
                    };
                    break;
            };
        };

        /* Clear the window */
        D_FillRect(out, D_NULL, D_rgbaToFormat(out->format, 181, 233, 255, 255));

        if(MWG_DetectCollision(&player, &mapRect)){
            col = D_rgbaToFormat(out->format, 255, 0, 0, 255);
        }else{
            col = D_rgbaToFormat(out->format, 0, 255, 0, 255);
        };

        /* Draw the mapRect */
        tempRect.x = mapRect.x; tempRect.y = mapRect.y;
        tempRect.w = mapRect.w; tempRect.h = mapRect.h;
        D_FillRect(out, &tempRect, D_rgbaToFormat(out->format, 17, 127, 11, 255));


        /* Draw the player's hitbox */
        tempRect.x = player.x + player.hitboxX; tempRect.y = player.y + player.hitboxY;
        tempRect.w = player.hitboxW; tempRect.h = player.hitboxH;
        D_FillRect(out, &tempRect, col);

        D_FlipOutSurf(out);

        D_Delay(DELAY);
    };

    D_StopEvents();

    D_FreeOutSurf(out);
    out = D_NULL;

    return 0;
};
