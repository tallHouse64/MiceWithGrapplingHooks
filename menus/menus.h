#ifndef MWG_MENUS_H
#define MWG_MENUS_H

#include"../main.h"
#include"../maps/maps.h"


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
    void * nextMenu;
    MWG_Map * nextMap;

    /* Colour */
    int r, g, b;

    /* Colour when hovered over */
    int hovR, hovG, hovB;
} MWG_Button;

typedef struct MWG_Menu {

    /* The index of the button that is currently
     *  being hovered over. This can be -1 if
     *  nothing is being hovered over. */
    int hoveredButton;

    MWG_Button button[MWG_MAX_BUTTONS];
    int numButtons;
} MWG_Menu;


extern MWG_Menu mainMenu;

extern MWG_Menu levelMenu;


#endif
