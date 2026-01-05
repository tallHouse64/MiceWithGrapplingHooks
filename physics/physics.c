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
    };

    /* At this point the player must have been
     *  inside the rect last frame, do nothing
     *  and return -2. */

    return -2;
};

int MWG_CalcPhysics(MWG_Map * map){

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

        #if 0
        /* Loop through all the map rectangles. */
        j = 0;
        xEntryPoint = 0;
        yEntryPoint = 0;
        while(j < map->numRects){

            /* Is the player in this rect?
             *  (collision detection). */
            if( map->player[i].x >= map->rect[j].x &&
                map->player[i].x <  map->rect[j].x + map->rect[j].w &&
                map->player[i].y >= map->rect[j].y &&
                map->player[i].y <  map->rect[j].y + map->rect[j].h
                ){

                /* Collision detected, now do
                 *  collision resolution. */

                /* Is the old position in the
                 *  rect? */
                if( map->player[i].oldX >= map->rect[j].x &&
                    map->player[i].oldX <  map->rect[j].x + map->rect[j].w &&
                    map->player[i].oldY >= map->rect[j].y &&
                    map->player[i].oldY <  map->rect[j].y + map->rect[j].h
                    ){
                    /* For now don't bother doing
                     *  collision resolution in
                     *  this case. */

                    j++;
                continue;
    };

    /* Was the player player to the
     *  left of the rect last frame? */
    if(map->player[i].oldX < map->rect[j].x){
        /* Original equation (only
         *  works for floats and
         *  doubles, not ints):
         *  (((map->rect[j].x - map->player[i].oldX) / (map->player[i].x - map->player[i].oldX)) * (map->player[i].y - map->player[i].oldY)) + map->player[i].oldY
         */
        yEntryPoint = ((((map->rect[j].x - map->player[i].oldX) * (map->player[i].y - map->player[i].oldY)) / (map->player[i].x - map->player[i].oldX))) + map->player[i].oldY;

        /* The yEntry point may be
         *  wrong, check if it's on
         *  the border of the rect.*/
        if(yEntryPoint < map->rect[j].y){
            /* yEntryPoint is wrong,
             *  don't bother
             *  correcting it, at
             *  this point in the
             *  logic the player must
             *  have collided with
             *  the top of the rect.
             */

            map->player[i].y = map->rect[j].y - 1;

    }else if(yEntryPoint >= map->rect[j].y + map->rect[j].h){
        /*yEntryPoint is wrong,
         *  don't bother
         *  correcting it,
         *  the player must have
         *  collided with the
         *  bottom.
         */

        map->player[i].y = map->rect[j].y + map->rect[j].h;
    };

    /* Was the player to the
     *  right of the rect last
     *  frame? */
    }else if(map->player[i].oldX >= map->rect[j].x + map->rect[j].w){

    };

    };

    j++;
    };

    #endif


    /* Move player */

    temp1 = map->player[i].x;
    temp2 = map->player[i].y;

    map->player[i].x = map->player[i].x + (map->player[i].x - map->player[i].oldX);
    map->player[i].y = map->player[i].y + (map->player[i].y - map->player[i].oldY);

    map->player[i].oldX = temp1;
    map->player[i].oldY = temp2;

    i++;
    };

    return 0;
};
