#include"maps.h"
#include"../main.h"

MWG_Map testMap = {
    /* MapRects */
    {
        {
            -250, 30, 500, 40,
            D_NULL,
            17, 127, 11,
            8
        },
        {
            -200, -210, 100, 100,
            D_NULL,
            17, 127, 11,
            8
        },
        {
            200, -100, 100, 100,
            D_NULL,
            8, 63, 5,
            8,
            MWG_MAP_RECT_INTANGIBLE
        }
    },
    3, /* numRects */

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
            17, 127, 11,
            256
        },
        {
            -250, -290, 70, 400,
            D_NULL,
            17, 127, 11,
            256
        },
        {
            -250, -290, 400, 70,
            D_NULL,
            17, 127, 11,
            256
        }
    },
    3, /* numRects */

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
