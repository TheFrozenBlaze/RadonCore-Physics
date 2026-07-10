
#ifndef MATERIAL_H
#define MATERIAL_H
#include <unordered_map>
struct Material {
	float density,
		youngsmodulus,
		poisson,
		yieldstrength,
		ultimatetensile,
		shearstrength,
		torsionalstr,
		tconduct,
		specheat,
		texpand,
		econduct,
		permittivity,
		permeability,
		albedo,
		emissivity,
		corrosionres,
		radres,
		neutronabs,
		neutronscat,
		viscosity,
		cohesiveen;
};
Material Aluminium6061_T6 =
	{

		2700.0f,
		68.9e9f, 
		0.33f,	 
		276e6f,	 
		310e6f,	 
		207e6f,	 
		207e6f,	 

		167.0f,	  
		896.0f,	  
		23.6e-6f, 

		2.50e7f,   
		8.85e-12f, 
		1.000022f, 

		0.65f, 
		0.09f, 

		0.90f, 

		0.80f, 
		0.23f, 
		1.50f,

		0.0f,
		3.3e10f
};
#endif
