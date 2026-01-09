#ifndef MWG_PHYSICS_H
#define MWG_PHYSICS_H

int MWG_FindEntryPoint(MWG_Player * player, MWG_MapRect * rect, int * xEntryPoint, int * yEntryPoint);

int MWG_PointInRect(int rx, int ry, int rw, int rh, int px, int py);

int MWG_CalcPhysics(MWG_Map * map);

#endif
