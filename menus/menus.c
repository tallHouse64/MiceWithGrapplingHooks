#include"../d.h"
#include"../devents.h"
#include"../main.h"
#include"menus.h"
#include"../maps/maps.h"

#ifdef NDS
/* This is temporary */
#include<nds.h>
#else

/* This is also a hack to get the code to compile
 *  for PC */
#define KEY_UP 0
#define KEY_DOWN 0
#define KEY_LEFT 0
#define KEY_RIGHT 0
#define KEY_A 0
#define KEY_START 0
#define KEY_SELECT 0

#endif

const MWG_Menu mainMenu = {
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

const MWG_Menu levelMenu = {
    0,
    {
    {
        {-135, -25, 100, 25},
        "Back",
        4,
        4,  /* Press up for test level */
        -1, /* Pressing down does nothing */
        -1, /* Pressing left does nothing */
        1,  /* Press right for level 1 */
        &mainMenu,
        D_NULL,
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    },
    {
        {-25, -25, 50, 50},
        "1",
        1,
        4,  /* Press up for test level */
        -1, /* Pressing down does nothing */
        0,  /* Press left for the back button */
        2,  /* Press right for level 2 */
        D_NULL,
        &level1,
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    },
    {
        {35, -25, 50, 50},
        "2",
        1,
        4,  /* Press up for test level */
        -1, /* Pressing down does nothing */
        1,  /* Press left for level 1 */
        3,  /* Press left for level 3 */
        D_NULL,
        &level2,
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    },
    {
        {95, -25, 50, 50}, /* x, y, w, h */
        "3", /* Text */
        1, /* Text length */
        4,  /* Press up for test level */
        -1, /* Pressing down does nothing */
        2,  /* Press left for level 2 */
        -1, /* Pressing right does nothing */
        D_NULL, /* Open this menu on press */
        &level3, /* Open this map on press */
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    },
    {
        {-25, -60, 200, 25},
        "Test map",
        8,
        -1, /* Pressing up does nothing */
        1,  /* Press down for level 1 */
        -1, /* Pressing left does nothing */
        -1, /* Pressing right does nothing */
        D_NULL,
        &testMap,
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    }
    },
    5 /* Num buttons */
};

const MWG_Menu pauseMenu = {
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
            &optionsMenu,
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

const MWG_Menu optionsMenu = {
    0, /* hoveredButton */
    {
    {
        {-195, -70, 390, 30},
        "Back",
        4, /* text len */
        -1, /* Pressing up does nothing */
        2,  /* Press down for "up" (FPS) */
        -1, /* Pressing left does nothing */
        -1, /* Pressing right does nothing */
        &pauseMenu,
        D_NULL,
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    },
    {
        {-195, -30, 165, 30},
        "FPS",
        3, /* text len */
        0,  /* Press up for "back" */
        -1, /* Pressing down does nothing */
        -1, /* Pressing left does nothing */
        -1, /* Pressing right does nothing */
        D_NULL,
        D_NULL,
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    },
    {
        {-25, -30, 60, 30},
        "dn",
        2, /* text len */
        0,  /* Press up for "back" */
        -1, /* Pressing down does nothing */
        -1, /* Pressing left does nothing */
        4, /* Press right for "up" */
        D_NULL,
        D_NULL,
        0, 0, 0, /* Colour */
        100, 100, 100, /* Hover colour */
        MWG_BUTTON_FPS_DOWN
    },
    {
        {40, -30, 90, 30},
        " ??",
        3, /* text len */
        0,  /* Press up for "back" */
        -1, /* Pressing down does nothing */
        -1, /* Pressing left does nothing */
        -1, /* Pressing right does nothing */
        D_NULL,
        D_NULL,
        0, 0, 0, /* Colour */
        100, 100, 100, /* Hover colour */

        /*When the button is pressed do nothing*/
        MWG_BUTTON_NO_ACTION,

        /* Replace the text with
         *  gameState->frameRate */
        MWG_BUTTON_TEXT_FPS
    },
    {
        {135, -30, 60, 30},
        "up",
        2, /* text len */
        0,  /* Press up for "back" */
        -1, /* Pressing down does nothing */
        2,  /* Press left for "dn" */
        -1, /* Pressing right does nothing */
        D_NULL,
        D_NULL,
        0, 0, 0, /* Colour */
        100, 100, 100, /* Hover colour */
        MWG_BUTTON_FPS_UP
    }
    },
    5 /* numButtons */
};

const MWG_Menu editorMenu = {
    0, /* hoveredButton */
    {
    {
        {-125, -80, 250, 25},
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
        {-125, -50, 250, 25},
        "Draw rect",
        9,
        0,
        2,
        -1,
        -1,
        &emptyMenu,
        D_NULL,
        0, 0, 0, /* Colour */
        100, 100, 100, /* Hover colour */
        MWG_BUTTON_DRAW_RECT
    },
    {
        {-125, -20, 250, 25},
        "Noclip fly",
        10,
        1,
        -1,
        -1,
        -1,
        &emptyMenu,
        D_NULL,
        0, 0, 0, /* Colour */
        100, 100, 100, /* Hover colour */
        MWG_BUTTON_NOCLIP_FLY
    },
    }, /* buttons */
    3 /* numButtons */
};

const MWG_Menu winMenu = {
    0, /* hoveredButton */
    {
    {
        {-150, -50, 300, 30}, /* x, y, w, h */
        "Next level", /* Text */
        10, /* Text length */
        -1, /* Pressing up does nothing */
        1,  /* Press down for "Close" */
        -1, /* Pressing left does nothing */
        -1, /* Pressing right does nothing */
        D_NULL, /* Open this menu on press */
        D_NULL, /* Open this map on press */
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    },
    {
        {-150, -10, 150, 30}, /* x, y, w, h */
        "Close", /* Text */
        5, /* Text length */
        0,  /* Press up for "Next level" */
        -1, /* Pressing down does nothing */
        -1, /* Pressing left does nothing */
        -1, /* Pressing right does nothing */
        &emptyMenu, /* Open this menu on press */
        D_NULL, /* Open this map on press */
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    },
    }, /* buttons */
    2 /* numButtons */
};

const MWG_Menu emptyMenu = {
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
                MWG_MapRect r = {player1->x, player1->y, 8, 8, D_NULL, 17, 127, 11, 128};
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

        case MWG_BUTTON_NOCLIP_FLY:

            /* If the player is flying */
            if(player1->flags & MWG_PLAYER_FLYING){

                /* Turn off flying and noclip */
                player1->flags = player1->flags & (~(MWG_PLAYER_FLYING | MWG_PLAYER_NOCLIP));
            }else{

                /* Otherwise turn on flying and
                 *  noclip */
                player1->flags = player1->flags | MWG_PLAYER_FLYING | MWG_PLAYER_NOCLIP;
            };

            break;

        case MWG_BUTTON_FPS_UP:
            /* When the FPS up button in the
             *  options menu is pressed... */
            gameState->frameRate = gameState->frameRate + 5;
            break;

        case MWG_BUTTON_FPS_DOWN:
            /* When the FPS down button in the
             *  options menu is pressed... */
            gameState->frameRate = gameState->frameRate - 5;
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
 *  overwrites the map in gameState->map using a
 *  pointer with the new map. The menu passed in
 *  may also be overwritten if a button is
 *  pressed that opens a new menu.
 *
 * If a map gets opened, this function also adds
 *  a player to the map using MWG_AddPlayer() (if
 *  profile is not null) and overwrites the menu
 *  with an empty one to remove the menu.
 *
 * This function calls MWG_OpenMap() to open
 *  maps.
 *
 * If 0 gets pressed and there is no menu open
 *  (numButtons is 0), this function opens the
 *  editor menu by overwriting "menu". This also
 *  happens when escape is pressed except the
 *  pause menu opens.
 *
 * Some buttons need to change the game state,
 *  like "quit". This is what the gameState
 *  pointer is for. It is safe to pass null for
 *  this but some buttons may not work.
 *
 * menu: The menu to control, and overwrite when
 *  a new menu is opened.
 * e: An event to handle.
 * profile: The player structure to copy into a
 *  map using MWG_AddPlayer() when a map gets
 *  opened.
 * playerIndex: If a new map gets opened and a
 *  player gets added using MWG_AddPlayer(), this
 *  number gets overwritten with the index of
 *  that player.
 * gameState: The game state to modify when some
 *  buttons are pressed or when a map is opened.
 * returns: 0 on success or a negative number on
 *  failure.
 */
int MWG_ControlMenu(MWG_Menu * menu, D_Event * e, MWG_Player * profile, int * playerIndex, MWG_GameState * gameState){

    /* Keys up on a DS */
    D_uint32 keysReleased = 0;

    /* This is only used when opening a map and
     *  adding a player to it. */
    int newPlayerIndex = -1;

#ifdef NDS
    /* This is a hack to get input working on a
     *  DS and should be removed after drws-lib
     *  gets controller support. */

    keysReleased = keysUp();

    /* These 2 lines especially are a hack */
    e->type = D_KEYDOWN;
    e->keyboard.key = D_KNone;

#endif

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

            /* If 0 is pressed (or select is
             *  pressed on nds) and there is no
             *  menu onscreen, then open the
             *  editor menu. */
            if((e->keyboard.key == D_K0 || (keysReleased & KEY_SELECT)) && menu->numButtons <= 0){
                *menu = editorMenu;
            };

            /* If escape is pressed (or start is
             *  pressed on nds) and there is no
             *  menu onscreen, then open the
             *  pause menu. */
            if((e->keyboard.key == D_KEscape || keysReleased & KEY_START) && menu->numButtons <= 0){
                *menu = pauseMenu;
            };

            if((e->keyboard.key == D_Kw || e->keyboard.key == D_KUp || (keysReleased & KEY_UP)) && menu->button[menu->hoveredButton].upButton >= 0){
                menu->hoveredButton = menu->button[menu->hoveredButton].upButton;
            };

            if((e->keyboard.key == D_Ks || e->keyboard.key == D_KDown || (keysReleased & KEY_DOWN)) && menu->button[menu->hoveredButton].downButton >= 0){
                menu->hoveredButton = menu->button[menu->hoveredButton].downButton;
            };

            if((e->keyboard.key == D_Ka || e->keyboard.key == D_KLeft || (keysReleased & KEY_LEFT)) && menu->button[menu->hoveredButton].leftButton >= 0){
                menu->hoveredButton = menu->button[menu->hoveredButton].leftButton;
            };

            if((e->keyboard.key == D_Kd || e->keyboard.key == D_KRight || (keysReleased & KEY_RIGHT)) && menu->button[menu->hoveredButton].rightButton >= 0){
                menu->hoveredButton = menu->button[menu->hoveredButton].rightButton;
            };

            /* If space or enter have been
             *  pressed. */
            if(e->keyboard.key == D_KSpace || e->keyboard.key == D_KEnter || (keysReleased & KEY_A)){

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
                }else if(menu->button[menu->hoveredButton].nextMap != D_NULL && gameState->map != D_NULL){

                    MWG_OpenMap(menu->button[menu->hoveredButton].nextMap, gameState);

                    /* If a player stucture was
                     *  passed into the function
                     *  and a pointer to store
                     *  the index, then add
                     *  (copy) the player onto
                     *  the map. */
                    if(profile != D_NULL){
                        newPlayerIndex = MWG_AddPlayer(gameState->map, profile);

                        if(playerIndex != D_NULL){
                            *playerIndex = newPlayerIndex;
                        };
                    };

                    /* Remove the menu */
                    *menu = emptyMenu;
                };
            };

            break;
    };

    return 0;
};
