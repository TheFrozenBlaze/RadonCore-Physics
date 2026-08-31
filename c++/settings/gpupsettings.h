#ifndef GPUPSETTINGS_H
#define GPUPSETTINGS_H

enum threestate {FIRST, SECOND, THIRD};
enum fourstate {FIRST, SECOND, THIRD, FOURTH};
#define STATE4 enum fourstate
#define STATE3 enum threestate
bool vulkanallow = 0;
STATE4 GPUGPversion = FIRST;
// first: opencl api
// second: cuda api
// third: rocm api
// fourth: oneapi

//only opencl 1.2 is functional the other are at max experimental, but mostly aren't implemented yet

#endif