#ifndef CLFUNC_H
#define CLFUNC_H
#include "Material.h"
#include "3dvfunc.h"
namespace MyCL 
{
	typedef struct 
	{
		uint32_t triangle_index;
		uint32_t ray_index;
		cl_float3 hitpos;
	} Hit;
	struct Basic
	{
		cl::Platform def_platform;
		cl::Device def_device;
		cl::Context context;
		cl::Program::Sources source;
		cl::Program program;
		cl::Kernel kernel;
		cl::CommandQueue queue;
		Basic(const char* kernel_str, size_t&& length, std::string kernel_name) 
		{

			def_platform = MyCL::platforms[0];
			def_device = MyCL::devices[0];
			context = cl::Context({def_device});
			source.emplace_back(kernel_str, length);
			program = cl::Program(context, source);
			queue = cl::CommandQueue(context, def_device);

			if (program.build({def_device}) != CL_SUCCESS) {
				std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(def_device) << std::endl;
				exit(1);
			}
			kernel = cl::Kernel(program, kernel_name.c_str());
		};
	};
	// extern void MeshPopulator(Coord &cs, uint8_t precisiongradident);
	std::vector<MyCL::Hit> BVH(std::unique_ptr<Coord> &cs, Rays &rays, std::vector<float> *spec_rays, std::unique_ptr<Basic>& demand);
	std::array<float, 3> ForceApply(std::unique_ptr<Coord>& cs, std::vector<std::array<float, 3>>& applicable, std::unique_ptr<Basic>& demand);
	
}
#endif 
