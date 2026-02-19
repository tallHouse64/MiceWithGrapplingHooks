#include"../main.h"
#include"../maps/maps.h"

/* This function finds the entry point of a
 *  player into a rect if that player has just
 *  entered this frame. This is useful for
 *  collision resolution.
 *
 * Note that if this function finds valid entry
 *  or exit points, the points will always be
 *  outside the rectangle by 1 pixel. This means
 *  either:
 *   x must be rect->x - 1
 *  or
 *   x must be rect->x + rect->w
 *  or
 *   y must be rect->y - 1
 *  or
 *   y must be rect->y + rect->h
 *  Remember this applies to the entry and exit
 *  points. If the function returns 0 (success)
 *  and does not do this, it is a bug.
 *
 * It is safe to pass null for xEntryPoint,
 *  yEntryPoint, xExitPoint and yExitPoint if you
 *  don't need all the values.
 *
 * currentX: The players current x position.
 * currentY: The players current y position.
 * oldX: The players x position last frame.
 * oldY: The players y position last frame.
 * rect: The rectangle that the player just
 *  entered.
 * xEntryPoint: A number that gets filled in with
 *  the x position of where the player entered.
 * yEntryPoint: A number that gets filled in with
 *  the y position of where the player entered.
 * xExitPoint: A number that gets filled in with
 *  the x position of where the player would exit
 *  if the player kept moving.
 * yExitPoint: A number that gets filled in with
 *  the y position of where the player would exit
 *  if the player kept moving.
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_FindEntryPoint(int currentX, int currentY, int oldX, int oldY, MWG_MapRect * rect, int * xEntryPoint, int * yEntryPoint, int * xExitPoint, int * yExitPoint){

    /* This function has a known bug, for some
     *  edge cases it does not give an exit or
     *  entry point when it should. In game this
     *  is probably the cause of the
     *  unexplainable bouncing and clipping that
     *  sometimes happens. */

    if(rect == D_NULL){
        return -1;
    };

    /* There are two points below, one is an
     *  entry point the other is an exit point.
     *  It is unknown which is the entry point
     *  until the end of the function. */

    /* Point A */
    int pointAX = 0;
    int pointAY = 0;

    /* Point B */
    int pointBX = 0;
    int pointBY = 0;

    /* Set this to 1 when point B get filled with
     *  coordinates of a valid entry point. */
    int usedPointB = 0;

    /* Avoid a division by 0, solving the problem
     *  in a different way for this edge case. */
    if(oldX == currentX){

        /* Does the player pass through the
         *  rect? */
        if(oldX >= rect->x && oldX < rect->x + rect->w){

            pointAX = oldX;
            pointAY = rect->y - 1;

            pointBX = oldX;
            pointBY = rect->y + rect->h;

            usedPointB = 1;

        }else{
            return -2;
        };
    };

    /* Avoid a division by 0*/
    if(oldY == currentY){

        /* Does the player pass through the
         *  rect? */
        if(oldY >= rect->y && oldY < rect->y + rect->h){

            pointAX = rect->x - 1;
            pointAY = oldY;

            pointBX = rect->x + rect->w;
            pointBY = oldY;

            usedPointB = 1;

        }else{
            return -2;
        };
    };


    /* The whole point of this while loop is to
     *  have the ability to skip to the end of it
     *  using "break;". The loop should only run
     *  once. */
    while(usedPointB == 0){

        /* The two lines below assume that there
         *  is a valid entry or exit point on the
         *  top wall. */
        pointAX = LERP_INT(oldX, currentX, (((rect->y - oldY) * 256) / (currentY - oldY)));
        pointAY = rect->y - 1;
        /* Minus one above to make it just
         *  outside by 1 pixel. */

        /* Is there actually a valid entry/exit
         *  point on the top wall? */
        if(pointAX >= rect->x && pointAX <= rect->x + rect->w){
            pointBX = pointAX;
            pointBY = pointAY;
            usedPointB = 1;
        };



        /* The two lines below assume that there
         *  is a valid entry or exit point on the
         *  left wall. */
        pointAX = rect->x - 1;
        pointAY = LERP_INT(oldY, currentY, (((rect->x - oldX) * 256) / (currentX - oldX)));

        /* Is there actually a valid entry/exit
         *  point on the left wall? */
        if(pointAY > rect->y && pointAY <= rect->y + rect->h){
            if(usedPointB){
                /* Entry and exit points are
                 *  found, skip to the end. */
                break;
            };

            pointBX = pointAX;
            pointBY = pointAY;
            usedPointB = 1;
        };


        /* The two lines below assume that there
         *  is a valid entry or exit point on the
         *  right wall. */
        pointAX = rect->x + rect->w;
        pointAY = LERP_INT(oldY, currentY, ((((rect->x + rect->w) - oldX) * 256) / (currentX - oldX)));
         /* pointAX is already outside by 1
          *  pixel. */

        /* Is there actually a valid entry/exit
         *  point on the right wall? */
        if(pointAY > rect->y && pointAY <= rect->y + rect->h){
            if(usedPointB){
                /* Entry and exit points are
                 *  found, skip to the end. */
                break;
            };

            pointBX = pointAX;
            pointBY = pointAY;
            usedPointB = 1;
        };


        /* The two lines below assume that there
         *  is a valid entry or exit point on the
         *  bottom wall. */
        pointAX = LERP_INT(oldX, currentX, ((((rect->y + rect->h) - oldY) * 256) / (currentY - oldY)));
        pointAY = rect->y + rect->h;
        /* Minus one above to make it just
         *  outside by 1 pixel. */

        /* Is there actually a valid entry/exit
         *  point on the bottom wall? */
        if(pointAX > rect->x && pointAX <= rect->x + rect->w){
            if(usedPointB){
                /* Entry and exit points are
                 *  found, skip to the end. */
                break;
            };

            pointBX = pointAX;
            pointBY = pointAY;
            usedPointB = 1;
        };

        return -2;
    };

    /* Now find which point, A or B is the entry
     *  point. */

    /* Is point A closer to old x, y? */
    if(((pointAX - oldX) * (pointAX - oldX)) + ((pointAY - oldY) * (pointAY - oldY)) <
       ((pointBX - oldX) * (pointBX - oldX)) + ((pointBY - oldY) * (pointBY - oldY))){
        /* Point A is the entry point! */

        if(xEntryPoint != D_NULL){
            *xEntryPoint = pointAX;
        };

        if(yEntryPoint != D_NULL){
            *yEntryPoint = pointAY;
        };

        if(xExitPoint != D_NULL){
            *xExitPoint = pointBX;
        };

        if(yExitPoint != D_NULL){
            *yExitPoint = pointBY;
        };
    }else{
        /* Point B is the entry point! */

        if(xEntryPoint != D_NULL){
            *xEntryPoint = pointBX;
        };

        if(yEntryPoint != D_NULL){
            *yEntryPoint = pointBY;
        };

        if(xExitPoint != D_NULL){
            *xExitPoint = pointAX;
        };

        if(yExitPoint != D_NULL){
            *yExitPoint = pointAY;
        };
    };

    return 0;
};

