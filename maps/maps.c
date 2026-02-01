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

MWG_Map level1 = {
    /* MapRects */
    {
        {
            -250, 40, 400, 70,
            D_NULL,
            17, 127, 11
        },
        {
            -250, -290, 70, 400,
            D_NULL,
            17, 127, 11
        },
        {
            -250, -290, 400, 70,
            D_NULL,
            17, 127, 11
        }
    },
    3, /* numRects */

    /* Players */
    {},

    0 /* numPlayers*/
};
