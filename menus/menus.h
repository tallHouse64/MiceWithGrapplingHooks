#ifndef MWG_MENUS_H
#define MWG_MENUS_H

#include"../main.h"
#include"../maps/maps.h"

typedef enum MWG_ButtonAction {
    MWG_BUTTON_NO_ACTION,
    MWG_BUTTON_QUIT,
    MWG_BUTTON_DRAW_RECT,
    MWG_BUTTON_NOCLIP_FLY
} MWG_ButtonAction;

typedef struct MWG_Button {
    D_Rect rect;

    char text[MWG_MAX_TEXT_LEN];
    int textLen;

    /* The upButton number is an index of what
     *  button should be hovered over next when
     *  up is pressed (set menu->hoveredButton to
     *  upButton when up is pressed). Set it to
     *  -1 for nothing to happen. The same is
     *  true for downButton when down is pressed,
     *  etc. */
    int upButton;
    int downButton;
    int leftButton;
    int rightButton;

    /* If this button is activated, then set the
     *  menu to nextMenu. If nextMenu is null
     *  then set the map to nextMap and hide the
     *  menu. If both are null then do nothing.*/
    const void * nextMenu;
    const MWG_Map * nextMap;

    /* Colour */
    int r, g, b;

    /* Colour when hovered over */
    int hovR, hovG, hovB;

    /* This stores what the button does like quit
     *  the program. */
    MWG_ButtonAction action;
} MWG_Button;

typedef struct MWG_Menu {

    /* The index of the button that is currently
     *  being hovered over. This can be -1 if
     *  nothing is being hovered over. */
    int hoveredButton;

    MWG_Button button[MWG_MAX_BUTTONS];
    int numButtons;
} MWG_Menu;


extern const MWG_Menu mainMenu;

extern const MWG_Menu levelMenu;

extern const MWG_Menu pauseMenu;

extern const MWG_Menu emptyMenu;

extern const MWG_Menu editorMenu;


int MWG_RunAction(MWG_ButtonAction * action, MWG_GameState * gameState);

int MWG_ControlMenu(MWG_Menu * menu, D_Event * e, MWG_Map * map, MWG_Player * player, int * playerIndex, MWG_GameState * gameState);

#endif