/* This function detects a collision between a
 *  player's hitbox and a rectangle.
 *
 * If there is a collision, the function returns
 *  1, otherwise it returns 0. Bear in mind that
 *  if null is passed for player or rect the
 *  function just returns -1 which is important
 *  if you call it in an if statement.
 *
 * player: The player test for a collision (it's
 *  safe to pass null for this).
 * rect: The rectangle to test for a collision
 *  (it's safe to pass null for this).
 * returns: On success 1 gets returned when there
 *  is a collision, otherwise 0. A negative
 *  number gets returned on failure.
 */
int MWG_DetectCollision(MWG_Player * player, MWG_MapRect * rect){

    if(player == D_NULL || rect == D_NULL){
        return -1;
    };

    /* Find the hitbox of the player, with
     *  absolute coordinates (relative to the
     *  origin of the map). */
    D_Rect hitbox = {player->x + player->hitboxX, player->y + player->hitboxY, player->hitboxW, player->hitboxH};

    /* Does the hitbox overlap with the
     *  rectangle. */
    if( hitbox.x + hitbox.w > rect->x &&
        hitbox.x < rect->x + rect->w &&
        hitbox.y + hitbox.h > rect->y &&
        hitbox.y < rect->y + rect->h
        ){
        return 1;
    };

    return 0;
};

/* This function checks if a point is in a
 *  rectangle and returns 1 if it is, otherwise
 *  0.
 *
 * The reason this function exists is because
 *  using D_PointInRect() would mean converting
 *  many numbers into D_Point and D_Rect structs.
 *  Using this function where it's convenient
 *  should make code more compact and readable.
 *
 * rx: The x position of the rectangle.
 * ry: The y position of the rectangle.
 * rw: The width of the rectangle.
 * rh: The height of the rectangle.
 * px: The x position of the point.
 * py: The y position of the point.
 * returns: 1 when the point is in the rectangle,
 *  otherwise 0.
 */
