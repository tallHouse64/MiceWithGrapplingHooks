#ifndef MWG_MENUS_H
#define MWG_MENUS_H

#include"../main.h"
#include"../maps/maps.h"

typedef enum MWG_ButtonAction {
    MWG_BUTTON_NO_ACTION,
    MWG_BUTTON_QUIT,
    MWG_BUTTON_DRAW_RECT,
    MWG_BUTTON_NOCLIP_FLY,
    MWG_BUTTON_FPS_UP,
    MWG_BUTTON_FPS_DOWN
} MWG_ButtonAction;

/* This enum can be used to set the text on a
 *  button to anything non-static like
 *  framerates, battery, IP addresses, etc. Use
 *  MWG_BUTTON_TEXT_ORIGINAL to show the original
 *  static text.
 *
 * Note that MWG_BUTTON_TEXT_FPS shows the frame
 *  rate to aim for which is shown in the options
 *  menu (not a detected frame rate).
 */
typedef enum MWG_ButtonReplacementText {
    MWG_BUTTON_TEXT_ORIGINAL = 0,
    MWG_BUTTON_TEXT_FPS
} MWG_ButtonReplacementText;

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

    /* For buttons that show a number, string or
     *  other variable that can change at
     *  runtime, setting this value changes what
     *  text appears on the button. This is for
     *  text that is unknown at compile time and
     *  cannot be entered the usual way like ip
     *  addresses, battery, frame rate, etc.
     *
     * This value can be set to
     *  MWG_BUTTON_TEXT_FPS to show the current
     *  frame rate set in the options menu for
     *  example. Set this to
     *  MWG_BUTTON_TEXT_ORIGINAL to show the
     *  original text (the text member above). */
    MWG_ButtonReplacementText replacementText;
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

extern const MWG_Menu optionsMenu;

extern const MWG_Menu emptyMenu;

extern const MWG_Menu editorMenu;

extern const MWG_Menu winMenu;


int MWG_RunAction(MWG_ButtonAction * action, MWG_GameState * gameState);

int MWG_ControlMenu(MWG_Menu * menu, D_Event * e, MWG_GameState * gameState);

#endif
