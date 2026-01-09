#define D_IMPLEMENTATION
#include"../d.h"

#define DEVENTS_IMPLEMENTATION
#include"../devents.h"

#define D_PLATFORM_IMPLEMENTATION
#include"../platform/sdld.h"

#include"../main.h"
#include"../physics/physics.h"

#define DELAY 1000/30

/* This file tests the MWG_FindEntryPoint()
 *  function.
 *
 * Click and hold the left mouse button to move
 *  the player's old position (red), hold the
 *  right mouse button to move the player's
 *  current position (green). The entry point
 *  (white) should be between them on the border
 *  of the rectangle.
 */

int main(int argc, char ** argv){

    D_Surf * out = D_GetOutSurf(50, 50, 640, 480, "MWG_FindEntryPoint() Test", 0);
    D_Event e = {0};
    MWG_Player player = {100, 80, 40, 110};
    D_Rect rect = {70, 70, 100, 100};
    D_Rect entryPointR = {0, 0, 8, 8};
    D_Rect playerPosR = {0, 0, 8, 8};
    D_Rect oldPlayerPosR = {0, 0, 8, 8};
    MWG_MapRect mapRect = {rect.x, rect.y, rect.w, rect.h};
    int running = 1;

    D_StartEvents();

    while(running){
        D_PumpEvents();

        while(D_GetEvent(&e) != -1){
            switch(e.type){

                case D_MOUSEMOVE:

                    if(e.mouse.button & D_LEFTBUTTON){
                        player.oldX = e.mouse.x;
                        player.oldY = e.mouse.y;
                    };

                    if(e.mouse.button & D_RIGHTBUTTON){
                        player.x = e.mouse.x;
                        player.y = e.mouse.y;
                    };

                    break;

                case D_QUIT:
                    running = 0;
                    break;
            };
        };

        /* Draw sky */
        D_FillRect(out, D_NULL, D_rgbaToFormat(out->format, 181, 233, 255, 255));

        /* Draw box */
        D_FillRect(out, &rect, D_rgbaToFormat(out->format, 17, 127, 11, 255));

        /* Test function */
        MWG_FindEntryPoint(player.x, player.y, player.oldX, player.oldY, &mapRect, &entryPointR.x, &entryPointR.y);
        entryPointR.x -= 4;
        entryPointR.y -= 4;

        /* Draw entry point */
        D_FillRect(out, &entryPointR, D_rgbaToFormat(out->format, 255, 255, 255, 255));

        playerPosR.x = player.x - 4;
        playerPosR.y = player.y - 4;
        D_FillRect(out, &playerPosR, D_rgbaToFormat(out->format, 255, 0, 0, 255));

        oldPlayerPosR.x = player.oldX - 4;
        oldPlayerPosR.y = player.oldY - 4;
        D_FillRect(out, &oldPlayerPosR, D_rgbaToFormat(out->format, 0, 255, 0, 255));


        D_FlipOutSurf(out);

        D_Delay(DELAY);
    };

    D_StopEvents();

    D_FreeOutSurf(out);
    out = D_NULL;

    return 0;
};