int MWG_PointInRect(int rx, int ry, int rw, int rh, int px, int py){

    if( px >= rx &&
        px < rx + rw &&
        py >= ry &&
        py < ry + rh
    ){
        return 1;
    };

    return 0;
};

/* This function resolves a collision between a
 *  rectangle and a player by moving the player
 *  (this function does consider the hitbox of
 *  the player).
 *
 * It is safe to pass null for all of the
 *  parameters of this function, it would do
 *  nothing and return -1.
 *
 * player: The player to move.
 * rect: The rectangle that is colliding with the
 *  player.
 * newX: A pointer that gets filled in with the
 *  players new x position after resolving the
 *  collision.
 * newY: A pointer that gets filled in with the
 *  players new y position after resolving the
 *  collision.
 * returns: On success, 1 gets returned when a
 *  collision is resolved and 0 gets returned if
 *  the function detects there is no collision. A
 *  negative number gets returned on failure.
 */
int MWG_ResolveCollision(MWG_Player * player, MWG_MapRect * rect, int * newX, int * newY){

    /* The force that the player is launched
     *  backward when detected inside a
     *  rectangle. -256 for a full bounce with no
     *  energy lost or 0 for no bounce. */
    int const clippingBounceBackForce = 0;

    if(player == D_NULL || rect == D_NULL || newX == D_NULL || newY == D_NULL){
        return -1;
    };

    /* Is the top left of the hitbox inside the
     *  rectangle? */
    if(MWG_PointInRect(rect->x, rect->y, rect->w, rect->h, player->x + player->hitboxX, player->y + player->hitboxY)){
        MWG_FindEntryPoint(player->x + player->hitboxX, player->y + player->hitboxY,
                           player->oldX + player->hitboxX, player->oldY + player->hitboxY,
                           rect,
                           newX, newY,
                           D_NULL,
                           D_NULL);

        /* If the player hit the right wall */
        if(*newX == rect->x + rect->w){

            /*Change result y to apply friction*/
            *newY = LERP_INT(player->y + player->hitboxY, *newY, rect->friction);

            player->collisionDirection = player->collisionDirection | MWG_COLLISION_DIR_LEFT;

        /* If the player hit the bottom wall */
        }else if(*newY == rect->y + rect->h){

            /*Change result x to apply friction*/
            *newX = LERP_INT(player->x + player->hitboxX, *newX, rect->friction);

            player->collisionDirection = player->collisionDirection | MWG_COLLISION_DIR_UP;
        }else{

            /* Clipping detected, bounce the
             *  player backward. Bear in mind
             *  it's repeated below 4 times. */
            *newX = LERP_INT(player->oldX, player->x, clippingBounceBackForce);
            *newY = LERP_INT(player->oldY, player->y, clippingBounceBackForce);
            return 1;
        };

        *newX = *newX - player->hitboxX;
        *newY = *newY - player->hitboxY;

        return 1;
    };

    /* Is the top right of the hitbox inside the
     *  rectangle? */
    if(MWG_PointInRect(rect->x, rect->y, rect->w, rect->h, player->x + player->hitboxX + player->hitboxW - 1, player->y + player->hitboxY)){
        MWG_FindEntryPoint(player->x + player->hitboxX + player->hitboxW - 1, player->y + player->hitboxY,
                           player->oldX + player->hitboxX + player->hitboxW - 1, player->oldY + player->hitboxY,
                           rect,
                           newX, newY,
                           D_NULL,
                           D_NULL);

        /* If the player hit the left wall */
        if(*newX == rect->x - 1){

            /*Change result y to apply friction*/
            *newY = LERP_INT(player->y + player->hitboxY, *newY, rect->friction);

            player->collisionDirection = player->collisionDirection | MWG_COLLISION_DIR_RIGHT;

        /* If the player hit the bottom wall */
        }else if(*newY == rect->y + rect->h){

            /*Change result x to apply friction*/
            *newX = LERP_INT(player->x + player->hitboxX, *newX, rect->friction);

            player->collisionDirection = player->collisionDirection | MWG_COLLISION_DIR_UP;
        }else{

            /* Clipping detected, bounce the
             *  player backward. */
            *newX = LERP_INT(player->oldX, player->x, clippingBounceBackForce);
            *newY = LERP_INT(player->oldY, player->y, clippingBounceBackForce);
            return 1;
        };

        /* If the player's new position is
         *  touching the left wall, move the
         *  player left by 1 pixel (instead of
         *  getting moved into the wall). */
        if(*newX == rect->x){
            *newX = *newX - 1;
        };

        *newX = *newX - (player->hitboxX + player->hitboxW - 1);
        *newY = *newY - player->hitboxY;

        return 1;
    };

    /* Is the bottom left of the hitbox inside
     *  the rectangle? */
    if(MWG_PointInRect(rect->x, rect->y, rect->w, rect->h, player->x + player->hitboxX, player->y + player->hitboxY + player->hitboxH - 1)){
        MWG_FindEntryPoint(player->x + player->hitboxX, player->y + player->hitboxY + player->hitboxH - 1,
                           player->oldX + player->hitboxX, player->oldY + player->hitboxY + player->hitboxH - 1,
                           rect,
                           newX, newY,
                           D_NULL,
                           D_NULL);

        /* If the player hit the right wall */
        if(*newX == rect->x + rect->w){

            /*Change result y to apply friction*/
            *newY = LERP_INT(player->y + player->hitboxY, *newY, rect->friction);

            player->collisionDirection = player->collisionDirection | MWG_COLLISION_DIR_LEFT;

        /* If the player hit the top wall */
        }else if(*newY == rect->y - 1){

            /*Change result x to apply friction*/
            *newX = LERP_INT(player->x + player->hitboxX, *newX, rect->friction);

            player->collisionDirection = player->collisionDirection | MWG_COLLISION_DIR_DOWN;
        }else{

            /* Clipping detected, bounce the
             *  player backward. */
            *newX = LERP_INT(player->oldX, player->x, clippingBounceBackForce);
            *newY = LERP_INT(player->oldY, player->y, clippingBounceBackForce);
            return 1;
        };

        /* If the player's new position is
         *  touching the top wall, move the
         *  player up by 1 pixel (instead of
         *  getting moved into the wall). */
        if(*newY == rect->y){
            *newY = *newY - 1;
        };

        *newX = *newX - player->hitboxX;
        *newY = *newY - (player->hitboxY + player->hitboxH - 1);

        return 1;
    };

    /* Is the bottom right of the hitbox inside
     *  the rectangle? */
    if(MWG_PointInRect(rect->x, rect->y, rect->w, rect->h, player->x + player->hitboxX + player->hitboxW - 1, player->y + player->hitboxY + player->hitboxH - 1)){
        MWG_FindEntryPoint(player->x + player->hitboxX + player->hitboxW - 1, player->y + player->hitboxY + player->hitboxH - 1,
                           player->oldX + player->hitboxX + player->hitboxW - 1, player->oldY + player->hitboxY + player->hitboxH - 1,
                           rect,
                           newX, newY,
                           D_NULL,
                           D_NULL);

        /* If the player hit the left wall */
        if(*newX == rect->x - 1){

            /*Change result y to apply friction*/
            *newY = LERP_INT(player->y + player->hitboxY, *newY, rect->friction);

            player->collisionDirection = player->collisionDirection | MWG_COLLISION_DIR_RIGHT;

            /* If the player hit the top wall */
        }else if(*newY == rect->y - 1){

            /*Change result x to apply friction*/
            *newX = LERP_INT(player->x + player->hitboxX, *newX, rect->friction);

            player->collisionDirection = player->collisionDirection | MWG_COLLISION_DIR_DOWN;
        }else{

            /* Clipping detected, bounce the
             *  player backward. */
            *newX = LERP_INT(player->oldX, player->x, clippingBounceBackForce);
            *newY = LERP_INT(player->oldY, player->y, clippingBounceBackForce);
            return 1;
        };

        /* If the player's new position is
         *  touching the left wall, move the
         *  player left by 1 pixel (instead of
         *  getting moved into the wall). */
        if(*newX == rect->x){
            *newX = *newX - 1;
        };

        /* If the player's new position is
         *  touching the top wall, move the
         *  player up by 1 pixel (instead of
         *  getting moved into the wall). */
        if(*newY == rect->y){
            *newY = *newY - 1;
        };

        *newX = *newX - (player->hitboxX + player->hitboxW - 1);
        *newY = *newY - (player->hitboxY + player->hitboxH - 1);

        return 1;
    };

    return 0;
};


