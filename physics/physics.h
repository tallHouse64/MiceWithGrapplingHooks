#ifndef MWG_PHYSICS_H
#define MWG_PHYSICS_H

#include"../maps/maps.h"

int MWG_FindEntryPoint(int currentX, int currentY, int oldX, int oldY, MWG_MapRect * rect, int * xEntryPoint, int * yEntryPoint, int * xExitPoint, int * yExitPoint);

int MWG_DetectCollision(MWG_Player * player, MWG_MapRect * rect);

int MWG_PointInRect(int rx, int ry, int rw, int rh, int px, int py);

int MWG_CalcPhysics(MWG_Map * map, MWG_GameState * gameState);

#endif
