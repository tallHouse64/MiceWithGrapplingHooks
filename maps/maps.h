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
    MWG_MAP_RECT_CANT_GRAPPLE = 0x2,
    MWG_MAP_RECT_TELEPORT     = 0x4
} MWG_MapRectFlags;

typedef struct MWG_MapRect {
    int x, y, w, h;

    D_Surf * image;

    /* Box colour */
    int r, g, b;

    /* friction can be between 0 and 256 */
    int friction;

    /* If the teleport flag is set then players
     *  that touch this rectangle teleport to
     *  this position. */
    int tpX, tpY;

    /* These flags control how the rectangle
     *  works in game (read the above declaration
     *  of MWG_MapRectFlags). */
    MWG_MapRectFlags flags;

    /* If a player touches this rectangle, then
     *  the flags that are set (to 1) in
     *  enableFlags will also be set (to 1) on
     *  the player. The flags that are not set
     *  (they are 0), these will be ignored and
     *  not affect the player. This is how
     *  powerups work. */
    MWG_PlayerFlags enableFlags;

    /* If a player touches this rectangle, then
     *  the flags set (to 1) in disableFlags will
     *  be turned off (set to 0) on the player.
     *  The flags that are 0 are ignored and do
     *  not affect the player. */
    MWG_PlayerFlags disableFlags;
} MWG_MapRect;

typedef struct MWG_MapLabel {

    /* The position of the top left of the label
     */
    int x, y;

    int textHeight;

    /* The width of the label is
     *  textLen * textHeight */

    char text[16];

    int textLen;
} MWG_MapLabel;

typedef struct MWG_Map {
    MWG_MapRect rect[MWG_MAX_MAP_RECTS];
    int numRects;

    MWG_MapLabel label[MWG_MAX_MAP_LABELS];
    int numLabels;

    MWG_Player player[MWG_MAX_PLAYER];
    int numPlayers;
} MWG_Map;


extern const MWG_Map testMap;

extern const MWG_Map level1;

extern const MWG_Map level2;

extern const MWG_Map level3;


int MWG_AddMapRect(MWG_Map * map, MWG_MapRect * r);

int MWG_OpenMap(MWG_Map const * map, MWG_GameState * gameState);


#endif
