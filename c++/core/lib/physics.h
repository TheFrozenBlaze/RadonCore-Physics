#ifndef PHYSICS_H
#define PHYSICS_H

#include <cstdint>
#include <cstdio>
#include <memory>
//#include "physlayers.h"
#include <chrono>
#include <glad/glad.h>
#include <filesystem>
#include "Material.h"
#include <json.hpp>
#include <json_fwd.hpp>
#include "coord.h"
#include <cmath>

#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120

#include <CL/opencl.hpp>

#ifndef RADON_SOURCE_DIR
	#define RADON_SOURCE_DIR std::filesystem::canonical(std::filesystem::current_path() / "..");
#endif

namespace MyCL {
	extern std::vector<cl::Platform> platforms;
	extern std::vector<cl::Device> devices;
	void OpenCLInit();
}
struct Rays {
  public:
	//endpoint
	std::vector<float> xcoords;
	std::vector<float> ycoords;
	std::vector<float> zcoords;
	//startpoint
	std::vector<float> stpxcoords;
	std::vector<float> stpycoords;
	std::vector<float> stpzcoords;
	std::vector<float> expressed_force;
	std::vector<uint64_t> single_tick;
	//timeframe of actieveness
	std::vector<std::pair<float, float>> tfoa;
	
};
struct SimDet {
	std::vector<std::array<float, 3>> forces;
	uint start;
	uint finish;
	std::unique_ptr<Coord> cs;
	GLuint EBO;
	GLuint lEBO;
	GLuint VBO;
	GLuint VAO;
	std::unique_ptr<Material> bodmat;
	uint8_t meshboost;
	float unit_velocity;
	std::array<float, 3> vel_dir;
};

struct SimDef {
	std::chrono::duration<double, std::milli> simdur;
	uint tiles;
	double system_energy;
	//Material* surmats;
	std::vector<SimDet> action;
	SimDef() {
		simdur = static_cast<std::chrono::duration<double, std::milli>>(0);
		tiles = 0;
		system_energy = static_cast<double>(0);
		action = std::vector<SimDet>{};
	};
};

struct ProjectDef {
	std::filesystem::path projPath;
	std::unique_ptr<SimDef> simPointer;
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
void AddElement(SimDef& def, std::unique_ptr<Coord>&& cs);
void SimulationCollect(size_t ongoing);
// InitSettings PreSimState(SimDef* def);
// bool StartSim(SimDef* def, enum Physics::PhysLayersFlags flags, InitSettings& init);
};
#endif