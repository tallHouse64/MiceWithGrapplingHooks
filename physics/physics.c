#include"../main.h"

/* This function finds the entry point of a
 *  player into a rect if that player has just
 *  entered this frame. This is useful for
 *  collision resolution.
 *
 * This function assumes that the player was
 *  outside the rect last frame, but is inside
 *  this frame (otherwise there may be a division
 *  by zero and the program would crash).
 *
 * player: The player that just entered a
 *  rectangle.
 * rect: The rectangle that the player just
 *  entered.
 * xEntryPoint: A number that gets filled in with
 *  the x position of where the player entered.
 * yEntryPoint: A number that gets filled in with
 *  the y position of where the player entered.
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_FindEntryPoint(MWG_Player * player, MWG_MapRect * rect, int * xEntryPoint, int * yEntryPoint){

    if(player == D_NULL || rect == D_NULL || xEntryPoint == D_NULL || yEntryPoint == D_NULL){
        return -1;
    };

    /* Was the player to the left of the rect
     *  last frame? */
    if(player->oldX < rect->x){

        /* Find the y entry point, this may be
         *  wrong, check in the next if
         *  statement. */
        *yEntryPoint = ((((rect->x - player->oldX) * (player->y - player->oldY)) / (player->x - player->oldX))) + player->oldY;

        /* Is yEntryPoint on the border of the
         *  rect? */
        if(*yEntryPoint >= rect->y && *yEntryPoint < rect->y + rect->h){
            /* The yEntry point is correct, set
             *  xEnrtyPoint and return. */

            *xEntryPoint = rect->x;
            return 0;
        };
    };

    /* At this point, the entry point can't be on
     *  the left wall of the rect (the function
     *  would have returned). */

    /* Was the player to the right of the rect
     *  last frame? */
    if(player->oldX >= rect->x + rect->w){


        /* Find the y entry point (this time on
         *  the right wall, not left), this may
         *  be wrong, check in the next if
         *  statement. */
        *yEntryPoint = (((((rect->x + rect->w) - player->oldX) * (player->y - player->oldY)) / (player->x - player->oldX))) + player->oldY;

        /* Is yEntryPoint on the border of the
         *  rect? */
        if(*yEntryPoint >= rect->y && *yEntryPoint < rect->y + rect->h){
            /* The yEntry point is correct, set
             *  xEnrtyPoint and return. */

            *xEntryPoint = rect->x + rect->w;
            return 0;
        };
    };

    /* Now the entry point must be either the top
     *  or bottom wall. */

    /* Was the player above the rect last
     *  frame? */
    if(player->oldY < rect->y){

        /* Original equation (doesn't work for
         *  integers).
         * (((rect.y - player->oldY) / (player->y - player->oldY)) * (player->x - player->oldX)) + player->oldX
         */

        *xEntryPoint = (((rect->y - player->oldY) * (player->x - player->oldX)) / (player->y - player->oldY)) + player->oldX;

        /* The entry point must be on the top
         *  wall of the rectangle. */
        *yEntryPoint = rect->y;
        return 0;
    };

    /* Was the player below the rect last frame? */
    if(player->oldY >= rect->y + rect->h){

        *xEntryPoint = ((((rect->y + rect->h) - player->oldY) * (player->x - player->oldX)) / (player->y - player->oldY)) + player->oldX;

        /* The entry point must be on the bottom
         *  wall of the rectangle. */
        *yEntryPoint = rect->y + rect->h;
        return 0;
    };

    /* At this point the player must have been
     *  inside the rect last frame, do nothing
     *  and return -2. */

    return -2;
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
    int xEntryPoint = 0;
    int yEntryPoint = 0;
    while(i < map->numPlayers){




        /* Apply gravity */
        map->player[i].oldY = map->player[i].oldY - 5;

        /* Apply air resistance (halve the
         *  speed) */
        map->player[i].oldX = (map->player[i].oldX + map->player[i].x) / 2;
        map->player[i].oldY = (map->player[i].oldY + map->player[i].y) / 2;



        /* Move player */

        temp1 = map->player[i].x;
        temp2 = map->player[i].y;

        map->player[i].x = map->player[i].x + (map->player[i].x - map->player[i].oldX);
        map->player[i].y = map->player[i].y + (map->player[i].y - map->player[i].oldY);

        map->player[i].oldX = temp1;
        map->player[i].oldY = temp2;


        /* Loop through all the map rectangles
         *  and detect collisions. */
        j = 0;
        while(j < map->numRects){

            /* Is the player in this rect?
             *  (collision detection). */
            if( map->player[i].x >= map->rect[j].x &&
                map->player[i].x <  map->rect[j].x + map->rect[j].w &&
                map->player[i].y >= map->rect[j].y &&
                map->player[i].y <  map->rect[j].y + map->rect[j].h
            ){

                xEntryPoint = 0;
                yEntryPoint = 0;
                if(MWG_FindEntryPoint(&map->player[i], &map->rect[j], &xEntryPoint, &yEntryPoint) == -2){
                    /* The player was inside the
                     *  rect last frame, do
                     *  nothing. */

                    j++;
                    continue;
                };

                /* Now do collision resolution */

                /* Did the player hit the top? */
                if(yEntryPoint == map->rect[j].y){

                    /* Snap to the top of the
                     *  rectangle. */
                    map->player[i].y = map->rect[j].y - 1;

                    j++;
                    continue;

                    /* Did the player hit the left
                     *  wall? */
                }else if(xEntryPoint == map->rect[j].x){

                    /* Snap to the left wall. */
                    map->player[i].x = map->rect[j].x - 1;

                    /* Did the player hit the right
                     *  wall? */
                }else if(xEntryPoint == map->rect[j].x + map->rect[j].w){

                    /* Snap to the right wall. */
                    map->player[i].x = map->rect[j].x + map->rect[j].w;

                    /* Did the player hit the bottom
                     *  of the rect? */
                }else if(yEntryPoint == map->rect[j].y + map->rect[j].h){

                    /* Snap to the bottom of the
                     *  rect. */
                    map->player[i].y = map->rect[j].y + map->rect[j].h;
                };

            };

            j++;
        };

        i++;
    };

    return 0;
};
