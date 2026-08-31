#ifndef MATERIAL_H
#define MATERIAL_H

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

#endif
