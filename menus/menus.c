#include"../d.h"
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
