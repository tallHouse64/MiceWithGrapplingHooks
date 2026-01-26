#include"maps.h"
#include"../main.h"

MWG_Map testMap = {
    /* MapRects */
    {
        {
            -250, 30, 500, 40,
            D_NULL,
            17, 127, 11
        },
        {
            -200, -210, 100, 100,
            D_NULL,
            17, 127, 11
        }
    },
    2, /* numRects */

    /* Players */
    {},

    0 /* numPlayers*/
};

MWG_Map mainMenu = {
    /* MapRects */
    {
        {
            -150, -50, 300, 100,
            D_NULL,
            255, 255, 255
        },
        {
            -150, 70, 300, 100,
            D_NULL,
            255, 255, 255
        }
    },
    2, /* NumRects */

    /* Players */
    {},

    0 /* numPlayers */
};
