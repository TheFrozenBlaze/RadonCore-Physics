#ifndef PHYSLAYERS_H
#define PHYLAYERS_H

#include "Material.h"
#include "time.h"
#include "3dvfunc.h"
#include <array>


class Basic {
public:
	Basic(Coord& cs, const std::vector<Material> material) {
		
	};
	~Basic();
};
class Planetary {
private:
const float gamma = 6.67 * 10e-11;
public:
	Planetary(Coord& cs) {
	};
	~Planetary();
};
class Thermal {
private:
//const float stefanboltzmann = ;
public:
	Thermal(Coord& cs) {};
	~Thermal();
};
class Fluid {
private:
public:
	Fluid(Coord& cs, std::vector<Material>* bodmaterail, bool bodyfluid, std::vector<Material>* surround) {
			
	};
	//navier stoakes shit
	~Fluid();
};
class Restrictive {
public:
	Restrictive(Coord& cs, std::vector<Material>* bodmaterial) {};
	~Restrictive();
};
class Photo {
private:
public:
	Photo(Coord& cs, std::vector<Material>* bodmaterial, std::vector<Material>* surmaterial) {};
	~Photo();

};
class Radioactive{
private:
public:
	Radioactive(Coord& cs, std::vector<Material>* bodmaterial, std::vector<Material>* intermat) {};
	~Radioactive();
};
class Aero{
private:
public:
	Aero(Coord& cs, std::array<float, 3> speed) {};
	~Aero();
};
class Electrical {
private:
public: 
	Electrical(Coord& cs, std::vector<Material>* bodmat, std::vector<Material>* surmat) {};
	~Electrical();
};
class Structural {
private:
	enum class flags : uint8_t
	{
		miniature = 0,
		big = 1 << 0,
		relative = 1 << 1,
		complex = 1 << 2
	};
	friend inline flags operator|(flags a, flags b)
	{
		return static_cast<flags>(
			static_cast<uint32_t>(a) |
			static_cast<uint32_t>(b));
	};

	friend inline flags operator&(flags a, flags b)
	{
		return static_cast<flags>(
			static_cast<uint32_t>(a) &
			static_cast<uint32_t>(b));
	};

public:
#define MINI 0x00
	#define BIG 0x01
	#define RELATIVE 0x02
	#define COMPLEX 0x04
	Structural(std::vector<std::pair<Material, Coord&>> thebods, std::vector<Material>* surmat, flags flags) {

	};
	~Structural();
};
#endif
