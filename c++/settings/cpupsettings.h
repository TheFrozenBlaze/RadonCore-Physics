#ifndef CPUPSETTINGS_H
#define CPUPSETTINGS_H

#define STATE3 enum threestate {FIRST, SECOND, THIRD};
#define STATE4 enum fourstate {FIRST, SECOND, THIRD, FOURTH};

bool allow = 1;
bool splitactiontree = 0;
//basically splits the action tree on a given level(if it has more than one branches)
//to multiple threads


#endif