#ifndef MWG_MAPS_H
#define MWG_MAPS_H

#include"../main.h"


typedef struct MWG_MapRect {
    int x, y, w, h;

    D_Surf * image;

    /* Box colour */
    int r, g, b;

    /* friction can be between 0 and 256 */
    int friction;
} MWG_MapRect;

typedef struct MWG_Map {
    MWG_MapRect rect[MWG_MAX_MAP_RECTS];
    int numRects;
    MWG_Player player[MWG_MAX_PLAYER];
    int numPlayers;
} MWG_Map;


extern MWG_Map testMap;

extern MWG_Map level1;


int MWG_AddMapRect(MWG_Map * map, MWG_MapRect * r);

#endif
