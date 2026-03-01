#include"maps.h"
#include"../main.h"

const MWG_Map testMap = {
    /* MapRects */
    {
        {
            -250, 30, 500, 40,
            D_NULL,
            17, 127, 11,
            8,
            0, 0, /* tpX, tpY */
        },
        {
            -200, -210, 100, 100,
            D_NULL,
            17, 127, 11,
            8,
            0, 0, /* tpX, tpY */
        },
        {
            200, -100, 100, 100,
            D_NULL,
            8, 63, 5,
            /*Could use this colour 17, 80, 43*/
            8,
            0, 0, /* tpX, tpY */
            MWG_MAP_RECT_INTANGIBLE
        },
        {
            -200, 150, 100, 100,
            D_NULL,
            57, 148, 52,
            /*Could use this colour 14, 131, 52*/
            8,
            0, 0, /* tpX, tpY */
            MWG_MAP_RECT_CANT_GRAPPLE
        },
        {
            100, 150, 100, 100,
            D_NULL,
            85, 85, 85,
            8,
            0, 0, /* tpX, tpY */
            MWG_MAP_RECT_CANT_GRAPPLE |
            MWG_MAP_RECT_INTANGIBLE
        },
        {
            400, 100, 100, 100,
            D_NULL,
            237, 59, 53, /* 240, 84, 79 */
            8,
            50, -50, /* tpX, tpY */
            MWG_MAP_RECT_TELEPORT
        },
        {
            -300, -30, 50, 50,
            D_NULL,
            255, 255, 255,
            8,
            50, -50, /* tpX, tpY */
            MWG_MAP_RECT_NO_FLAGS,

            /* Enable player flags */
            MWG_PLAYER_HAS_WINGS,

            /* Disable player flags */
            MWG_PLAYER_NO_FLAG
        },
        {
            200, -30, 50, 50,
            D_NULL,
            0, 0, 0,
            8,
            50, -50, /* tpX, tpY */
            MWG_MAP_RECT_NO_FLAGS,

            /* Enable player flags */
            MWG_PLAYER_NO_FLAG,

            /* Disable player flags */
            MWG_PLAYER_HAS_WINGS
        }
    },
    8, /* numRects */

    /* labels */
    {
        0, 0, /* x, y */
        20, /* Text height*/
        "Test",
        4 /* textLen */
    },
    1, /* numLabels */

    /* Players */
    {},

    0 /* numPlayers*/
};

const MWG_Map level1 = {
    /* MapRects */
    {
        {
            -250, 40, 400, 70,
            D_NULL,
            17, 127, 11,
            256,
            0, 0, /* tpX, tpY */
        },
        {
            -250, -290, 70, 400,
            D_NULL,
            17, 127, 11,
            256,
            0, 0, /* tpX, tpY */
        },
        {
            -250, -290, 400, 70,
            D_NULL,
            17, 127, 11,
            256,
            0, 0, /* tpX, tpY */
        }
    },
    3, /* numRects */

    /* labels */
    {},
    0, /* numLabels */

    /* Players */
    {},

    0 /* numPlayers*/
};

