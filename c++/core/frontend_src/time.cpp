#include "time.h"
#include <array>
void Step(SimDef* def) {
if(def->action.empty()) {
	std::cout << "No bodies in action" << std::endl;
	exit(1);
}
if(def->simdur == 0) {
	bool infinite {true};
	while(infinite) {
        for(size_t j{}; j < def->action.size(); j++) {
		
			if((def->action[j]->start <= i) && (def->action[j]->finish >= i)){
			std::array<float, 3> appliedforce {0.f,0.f,0.f};
			for (size_t l{}; l < def->action[j]->forces.size(); l++)
			{
				appliedforce[0] += def->action[j]->forces[l][0];
				appliedforce[1] += def->action[j]->forces[l][1];
				appliedforce[2] += def->action[j]->forces[l][2];
			}
			for(size_t k{}; k< def->action[j]->cs.vpc.size()/3; k++) 
			{
				def->action[j]->cs.vpc[k*3] += appliedforce[0];
				def->action[j]->cs.vpc[k*3+1] += appliedforce[1];
				def->action[j]->cs.vpc[k*3+2] += appliedforce[2];
			}
		}
	}
}else{
	for(size_t i{}; i < def->tiles; i++) {
		for(size_t j{}; j < def->action.size(); j++) {
		
			if((def->action[j]->start <= i) && (def->action[j]->finish >= i)){
			std::array<float, 3> appliedforce {0.f,0.f,0.f};
			for (size_t l{}; l < def->action[j]->forces.size(); l++)
			{
				appliedforce[0] += def->action[j]->forces[l][0];
				appliedforce[1] += def->action[j]->forces[l][1];
				appliedforce[2] += def->action[j]->forces[l][2];
			}
			for(size_t k{}; k< def->action[j]->cs.vpc.size()/3; k++) 
			{
				def->action[j]->cs.vpc[k*3] += appliedforce[0];
				def->action[j]->cs.vpc[k*3+1] += appliedforce[1];
				def->action[j]->cs.vpc[k*3+2] += appliedforce[2];
			}
			} else {
				continue;
			}
		}
	}
}
};

