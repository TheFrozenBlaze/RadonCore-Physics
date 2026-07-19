#ifndef GSETTINGS_H
#define GSETTINGS_H


enum threestate {FIRST, SECOND, THIRD};
#define STATE3 enum threestate
enum fourstate {fFIRST, fSECOND, fTHIRD, fFOURTH};
#define STATE4 enum fourstate


bool backfaceculling = 1;
bool depthcheck = 1;
bool drawduringsim = 1;
//draw the simulation
STATE3 lighting = FIRST;
//first : normal, all around lighting
//second: headlamp
//third: inverse of headlamp

#endif