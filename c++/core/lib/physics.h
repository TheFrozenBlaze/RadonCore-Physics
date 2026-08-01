#ifndef PHYSICS_H
#define PHYSICS_H

#include <cstdint>
#include <cstdio>
//#include "physlayers.h"
#include <chrono>
#include <glad/glad.h>

#include "coord.h"
struct SimDet {
	std::vector<std::array<float, 3>> forces;
	uint start;
	uint finish;
	Coord cs;
    GLuint EBO;
    GLuint lEBO;
    GLuint VBO;
    GLuint VAO;
    //Material *bodmat;
    uint8_t meshboost;
};

struct SimDef {
	std::chrono::duration<double, std::milli> simdur;
	uint tiles;
    double system_energy;
    //Material* surmats;
    std::vector<SimDet> action;
};

struct ProjectDef {
    FILE* jsonPointer;
    SimDef* simPointer;
    std::string name;



};



struct InitSettings
{
    std::vector<std::pair<std::array<float, 3>, SimDet&>> speed;
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
    extern std::vector<ProjectDef> projectQueue;

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
void AddElement(SimDef& def, Coord&& cs);
    //InitSettings PreSimState(SimDef* def);
    //bool StartSim(SimDef* def, enum Physics::PhysLayersFlags flags, InitSettings& init);
};
#endif