#include"../d.h"
#include"../devents.h"
#include"../main.h"
#include"menus.h"
#include"../maps/maps.h"

MWG_Menu mainMenu = {
    -1,
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
            "Exit",
            14,
            0,
            -1,
            -1,
            -1,
            D_NULL,
            D_NULL,
            0, 0, 0, /* Colour */
            100, 100, 100 /* Hover colour */
        }
    },
    2 /* Num buttons */
};

MWG_Menu levelMenu = {
    -1,
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
        &testMap,
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
        D_NULL,
        0, 0, 0, /* Colour */
        100, 100, 100 /* Hover colour */
    }
    },
    3 /* Num buttons */
};

int MWG_ControlMenu(MWG_Menu * menu, D_Event * e, MWG_Map * map, int * playerIndex){

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
                };
            };

            break;
    };

    return 0;
};
