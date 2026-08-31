#include <iostream>
//#include "Material.h"
#include "clfunc.h"
#include <string>
#include "misc.h"
using json = nlohmann::json;
// #include "clfunc.h"
// #include "physlayers.h"

std::vector<ProjectDef> Physics::projectQueue;

void Physics::AddElement(SimDef& def, std::unique_ptr<Coord>&& cs) {
	// Material* is currently commented, don't forget that!(for nullptr)
	def.action.emplace_back(SimDet{std::vector<std::array<float, 3>>{}, 0, 0, std::move(cs), 0, 0, 0, 0});
};

void Physics::SimulationCollect(size_t ongoing) 
{
	std::unique_ptr<Rays> ray = std::make_unique<Rays>();
	//std::cout << "after rays" << std::endl;
	std::ifstream sim(Physics::projectQueue[ongoing].projPath / std::filesystem::path("simulations/sim.json"));
	if(!sim.is_open()) 
	{
		std::cout << "file couldn't open" << std::endl;
	}
	json sj = json::parse(sim);
	//std::cout << "finished parsing" << std::endl;
	bool rays{true};
	uint rayi{};
	while (rays)
	{
		//std::cout << "enter while loop" << std::endl;
		if (sj.contains("ray" + std::to_string(rayi)))
		{
			ray->xcoords.push_back(sj["ray" + std::to_string(rayi)]["end"][0]);
			ray->ycoords.push_back(sj["ray" + std::to_string(rayi)]["end"][1]);
			ray->zcoords.push_back(sj["ray" + std::to_string(rayi)]["end"][2]);
			ray->stpxcoords.push_back(sj["ray" + std::to_string(rayi)]["start"][0]);
			ray->stpycoords.push_back(sj["ray" + std::to_string(rayi)]["start"][1]);
			ray->stpzcoords.push_back(sj["ray" + std::to_string(rayi)]["start"][2]);

			ray->tfoa.emplace_back(std::pair<float, float>{sj["ray" + std::to_string(rayi)]["frame"][0].get<float>(), sj["ray" + std::to_string(rayi)]["frame"][1].get<float>()});
			//std::cout << "active frame " << ray->tfoa.back().first << " " << ray->tfoa.back().second << std::endl;
			ray->single_tick.emplace_back(sj["ray" + std::to_string(rayi)]["tick"]);

			ray->expressed_force.emplace_back(sj["ray" + std::to_string(rayi)]["expressed"]);
			rayi++;
		}
		else 
		{
			rays = false;
		}
	}
	if(ray->xcoords.empty()) return;
	//std::cout << "made sure, that rays isn't empty" << std::endl;
	sim.close();

	// ------ creating opencl objects ------- (without proper separation the code looks disgusting, trust)
	std::ifstream kernel_file(RADON_SOURCE_DIR / std::filesystem::path("core/backend_src/cl/BVH.cl"));
	std::stringstream kss;
	kss << kernel_file.rdbuf();
	std::unique_ptr<MyCL::Basic> BVH = std::make_unique<MyCL::Basic>(kss.str().c_str(), kss.str().length(), "BVH");
	kernel_file.close();
	// ------ end of opencl object creation ------
	
	std::ofstream osim(Physics::projectQueue[ongoing].projPath / std::filesystem::path("simulations/sim.json"));
	if(!osim.is_open()) 
	{
		std::cout << "output file couldn't open" << std::endl;
	}
	
	
	
	for (size_t j{}; j < Physics::projectQueue[ongoing].simPointer->action.size(); j++)
	{
		Physics::projectQueue[ongoing].simPointer->action[j].cs->mass = Physics::projectQueue[ongoing].simPointer->action[j].cs->volume * 2700;
		std::cout << "added mass " <<  Physics::projectQueue[ongoing].simPointer->action[j].cs->mass << std::endl;
		sj["Initial"]["Body" + std::to_string(j)]["COM"] = {Physics::projectQueue[ongoing].simPointer->action[j].cs->COM[0], Physics::projectQueue[ongoing].simPointer->action[j].cs->COM[1], Physics::projectQueue[ongoing].simPointer->action[j].cs->COM[2]};
	}


	float timepertile = std::chrono::duration<float>(Physics::projectQueue[ongoing].simPointer->simdur).count() / Physics::projectQueue[ongoing].simPointer->tiles;	
	std::cout << "dt: " << timepertile << '\n';

	Vector vc;
	std::array<float, 3> temporaryCOM{0.00f, 0.00f, 0.00f};
	for (size_t i{}; i < Physics::projectQueue[ongoing].simPointer->tiles; i++)
	{
		std::cout << "tile " << i << std::endl;
		
		for (size_t j{}; j < Physics::projectQueue[ongoing].simPointer->action.size(); j++)
		{
			if(i == 0) 
			{
				Physics::projectQueue[ongoing].simPointer->action[j].vel_dir = std::array<float, 3>{0.0f, 0.0f, 0.0f};
			}

			std::vector<MyCL::Hit> current_hit = MyCL::BVH(Physics::projectQueue[ongoing].simPointer->action[j].cs, *ray, nullptr, BVH);
			std::cout << "current hit size" << current_hit.size() << std::endl;
			for (size_t k{}; k < current_hit.size(); k++)
			{
				std::cout << "triangle " << current_hit[k].triangle_index << std::endl;
				std::cout << "ray " << current_hit[k].ray_index << std::endl;
				std::cout << "hitpos " << current_hit[k].hitpos.s[0] << " " << current_hit[k].hitpos.s[1] << " " << current_hit[k].hitpos.s[2] << " " << std::endl;
			}
				for (size_t k{}; k < current_hit.size(); k++)
				{
					uint ray_idx = current_hit[k].ray_index;

					bool isActiveTick = (ray->single_tick[ray_idx] - 1 == i) ||
										((std::round(ray->tfoa[ray_idx].first * timepertile * 1000) - 1) <= i &&
										 i <= (std::round(ray->tfoa[ray_idx].second * timepertile * 1000) - 1) &&
										 (std::round(ray->tfoa[ray_idx].second + std::round(ray->tfoa[ray_idx].first))) > 0);
					// std::cout << "active tick 1: " << std::round(ray->tfoa[ray_idx].first * timepertile) - 1 << std::endl;
					// std::cout << "active tick 2: " << std::round(ray->tfoa[ray_idx].second * timepertile) - 1 << std::endl;

					if (isActiveTick)
					{
						if (k > 0 && current_hit[k].ray_index == current_hit[k - 1].ray_index)
							continue;
						std::cout << "force: " << ray->expressed_force[ray_idx] << '\n';

						float hit_x = current_hit[k].hitpos.s[0];
						float hit_y = current_hit[k].hitpos.s[1];
						float hit_z = current_hit[k].hitpos.s[2];

						if ((hit_x * temporaryCOM[0] +
							 hit_y * temporaryCOM[1] +
							 hit_z * temporaryCOM[2]) < 1e-3)
						{
							float acceleration = ray->expressed_force[ray_idx] / Physics::projectQueue[ongoing].simPointer->action[j].cs->mass;
							float vel_delta = acceleration * timepertile;
							std::cout << "acceleration: " << acceleration << '\n';

							std::cout << "vel_delta: " << vel_delta << '\n';

							float len = vc.lengthVector(
								ray->xcoords[ray_idx], ray->ycoords[ray_idx], ray->zcoords[ray_idx],
								ray->stpxcoords[ray_idx], ray->stpycoords[ray_idx], ray->stpzcoords[ray_idx]);

							if (len > 1e-7f)
							{
								Physics::projectQueue[ongoing].simPointer->action[j].vel_dir[0] += ((ray->xcoords[ray_idx] - ray->stpxcoords[ray_idx]) / len) * vel_delta;
								Physics::projectQueue[ongoing].simPointer->action[j].vel_dir[1] += ((ray->ycoords[ray_idx] - ray->stpycoords[ray_idx]) / len) * vel_delta;
								Physics::projectQueue[ongoing].simPointer->action[j].vel_dir[2] += ((ray->zcoords[ray_idx] - ray->stpzcoords[ray_idx]) / len) * vel_delta;
								sj["tick" + std::to_string(i)]["Body" + std::to_string(j)]["rayhit"] = ray_idx;
								std::cout << "velocity: "
										  << Physics::projectQueue[ongoing].simPointer->action[j].vel_dir[0] << " "
										  << Physics::projectQueue[ongoing].simPointer->action[j].vel_dir[1] << " "
										  << Physics::projectQueue[ongoing].simPointer->action[j].vel_dir[2] << '\n';
							}
						}
					}
			}
			
			// Generate a single travel displacement vector using the updated velocity
			std::vector<std::array<float, 3>> travel;
			travel.emplace_back(std::array<float, 3>{
				Physics::projectQueue[ongoing].simPointer->action[j].vel_dir[0] * timepertile,
				Physics::projectQueue[ongoing].simPointer->action[j].vel_dir[1] * timepertile,
				Physics::projectQueue[ongoing].simPointer->action[j].vel_dir[2] * timepertile
			});

			// Apply force (which now correctly acts as spatial displacement logic)
			std::array<float, 3> displace{0.00f, 0.00f, 0.00f};
			for (size_t i{}; i < travel.size(); i++)
			{
				displace[0] += travel[i][0];
				displace[1] += travel[i][1];
				displace[2] += travel[i][2];
				//std::cout << "applicable " << applicable[i][0] << " " << applicable[i][1] << " " << applicable[i][2] << std::endl;
			}
			temporaryCOM[0] += displace[0];
			temporaryCOM[1] += displace[1];
			temporaryCOM[2] += displace[2];

			sj["tick" + std::to_string(i)]["Body" + std::to_string(j)]["COM"] = {
				temporaryCOM[0],
				temporaryCOM[1],
				temporaryCOM[2]};
			sj["tick" + std::to_string(i)]["Body" + std::to_string(j)]["Displacement(com)"] = {displace[0], displace[1], displace[2]};
			//std::cout << "mass: " << Physics::projectQueue[ongoing].simPointer->action[j].cs->mass << '\n';
			
		}
	}
	//std::cout << "dumping json" << std::endl;
	osim << sj.dump(4);
	//std::cout << "finished dumping" << std::endl;
	osim.close();
	//std::cout << "closed file" << std::endl;
}
