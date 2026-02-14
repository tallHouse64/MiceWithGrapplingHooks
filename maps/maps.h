#ifndef MWG_MAPS_H
#define MWG_MAPS_H

#include"../main.h"


/* These flags control the qualities of a
 *  rectangle like whether you can walk through it
 *  (intangible).
 *
 * These flags can combined using bit-wise OR
 *  (|).
 */
typedef enum MWG_MapRectFlags {
    MWG_MAP_RECT_NO_FLAGS     = 0,
    MWG_MAP_RECT_INTANGIBLE   = 0x1,
    MWG_MAP_RECT_CANT_GRAPPLE = 0x2
} MWG_MapRectFlags;

typedef struct MWG_MapRect {
    int x, y, w, h;

    D_Surf * image;

    /* Box colour */
    int r, g, b;

    /* friction can be between 0 and 256 */
    int friction;

    /* These flags control how the rectangle
     *  works in game (read the above declaration
     *  of MWG_MapRectFlags). */
    MWG_MapRectFlags flags;
} MWG_MapRect;

typedef struct MWG_Map {
    MWG_MapRect rect[MWG_MAX_MAP_RECTS];
    int numRects;
    MWG_Player player[MWG_MAX_PLAYER];
    int numPlayers;
} MWG_Map;


extern const MWG_Map testMap;

extern const MWG_Map level1;


int MWG_AddMapRect(MWG_Map * map, MWG_MapRect * r);

#endif
