#ifndef PHYSICS_H
#define PHYSICS_H

#include <cstdint>
#include "physlayers.h"

namespace Physics
{
    enum class PhysLayersFlags : uint16_t
    {
        None = 0,
        Basic = 1 << 0,
        Planetary = 1 << 1,
        Thermal = 1 << 2,
        Fluid = 1 << 3,
        Restrictive = 1 << 4,
        Photo = 1 << 5,
        Radioactive = 1 << 6,
        Aero = 1 << 7,
        Electrical = 1 << 8,
        Structural = 1 << 9
    };
    #define BASIC 0x01
    #define PLANETARY 0x02
    #define THERMAL 0x04
    #define FLUID 0x08
    #define RESTRICTIVE 0x16
    #define PHOTO 0x32
    #define RADIOACTIVE 0x364
    #define AERO 0x128
    #define ELECTRICAL 0x256
    #define STRUCTURAL 0x512

    inline PhysLayersFlags operator|(PhysLayersFlags a, PhysLayersFlags b)
	{
		return static_cast<PhysLayersFlags>(
			static_cast<uint32_t>(a) |
			static_cast<uint32_t>(b));
	};

	inline PhysLayersFlags operator&(PhysLayersFlags a, PhysLayersFlags b)
	{
		return static_cast<PhysLayersFlags>(
			static_cast<uint32_t>(a) &
			static_cast<uint32_t>(b));
	};

    void ConstructSim(PhysLayersFlags flags);
    void StartSim();
};
#endif