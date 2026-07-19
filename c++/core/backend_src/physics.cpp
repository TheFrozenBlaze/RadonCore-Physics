#include <iostream>
#include "Material.h"
#include "physics.h"
#include "clfunc.h"
#include "physlayers.h"

SimDef* ConstructSim(std::chrono::duration<double, std::milli> duration, uint16_t unitspers, std::vector<std::pair<Coord&, std::pair<std::chrono::duration<double, std::milli>, std::chrono::duration<double, std::milli>>>>& object_description);

bool Physics::StartSim(SimDef* def, enum Physics::PhysLayersFlags flags, InitSettings& init) {


    for(size_t i{}; i < def->action.size(); i++)
    {
    if(flags == Basic) {
        Layer::Basic basic(def->action[i]->cs, *def->action[i]->bodmat);   
    }
    if(flags == Planetary) {

        Layer::Planetary planetary(def->action[i]->cs);
        
    }
    if(flags == Thermal) {
        Layer::Thermal thermal(def->action[i]->cs);
    }
    if(flags == Fluid) {

        Layer::Fluid fluid(def->action[i]->cs, *def->action[i]->bodmat, *def->surmats);

    }
    if(flags == Restrictive) {
        Layer::Restrictive restrictive(def->action[i]->cs, *def->action[i]->bodmat); 
    }
    if(flags == Photo) {
        Layer::Photo photo(def->action[i]->cs, *def->action[i]->bodmat, *def->surmats);
    }
    if(flags == Radioactive) {
        Layer::Radioactive radioactive(def->action[i]->cs, *def->action[i]->bodmat, *def->surmats);
    }
    if(flags == Aero) {
        Layer::Aero aero(def->action[i]->cs, init.speed[i].first, *def->surmats);
    }
    if(flags == Electrical) {
        Layer::Electrical electrical(def->action[i]->cs, *def->action[i]->bodmat, *def->surmats);
    }
    if(flags == Structural) {
        Layer::Structural structural(def->action[i]->cs,*def->action[i]->bodmat, *def->surmats);
    }
    }
    
    if (def->action.empty())
    {
        std::cout << "No bodies in action" << std::endl;
        exit(1);
    }
if(def->simdur <= static_cast<std::chrono::duration<double, std::milli>>(10e-1)) {
	bool infinite {true};
	    while(infinite) {
            for(size_t j{}; j < def->action.size(); j++) {
                 MyCL::ForceApply(*def->action[j]);
	        }
        }
    } else {
	    for(size_t i{}; i < def->tiles; i++) {
		    for(size_t j{}; j < def->action.size(); j++) {
			    if((def->action[j]->start <= i) && (def->action[j]->finish >= i)){
                    MyCL::ForceApply(*def->action[j]);
			    } else {
				    continue;
			    }
		    }
	    }
    }
}
