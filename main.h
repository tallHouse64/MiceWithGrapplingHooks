#ifndef MWG_MAIN_H
#define MWG_MAIN_H

#include"d.h"


#define DELAY 1000/30
#define MWG_MAX_MAP_RECTS 256
#define MWG_MAX_PLAYER 128
#define MWG_PLAYER_WIDTH 50
#define MWG_PLAYER_HEIGHT 50

/* This is an integer lerp macro (linear
 *  interpolation).
 *
 * When t is 256 a gets returned, when it is 0 b
 *  is returned.
 *
 * a: The first number (returned when t is 0).
 * b: The second number (returned when t is 256).
 * returns: An interpolated number between a and
 *  b.
*/
#define LERP_INT(a, b, t) ( (((a) * (256 - (t))) / 256) + (((b) * (t)) / 256) )

typedef struct MWG_Player {

    /* This x, y position is the bottom-middle of
     *  the player (like maro 64). */
    int x, y;

    int oldX, oldY;

    /* The hitbox is relative to the x, y of the
     *  player, it is recommended to set the
     *  hitboxX to -(hitboxW / 2) and set hitboxY
     *  to -hitboxH. */
    int hitboxX, hitboxY, hitboxW, hitboxH;

    D_Surf * image;

    /* The image rectangle is relative to the x,
     *  y of the player.*/
    int imageX, imageY, imageW, imageH;

    /* This is the centre of rotation for the
     *  image, it is relative to the top left of
     *  the image rectangle (which is relative to
     *  the player). */
    int rotateCentreX, rotateCentreY;

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
