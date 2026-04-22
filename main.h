#ifndef MWG_MAIN_H
#define MWG_MAIN_H

#include"beginCode.h"
#include"d.h"
#include"devents.h"


#define MWG_MAX_MAP_RECTS 256
#define MWG_MAX_MAP_LABELS 128
#define MWG_MAX_PLAYER 128
#define MWG_PLAYER_WIDTH 50
#define MWG_PLAYER_HEIGHT 50
#define MWG_MAX_BUTTONS 50
#define MWG_MAX_TEXT_LEN 16

#define MWG_PLAYER_IMAGE_MAX_WIDTH 64
#define MWG_PLAYER_IMAGE_MAX_HEIGHT 64
#define MWG_PLAYER_IMAGE_BIT_DEPTH 32 /*in bits*/

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

#define MWG_MIN(a, b) (( (a) < (b) ) ? (a) : (b) )

#define MWG_MAX(a, b) (( (a) > (b) ) ? (a) : (b) )

/* This enum shows the walls a player hit in the
 *  last frame. If the player was moving left and
 *  collided with a wall, then
 *  MWG_COLLISION_DIR_LEFT should be set. If the
 *  player hit a celing MWG_COLLISION_DIR_UP
 *  should be set.
 *
 * Any of these options can be bitwise "ORed"
 *  togeter (|).
 */
typedef enum MWG_CollisionDirection {
    MWG_COLLISION_DIR_NONE  = 0,
    MWG_COLLISION_DIR_UP    = 0x1,
    MWG_COLLISION_DIR_DOWN  = 0x2,
    MWG_COLLISION_DIR_LEFT  = 0x4,
    MWG_COLLISION_DIR_RIGHT = 0x8,
} MWG_CollisionDirection;

/* These flags control abilities and other
 *  options the player may have (like weather the
 *  player can edit the map or fly).
 *
 * Any flag can be enabled and disabled
 *  independently, you can use bit-wise OR to mix
 *  any flags.
 */
typedef enum MWG_PlayerFlags {
    MWG_PLAYER_NO_FLAG      = 0,
    MWG_PLAYER_CAN_EDIT     = 0x1,
    MWG_PLAYER_FLYING       = 0x2,
    MWG_PLAYER_NOCLIP       = 0x4,
    MWG_PLAYER_HAS_WINGS    = 0x8
    /* Add a hook enabled flag? */
} MWG_PlayerFlags;

typedef enum MWG_HookState {
    MWG_HOOK_UNATTACHED,
    MWG_HOOK_ATTACHED
} MWG_HookState;

typedef struct MWG_Player {

    /* This x, y position is the center of the
     *  player. */
    int x, y;

    int oldX, oldY;

    /* Zoom number, set this to 256 for no zoom
     *  effect. */
    int zoom;

    /* The hitbox is relative to the x, y of the
     *  player, it is recommended to set the
     *  hitboxX to -(hitboxW / 2) and set hitboxY
     *  to -(hitboxH / 2). */
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

    /* This is the position of the player's
     *  grappling hook. These are absolute
     *  coordinates, NOT relative. */
    int hookX, hookY;

    MWG_HookState hookState;

    D_double angle;

    /* This shows if the player collided with a
     *  wall last frame, read the comment above
     *  the declaration of
     *  MWG_CollisionDirection. */
    MWG_CollisionDirection collisionDirection;

    /* Flags that control the abilities and other
     *  options of the player, read the
     *  declaration of MWG_PlayerFlags above. */
    MWG_PlayerFlags flags;

    /* This number is the index of the rectangle
     *  that the player is drawing, if the player
     *  isn't drawing one this is -1. This is
     *  used for level editing. */
    int drawingRect;


} MWG_Player;

typedef struct MWG_GameState {

    /* The output surface (screen or window) */
    D_Surf * out;

    /* Is the game running, non zero while
     *  running */
    int running;

    /* The state of the keyboard, updated each
     *  frame. */
    D_uint8 keyboardState[D_NumKeys];

    /* A pointer to a font surface (see
     *  D_PrintToSurf()) */
    D_Surf * fontImage;

    /* The current menu onscreen */
    MWG_Menu * menu;

    /* The current map loaded */
    MWG_Map * map;

    /* Data for profile1, this gets copied into a
     *  map when it's loaded, that copy of the
     *  data is called player1. */
    MWG_Player profile1;

    /* The index of player 1 in the current map
     *  if there is a map loaded, -1 otherwise */
    int player1Index;

    /* When a level is "won" (by touching a
     *  victory rectangle, where MWG_MAP_RECT_WIN
     *  is set) the level that is pointed to by
     *  this pointer gets loaded if it's not
     *  null. */
    MWG_Map const * nextMap;

    /* The frame rate (frames per second) the
     *  game should run at. */
    int frameRate;

    /* This data is enough for each player to
     *  have an image 64 by 64 (the numbers in
     *  MWG_PLAYER_IMAGE_MAX_WIDTH and
     *  MWG_PLAYER_IMAGE_MAX_HEIGHT).
     *
     * To access a player's image data use
     *  imageData[playerIndex * (MWG_PLAYER_IMAGE_MAX_WIDTH * MWG_PLAYER_IMAGE_MAX_HEIGHT * (MWG_PLAYER_IMAGE_BIT_DEPTH / 8))].
     *
     * If this is null, just use the default
     *  mouse image (this is to support platforms
     *  that may not have enough memory, a little
     *  more than 2MB).
     */
    void * imageData;

    /* When profile1 gets copied into a map, this
     *  can be copied at the same time as long as
     *  imageData above is not null. This pointer
     *  should point to either null or a buffer
     *  that is exactly
     *  MWG_PLAYER_IMAGE_MAX_WIDTH * MWG_PLAYER_IMAGE_MAX_HEIGHT * (MWG_PLAYER_IMAGE_BIT_DEPTH / 8)
     *  bytes large.
     */
    void * profile1ImageData;

} MWG_GameState;


int MWG_AddPlayer(MWG_Map * map, MWG_Player * player);

#endif
