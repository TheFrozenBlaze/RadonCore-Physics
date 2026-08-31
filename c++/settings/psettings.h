#ifndef PSETTINGS_H
#define PSETTINGS_H

enum threestate {FIRST, SECOND, THIRD};
#define STATE3 enum threestate
enum fourstate {fFIRST, fSECOND, fTHIRD, fFOURTH};
#define STATE4 enum fourstate

extern bool drawgrid;
extern bool drawbodskel;
//controls
//extern char up = 'w';
//extern char down = 's';
//extern char left = 'a';
//extern char right = 'd';
//imgui settings


#endif