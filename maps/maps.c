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
