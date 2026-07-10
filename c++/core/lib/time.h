#ifndef TIME_H
#define TIME_H
#include "glmain.h"
#include "3dvfunc.h"
#include <vector>
#include <chrono>

 struct SimDet {
	std::vector<std::array<float, 3>> forces;
	uint64_t start;
	uint64_t finish;
	Coord& cs;
	uint8_t meshboost;
};

struct SimDef {
	std::chrono::duration<double, std::milli> simdur;
	uint64_t tiles;
	std::vector<SimDet*> action;
};
void Step(SimDef* def);

#endif 
