#ifndef PHYSLAYERS_H
#define PHYLAYERS_H

#include "Material.h"
#include "time.h"
#include "3dvfunc.h"
#include "vk.h"
#include <array>

namespace Layer
{
class Basic {
public:
	Basic(Coord& cs,Material& material) {
		//MyCL::VolumeCalc(cs);
		vkInit();
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
	Fluid(Coord& cs, Material& bodmaterial,Material& surround) {
			
	};
	//navier stoakes shit
	~Fluid();
};
class Restrictive {
public:
	Restrictive(Coord& cs, Material& bodmaterial) {};
	~Restrictive();
};
class Photo {
private:
public:
	Photo(Coord& cs, Material& bodmaterial, Material& surmaterial) {};
	~Photo();

};
class Radioactive{
private:
public:
	Radioactive(Coord& cs, Material& bodmaterial, Material& intermat) {};
	~Radioactive();
};
class Aero{
private:
public:
	Aero(Coord& cs, std::array<float, 3> speed, Material& surmat) {};
	~Aero();
};
class Electrical {
private:
public: 
	Electrical(Coord& cs, Material& bodmat, Material& surmat) {};
	~Electrical();
};

class Structural {
private:
	
public:
	Structural(Coord& cs,Material& bodmat , Material& surmat) {

	};
	~Structural();
};
}
#endif
