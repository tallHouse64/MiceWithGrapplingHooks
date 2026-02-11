#include"../d.h"
#include"../devents.h"
#include"../main.h"
#include"menus.h"
#include"../maps/maps.h"

MWG_Menu mainMenu = {
    0,
    {
        {
            {-210, -50, 420, 30},
            "Start crashing",
            14,
            -1,
            1,
            -1,
            -1,
            &levelMenu,
            D_NULL,
            0, 0, 0, /* Colour */
            100, 100, 100 /* Hover colour */
        },
        {
            {-210, -10, 120, 30},
            "Quit",
            14,
            0,
            -1,
            -1,
            -1,
            D_NULL,
            D_NULL,
            0, 0, 0, /* Colour */
            100, 100, 100, /* Hover colour */
            MWG_BUTTON_QUIT
        }
    },
    2 /* Num buttons */
};

MWG_Menu levelMenu = {
    0,
    {
    {
        {-135, -25, 100, 25},
        "Back",
        4,
        -1,
        -1,
        -1,
        1,
        &mainMenu,
        D_NULL,
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    },
    {
        {-25, -25, 50, 50},
        "1",
        1,
        -1,
        -1,
        0,
        2,
        D_NULL,
        &level1,
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    },
    {
        {35, -25, 50, 50},
        "2",
        1,
        -1,
        -1,
        1,
        -1,
        D_NULL,
        &testMap,
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    }
    },
    3 /* Num buttons */
};

MWG_Menu pauseMenu = {
    0,
    {
        {
            {-150, -70, 300, 30},
            "Resume",
            6,
            -1,
            1,
            -1,
            -1,
            &emptyMenu,
            D_NULL,
            0, 0, 0, /* Colour */
            100, 100, 100 /* Hover colour */
        },
        {
            {-150, -20, 300, 30},
            "Options",
            7,
            0,
            2,
            -1,
            -1,
            D_NULL,
            D_NULL,
            0, 0, 0, /* Colour */
            100, 100, 100 /* Hover colour */
        },
        {
            {-150, 30, 300, 30},
            "Main menu",
            9,
            1,
            -1,
            -1,
            -1,
            &mainMenu,
            D_NULL,
            0, 0, 0, /* Colour */
            100, 100, 100 /* Hover colour */
        }
    },
    3 /* Num buttons */
};

MWG_Menu editorMenu = {
    0, /* hoveredButton */
    {
    {
        {-112, -80, 225, 25},
        "Resume",
        6,
        -1,
        1,
        -1,
        -1,
        &emptyMenu,
        D_NULL,
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    },
    {
        {-112, -50, 225, 25},
        "Draw rect",
        9,
        0,
        -1,
        -1,
        -1,
        &emptyMenu,
        D_NULL,
        0, 0, 0, /* Colour */
        100, 100, 100, /* Hover colour */
        MWG_BUTTON_DRAW_RECT
    },
    }, /* buttons */
    2 /* numButtons */
};

MWG_Menu emptyMenu = {
    -1, /* hoveredButton */
    {}, /* buttons */
    0 /* numButtons */
};

/* This function runs an action stored in a
 *  button for when it is pressed (like quit,
 *  change FPS, etc).
 *
 * If the action is not recognised, the function
 *  would do nothing and return -2.
 *
 * It is safe to pass null for action and
 *  gameState, the function would do nothing and
 *  return -1.
 *
 * action: The action to take.
 * gameState: The game state to modify in order
 *  to take the action.
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_RunAction(MWG_ButtonAction * action, MWG_GameState * gameState){

    MWG_Player * player1 = D_NULL;

    if(action == D_NULL || gameState == D_NULL){
        return -1;
    };

    player1 = &gameState->map->player[gameState->player1Index];

    switch(*action){

        case MWG_BUTTON_QUIT:
            /* Stop the program from running at
             *  the end of this frame */
            gameState->running = 0;
            break;

        case MWG_BUTTON_DRAW_RECT:

            /* If player 1 is not drawing a rect
             */
            if(player1->drawingRect == -1){

                /* Start drawing a rect (point 1
                 *  of the rectangle is set here,
                 *  read below). */
                MWG_MapRect r = {player1->x, player1->y, 8, 8, D_NULL, 255, 255, 255, 128};
                player1->drawingRect = MWG_AddMapRect(gameState->map, &r);

                /* If creating the new rectangle
                 *  failed */
                if(player1->drawingRect < 0){

                    /* Stop drawing */
                    player1->drawingRect = -1;
                };

            }else{
                /* Otherwise finish drawing the
                 *  rect that is being drawn now.
                 *  By setting one of the corners
                 *  of the new rectangle to where
                 *  the player is now.
                 *
                 * Point 1 of the rectangle is
                 *  where the rectangle started
                 *  being drawn.
                 *
                 * Point 2 of the rectangle is
                 *  where the player is at this
                 *  point of the logic (where the
                 *  rectangle stops being drawn).
                 *
                 * The name of that rect is:
                 *  gameState->map->rect[player1->drawingRect]
                 */

                /* Is the player to the left of
                 *  point 1 of the rectanlge? */
                if(player1->x < gameState->map->rect[player1->drawingRect].x){

                    gameState->map->rect[player1->drawingRect].w =
                    gameState->map->rect[player1->drawingRect].x - player1->x;

                    gameState->map->rect[player1->drawingRect].x = player1->x;
                }else{

                    gameState->map->rect[player1->drawingRect].w =
                    player1->x - gameState->map->rect[player1->drawingRect].x;

                };

                /* Is the player above point 1 of
                 *  the rectanlge? */
                if(player1->y < gameState->map->rect[player1->drawingRect].y){

                    gameState->map->rect[player1->drawingRect].h =
                    gameState->map->rect[player1->drawingRect].y - player1->y;

                    gameState->map->rect[player1->drawingRect].y = player1->y;
                }else{

                    gameState->map->rect[player1->drawingRect].h =
                    player1->y - gameState->map->rect[player1->drawingRect].y;

                };

                player1->drawingRect = -1;
            };

            break;

        default:
            /* Action not recognised */
            return -2;
            break;
    };

    return 0;
};

