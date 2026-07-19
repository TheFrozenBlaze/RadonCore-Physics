#ifndef PHYSICS_H
#define PHYSICS_H

#include <cstdint>
#include "physlayers.h"
#include <chrono>
struct SimDet {
	std::vector<std::array<float, 3>> forces;
	uint start;
	uint finish;
	Coord& cs;
    Material* bodmat;
	uint8_t meshboost;
};

typedef struct SimDef {
	std::chrono::duration<double, std::milli> simdur;
	uint tiles;
    double system_energy;
    Material *surmats;
    std::vector<SimDet *> action;
};
struct InitSettings {
    std::vector<std::pair<std::array<float, 3>, Coord &>> speed;
};

namespace Physics
{
    enum PhysLayersFlags 
    {
        None,
        Basic,
        Planetary,
        Thermal,
        Fluid,
        Restrictive,
        Photo,
        Radioactive,
        Aero,
        Electrical,
        Structural
    };


    inline PhysLayersFlags operator|(PhysLayersFlags a, PhysLayersFlags b)
	{
		return static_cast<PhysLayersFlags>(
			static_cast<uint32_t>(a) |
			static_cast<uint32_t>(b));
	};
    extern 
	inline PhysLayersFlags operator&(PhysLayersFlags a, PhysLayersFlags b)
	{
		return static_cast<PhysLayersFlags>(
			static_cast<uint32_t>(a) &
			static_cast<uint32_t>(b));
	};
    SimDef *ConstructSim(std::vector<Coord>& cs);
    InitSettings PreSimState(SimDef* def);
    bool StartSim(SimDef* def, enum Physics::PhysLayersFlags flags, InitSettings& init);
};
#endif