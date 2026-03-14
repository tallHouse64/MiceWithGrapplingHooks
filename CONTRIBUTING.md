
# Contributing

Thank you for taking the time to help with this project. Reading this file will hopefully give you a better idea of how MWGH works.


## MWGH Documentation

Please note that most of the documentation for MWGH is in the source code. Every function and structure should be explained there.


## Architecture

### Maps

The way MWGH stores, opens and runs maps is very simple.

Maps are stored as constant structures, this means they are part of the final executable. You can open a map by using MWG_OpenMap(), it copies the map into gameState->map, here it can be modified without affecting the original.

After opening a map nothing would happen except the screen would turn sky blue, you need to add a player to it. Use MWG_AddPlayer() (using gameState->player1), this copies a player structure into the map which the camera can follow which means you can now see the map. Please read the section on profiles to see why this is.

Each frame, MWG_CalcPhysics() is run on the map that is in gameState->map. This moves each player according to their speeds, gravity, collisions and other forces.

Note that MWG_CalcPhysics() also handles rectangle flags and player flags that relate to collision, like MWG_PLAYER_NOCLIP or MWG_MAP_RECT_INTANGIBLE. See the MWG_PlayerFlags and MWG_MapRectFlags enumerations for more flags.

At some point in the future maps do need to be stored as files so users can make their own. But lets keep some maps built into the game so platforms that don't have a file system can still have some content.

If MWGH takes up too much memory for some platforms (think GBA), consider compressing the constant map data and any other data that is rarely read (we don't want to decompress something every frame).


### Profiles

In MWGH, there is the concept of profiles. Any customisation of your character gets stored in your profile (after customisation features are added to MWGH).

When a map gets opened and player 1 gets added, what is happening is the profile gameState->player1 gets copied (not referenced) into the map. Because of this temporary changes can be made to your player that only affect you while the map is running, when the map is closed those changes are lost. To make permanent changes to your player, edit your profile. Then every time a map is opened, your player carries over the changes.


### Menus

MWGH has it's own UI system built in. As you can imagine, it's very limited. You can create menus using the MWG_Menu structure (please put them in menus.c like the others and make them constant).

When the game starts the main menu is shown, it gets copied into gameState->menu, pressing a button may open another menu, like the level menu. When this happens, it also gets copied in the same way.

A menu can be open while a map is running (like the pause menu), when this happens the map continues to run but the player can't move. Inputs instead control the menu. To check if a menu is onscreen at runtime see if it has 0 or less buttons (eg, "if(gameState->numButtons <= 0){then...};").

If you make a button, you need the ability to control what happens when it's pressed. You can assign an action to a button (like "button->action = MWG_BUTTON_QUIT"). See the MWG_ButtonAction enumeration for a list of all actions. If you want to make a new action, add it to the enumeration and change MWG_RunAction() so it handles your new action.

Buttons can also open maps, to set which map a button opens write to it's nextMap member.

While making a button, you also need to know how to control which button gets "hovered" over when up, down, left or right is pressed (this is only relevant for controller/keyboard input). If a user has button 0 hovered for example, to control which button gets hovered next if the user presses down, write the index of the next button to the current button's downButton member, set it to -1 for no change. Feel free to copy/edit other menus in menus.c and change their upButton, downButton, leftButton and rightButton members to see how this works.

Note that the index of the button that is currently being hovered over is in menu->hoveredButton, you probably want the menu that is onscreen which would be gameState->menu->hoveredButton.


## Global variables

In MWGH, no global variables are allowed except for constants. This is so that the behaviour of every function only depends on it's parameters, this makes testing much easier.


## Cross platform

MWGH is built using drws-lib, it inherits the same ideas when it comes to cross platform programming. That is, use as few dependencies as possible and use optional dependencies only, even the C standard library is not used.

If any file includes a header using angle brackets ("<" and ">"), this is a bug. Unless it's in an optional dependency or if the code does not end up in the final executable (like assets/convert.c). Including files with double quote marks is okay, as long as those files are in the repository.

It's okay to include headers with angle brackets temporarily as a "hack" to make what would otherwise be impossible. As long as the include line is conditionally included so it only affects specific platforms. This probably means that drws-lib doesn't have a feature that makes the "hack" necessary.


## Compiling

Before building for Linux or Windows, you need to install:
- make
- gcc
- SDL2 (or SDL3 with the SDL2 compatibility layer)

Windows users need MSYS2 as well as the above installed inside MSYS2.

To build for your machine run "make". This works for Linux and Windows. You can then run "./MWGH" to start the game.


## Cross Compiling

Remember to run "make clean" before compiling to a new platform if you have already compiled for a different platform.


Before building for nds you need to install:
- make
- Blocks DS (https://blocksds.skylyrac.net/)
- The Wonderful tool chain (the website above has instructions to install this)

To build for nds run "make nds" in the repository folder, this creates the MWGH.nds file in the "nds" folder. MWGH.nds can be run in an emulator or on a jailbroken DS.


If you want to build for web, you need to install:
- make
- Emscripten
- Any simple http server (like "python3 -m http.server") so you can test with localhost.

To build for web run "make web" in the repository folder. You can then test your build by running a server in the web folder (if you have python 3 try running "cd web && python3 -m http.server") then open "http://localhost:8000/" in a browser.