/* This function handles player input on a menu.
 *
 * When a button is pressed on a menu, if the
 *  button opens a new map, this function
 *  overwrites the map passed in using a pointer
 *  with the new map. The menu passed in may also
 *  be overwritten if a button is pressed that
 *  opens a new menu.
 *
 * If a map gets opened, this function also adds
 *  a player to the map using MWG_AddPlayer() and
 *  overwrites the menu with an empty one to
 *  remove the menu.
 *
 * If 0 gets pressed and there is no menu open
 *  (numButtons is 0), this function opens the
 *  editor menu by overwriting "menu".
 *
 * Some buttons need to change the game state,
 *  like "quit". This is what the gameState
 *  pointer is for. It is safe to pass null for
 *  this but some buttons may not work.
 *
 * menu: The menu to control, and overwrite when
 *  a new menu is opened.
 * e: An event to handle.
 * map: A map to overwrite if a new map gets
 *  opened.
 * player: The player structure to copy into a
 *  map using MWG_AddPlayer() when a map gets
 *  opened.
 * playerIndex: If a new map gets opened and a
 *  player gets added using MWG_AddPlayer(), this
 *  number gets overwritten with the index of
 *  that player.
 * gameState: The game state to modify when some
 *  buttons are pressed.
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_ControlMenu(MWG_Menu * menu, D_Event * e, MWG_Map * map, MWG_Player * player, int * playerIndex, MWG_GameState * gameState){

    MWG_Menu emptyMenu = {
        -1, /* hoveredButton */
        {}, /* buttons */
        0 /* numButtons */
    };

    if(menu == D_NULL || e == D_NULL){
        return -1;
    };

    switch(e->type){
        case D_KEYDOWN:

            /* When any button is pressed, if
             *  no button is being hovered
             *  over then hover over the
             *  zeroth one. */
            if(menu->hoveredButton < 0){
                menu->hoveredButton = 0;
            };

            if(e->keyboard.key == D_K0 && menu->numButtons <= 0){
                *menu = editorMenu;
            };

            if((e->keyboard.key == D_Kw || e->keyboard.key == D_KUp) && menu->button[menu->hoveredButton].upButton >= 0){
                menu->hoveredButton = menu->button[menu->hoveredButton].upButton;
            };

            if((e->keyboard.key == D_Ks || e->keyboard.key == D_KDown) && menu->button[menu->hoveredButton].downButton >= 0){
                menu->hoveredButton = menu->button[menu->hoveredButton].downButton;
            };

            if((e->keyboard.key == D_Ka || e->keyboard.key == D_KLeft) && menu->button[menu->hoveredButton].leftButton >= 0){
                menu->hoveredButton = menu->button[menu->hoveredButton].leftButton;
            };

            if((e->keyboard.key == D_Kd || e->keyboard.key == D_KRight) && menu->button[menu->hoveredButton].rightButton >= 0){
                menu->hoveredButton = menu->button[menu->hoveredButton].rightButton;
            };

            /* If space or enter have been
             *  pressed. */
            if(e->keyboard.key == D_KSpace || e->keyboard.key == D_KEnter){

                if(gameState != D_NULL){
                    MWG_RunAction(&menu->button[menu->hoveredButton].action, gameState);
                };

                /* Change the menu if this
                 *  button points to another
                 *  menu. */
                if(menu->button[menu->hoveredButton].nextMenu != D_NULL){
                    *menu = *((MWG_Menu *)(menu->button[menu->hoveredButton].nextMenu));

                    /* Otherwise change the map
                     *  if this button points to
                     *  a map. */
                }else if(menu->button[menu->hoveredButton].nextMap != D_NULL && map != D_NULL){
                    *map = *(menu->button[menu->hoveredButton].nextMap);

                    /* If a player stucture was
                     *  passed into the function
                     *  and a pointer to store
                     *  the index, then add
                     *  (copy) the player onto
                     *  the map. */
                    if(player != D_NULL && playerIndex != D_NULL){
                        *playerIndex = MWG_AddPlayer(map, player);
                    };

                    /* Remove the menu */
                    *menu = emptyMenu;
                };
            };

            break;
    };

    return 0;
};