const MWG_Map level2 = {
    {
    { /* 0 */
        -60, 20, 160, 40,
        D_NULL,
        17, 127, 11,
        256,
        0, 0, /* tpX, tpY */
    },
    { /* 1 */
        100, -140, 1120, 40,
        D_NULL,
        17, 127, 11,
        256,
        0, 0, /* tpX, tpY */
    },
    { /* 2 */
        300, -340, 1100, 40,
        D_NULL,
        17, 127, 11,
        256,
        0, 0, /* tpX, tpY */
    },
    { /* 3 */
        100, -540, 1100, 40,
        D_NULL,
        17, 127, 11,
        256,
        0, 0, /* tpX, tpY */
    },
    { /* 4 */
        300, -740, 1100, 40,
        D_NULL,
        17, 127, 11,
        256,
        0, 0, /* tpX, tpY */
    },
    { /* 5 */
        100, -940, 1100, 40,
        D_NULL,
        17, 127, 11,
        256,
        0, 0, /* tpX, tpY */
    },
    { /* 6 */
        80, -940, 40, 840,
        D_NULL,
        17, 127, 11,
        256,
        0, 0, /* tpX, tpY */
    },
    { /* 7 */
        1380, -940, 40, 960,
        D_NULL,
        17, 127, 11,
        256,
        0, 0, /* tpX, tpY */
    },
    { /* 8 */
        1340, -1220, 120, 40,
        D_NULL,
        17, 127, 11,
        256,
        0, 0, /* tpX, tpY */
    },
    { /* 9 */
        -380, -740, 460, 40,
        D_NULL,
        237, 59, 53,
        256,
        0, 0, /* tpX, tpY */
        MWG_MAP_RECT_TELEPORT |
        MWG_MAP_RECT_CANT_GRAPPLE
    },
    { /* 10 */
        120, -160, 1020, 20,
        D_NULL,
        237, 59, 53,
        256,
        0, 0, /* tpX, tpY */
        MWG_MAP_RECT_TELEPORT
    },
    { /* 11 */
        360, -360, 1020, 20,
        D_NULL,
        237, 59, 53,
        256,
        0, 0, /* tpX, tpY */
        MWG_MAP_RECT_TELEPORT
    },
    { /* 12 */
        120, -560, 1020, 20,
        D_NULL,
        237, 59, 53,
        256,
        0, 0, /* tpX, tpY */
        MWG_MAP_RECT_TELEPORT
    },
    { /* 13 */
        360, -760, 1020, 20,
        D_NULL,
        237, 59, 53,
        256,
        0, 0, /* tpX, tpY */
        MWG_MAP_RECT_TELEPORT
    },
    { /* 14 */
        -2100, 1040, 5000, 760,
        D_NULL,
        181, 233, 255,
        256,
        0, 0, /* tpX, tpY */
        MWG_MAP_RECT_TELEPORT |
        MWG_MAP_RECT_CANT_GRAPPLE
    }
    },
    15, /* numRects */

    /* labels */
    {
    {
        580, -940, /* x, y */
        20, /* Text height*/
#ifdef NDS
        "Hold X or Y to",
#else
        "Hold i or o to",
#endif
        14 /* textLen */
    },
    {
        880, -940, /* x, y */
        20, /* Text height*/
        "zoom in or out",
        14 /* textLen */
    },
    {
        120, -140, /* x, y */
        20, /* Text height*/
        "Hold",
        9 /* textLen */
    },
    {
        220, -140, /* x, y */
        20, /* Text height*/
#ifdef NDS
        "Up to zipline",
        13 /* textLen */
#else
        "W to zipline",
        12 /* textLen */
#endif
    }
    },
    4, /* numLabels */

    /* Players */
    {},

    0 /* numPlayers*/
};

const MWG_Map level3 = {
    /* MapRects */
    {
    {
        -250, 40, 400, 70, /* x, y, w, h */
        D_NULL, /* Image */
        17, 127, 11, /* Colour*/
        256, /* friction */
        0, 0, /* tpX, tpY */
        MWG_MAP_RECT_NO_FLAGS /* Flags */
    }
    },
    1, /* numRects */

    /* labels */
    {},
    0, /* numLabels */

    /* Players */
    {},

    0 /* numPlayers*/
};

/* This function adds a rectangle to a map.
 *
 * The rectangle gets copied into the map so it
 *  can be safely freed after calling the
 *  function.
 *
 * If the map is full the function would do
 *  nothing and return -2.
 *
 * It is safe to pass null for "map" and "r", the
 *  function would do nothing and return -1.
 *
 * map: The map to copy the rectangle into.
 * r: The rectangle to copy.
 * returns: The index of the new rectangle in
 *  "map" on success or a negative number on
 *  failure.
 */
int MWG_AddMapRect(MWG_Map * map, MWG_MapRect * r){

    int i = 0;

    if(map == D_NULL || r == D_NULL){
        return -1;
    };

    i = map->numRects;

    /* Is the map full? */
    if(i >= MWG_MAX_MAP_RECTS){
        return -2;
    };

    map->rect[i] = *r;

    map->numRects = map->numRects + 1;

    return i;
};
