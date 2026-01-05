#ifndef MWG_MAIN_H
#define MWG_MAIN_H


#define DELAY 1000/30
#define MWG_MAX_MAP_RECTS 256
#define MWG_MAX_PLAYER 128
#define MWG_PLAYER_WIDTH 50
#define MWG_PLAYER_HEIGHT 50


typedef struct MWG_Player {

    /* This x, y position is the bottom-middle of
     *  the player (like maro 64). */
    int x, y;

    int oldX, oldY;

    D_double angle;
} MWG_Player;

typedef struct MWG_MapRect {
    int x, y, w, h;

    /* Box colour */
    int r, g, b;
} MWG_MapRect;

typedef struct MWG_Map {
    MWG_MapRect rect[MWG_MAX_MAP_RECTS];
    int numRects;
    MWG_Player player[MWG_MAX_PLAYER];
    int numPlayers;
} MWG_Map;

#endif