/* This function moves calculates the physics for
 *  all players in the map for one frame. This
 *  function should be called once per frame.
 *
 * It's safe to pass null for map, the function
 *  would do nothing and return -1.
 *
 * map: The map to move forward by one frame
 *  (it's safe to pass null for this).
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_CalcPhysics(MWG_Map * map){

    if(map == D_NULL){
        return -1;
    };

    int i = 0;
    int j = 0;
    int temp1 = 0;
    int temp2 = 0;
    int newX = 0;
    int newY = 0;
    for(; i < map->numPlayers; i++){




        /* Apply gravity unless flying */
        if(!(map->player[i].flags & MWG_PLAYER_FLYING)){
            map->player[i].oldY = map->player[i].oldY - ((70 * DELAY)/256);
        };

        /* Apply air resistance (halve the
         *  speed) */
        map->player[i].oldX = LERP_INT(map->player[i].oldX, map->player[i].x, 64);
        map->player[i].oldY = LERP_INT(map->player[i].oldY, map->player[i].y, 64);

        if(map->player[i].hookState == MWG_HOOK_ATTACHED){

            map->player[i].oldX = LERP_INT(map->player[i].oldX, map->player[i].hookX, -32);
            map->player[i].oldY = LERP_INT(map->player[i].oldY, map->player[i].hookY, -32);
        };


        /* Stop the player moving when the speed
         *  is very slow (this is probably
         *  masking a bug where the player's
         *  speed changes depending on what
         *  quadrant around the origin the player
         *  is). */
        if(map->player[i].oldX >= map->player[i].x - 1 && map->player[i].oldX <= map->player[i].x + 1){
            map->player[i].oldX = map->player[i].x;
        };

        if(map->player[i].oldY >= map->player[i].y - 1 && map->player[i].oldY <= map->player[i].y + 1){
            map->player[i].oldY = map->player[i].y;
        };


        /* Move player */

        temp1 = map->player[i].x;
        temp2 = map->player[i].y;

        map->player[i].x = map->player[i].x + (map->player[i].x - map->player[i].oldX);
        map->player[i].y = map->player[i].y + (map->player[i].y - map->player[i].oldY);

        map->player[i].oldX = temp1;
        map->player[i].oldY = temp2;

        /* Reset the stored collision infromation
         *  from last frame. */
        map->player[i].collisionDirection = MWG_COLLISION_DIR_NONE;


        /* If noclip is enabled on the player,
         *  don't run the below collision
         *  detection and resolution (just skip
         *  the text player). */
        if(map->player[i].flags & MWG_PLAYER_NOCLIP){
            continue;
        };

        /* Loop through all the map rectangles
         *  and detect collisions. */
        for(j = 0;j < map->numRects; j++){

            if(map->rect[j].flags & MWG_MAP_RECT_INTANGIBLE){
                continue;
            };

            /* If newX and newY are not
             *  overwritten by
             *  MWG_ResolveCollision() and it
             *  still returns non-zero, the
             *  player should bounce because of
             *  the two lines below. */
            newX = LERP_INT(map->player[i].oldX, map->player[i].x, -256);
            newY = LERP_INT(map->player[i].oldY, map->player[i].y, -256);

            if(MWG_ResolveCollision(&map->player[i], &map->rect[j], &newX, &newY)){

                if(map->rect[j].flags & MWG_MAP_RECT_TELEPORT){

                    map->player[i].oldX = map->rect[j].tpX - (map->player[i].x - map->player[i].oldX);
                    map->player[i].oldY = map->rect[j].tpY - (map->player[i].y - map->player[i].oldY);

                    map->player[i].x = map->rect[j].tpX;
                    map->player[i].y = map->rect[j].tpY;

                    /* When you teleport, release
                     *  the grappling hook. */
                    map->player[i].hookState = MWG_HOOK_UNATTACHED;

                    continue;
                };

                /* Enable flags that this
                 *  rectangle enables. */
                map->player[i].flags = map->player[i].flags | map->rect[j].enableFlags;

                /* Disable player flags that this
                 *  rectangle disables. */
                map->player[i].flags = map->player[i].flags & (~map->rect[j].disableFlags);

                map->player[i].x = newX;
                map->player[i].y = newY;
            };
        };


    };

    return 0;
};
