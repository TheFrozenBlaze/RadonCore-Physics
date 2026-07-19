#include <iostream>
#include <cstdint>
#include <fstream>
#include <cstring>
#include <cassert> 
#include "clfunc.h"
#include "3dvfunc.h"
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#include <CL/opencl.hpp>
extern "C" MyCL::volcalcret MyCL::VolumeCalc(Coord& cs) {

	cl::Platform def_platform = MyCL::platforms[0];
	cl::Device def_device = MyCL::devices[0];
	cl::Context context({def_device});
	cl::Program::Sources source;
	std::string kernel_code = "__kernel void volume_calc(__global const float* coords, __global const uint* triangles, __global float4* out)"
							  "{"
							  "uint i;"
							  "i = get_global_id(0);"
							  "float volume = (coords[triangles[i * 3]] *(coords[triangles[i * 3 + 1] + 1]*coords[triangles[i * 3 + 2]+2]-coords[triangles[i * 3 + 2]+1]*coords[triangles[i * 3 + 1]+2])"
							  "+ coords[triangles[i * 3] + 1]*(coords[triangles[i * 3 + 1]+2]*coords[triangles[i * 3 + 2]]-coords[triangles[i * 3 + 1]]*coords[triangles[i * 3 + 2]+2])"
							  "+ coords[triangles[i * 3] + 2]*(coords[t10]*coords[triangles[i * 3 + 2]+1]-coords[triangles[i * 3 + 1]+1]*coords[triangles[i * 3 + 2]]))/6;"
							  "out[i].w = volume; "
							  "out[i].x = (coords[triangles[i * 3]] + coords[triangles[i * 3 + 1]] + coords[triangles[i * 3 + 2]])/4 * volume;"
							  "out[i].y = (coords[triangles[i * 3] + 1] + coords[triangles[i * 3 + 1]+1] + coords[triangles[i * 3 + 2]+1])/4 * volume;"
							  "out[i].z = (coords[triangles[i * 3] + 2] + coords[triangles[i * 3 + 1]+2] + coords[triangles[i * 3 + 2]+2])/4 * volume;"
							  "}";
	source.push_back({kernel_code.c_str(), kernel_code.length()});
	cl::Program program(context, source);
	 if (program.build({def_device}) != CL_SUCCESS) {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(def_device) << std::endl;
        exit(1);
    }
	std::vector<cl_float4> out(cs.triangles.size());
	cl::Buffer triangle_context_buffer(context, CL_MEM_READ_ONLY | CL_MEM_HOST_PTR, sizeof(uint32_t) * cs.triangles.size() * 3, cs.triangles.data());
	cl::Buffer triangle_buffer(context, CL_MEM_READ_ONLY | CL_MEM_HOST_PTR, sizeof(float)*cs.vpc.size(), cs.vpc.data());
	cl::Buffer OutBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_PTR, sizeof(cl_float4) * cs.triangles.size());
	cl::CommandQueue queue(context, def_device);
	queue.enqueueWriteBuffer(triangle_buffer, CL_TRUE, 0, sizeof(float)*cs.vpc.size(), cs.vpc.data());
	queue.enqueueWriteBuffer(triangle_context_buffer, CL_TRUE, 0, sizeof(uint32_t)*cs.triangles.size() * 3, cs.triangles.data());
	cl::KernelFunctor volume_calc(cl::Kernel(program, "volume_calc"), queue, cl::NullRange, cl::NDRange(cs.triangles.size()), cl::NullRange);
	volume_calc(triangle_buffer, triangle_context_buffer,  OutBuffer);
	queue.enqueueReadBuffer(OutBuffer, CL_TRUE, 0, sizeof(cl_float4) * cs.triangles.size(), out.data());
	float volume;
	std::array<float, 3> COM;

	for (size_t i{}; i < out.size(); i++)
	{
		
		COM[0] += out[i].x;
		COM[1] += out[i].y;
		COM[2] += out[i].z;
		volume += out[i].w;
	}
	return  {
		cl_float4{COM[0], COM[1], COM[2], volume}, cl_float3{}
	};
};
