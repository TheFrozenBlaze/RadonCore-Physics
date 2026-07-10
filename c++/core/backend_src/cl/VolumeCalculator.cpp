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
extern "C" void VolumeCalculator(Coord& cs) {

	cl::Platform def_platform = MyCL::platforms[0];
	cl::Device def_device = MyCL::devices[0];
	cl::Context context({def_device});
	cl::Program::Sources source;
	std::string kernel_code = "__kernel void volume_calc(__global const float* coords, __global const uint* triangles, __global volatile uint* counter, __global float* volume, __global float* com)"
							  "{"
							  "uint i;"
							  "i = get_global_id(0);"
							  "uint t00 = triangles[i * 9 + 0];"
							  "uint t01 = triangles[i * 9 + 1];"
							  "uint t02 = triangles[i * 9 + 2];"
							  "uint t10 = triangles[i * 9 + 3];"
							  "uint t11 = triangles[i * 9 + 4];"
							  "uint t12 = triangles[i * 9 + 5];"
							  "uint t20 = triangles[i * 9 + 6];"
							  "uint t21 = triangles[i * 9 + 7];"
							  "uint t22 = triangles[i * 9 + 8];"
							  "float current_volume = (coords[t00] *(coords[t11]*coords[t22]-coords[t21]*coords[t12]) + coords[t01]*(coords[t12]*coords[t20]-coords[t10]*coords[t22]) + coords[t02]*(coords[t10]*coords[t21]-coords[t11]*coords[t20]))/6;"
							  "volume += current_volume;"
							  "uint index = atomic_add(counter, 1);"
							  "usable[index] = i;"
							  "}";
	source.push_back({kernel_code.c_str(), kernel_code.length()});
	cl::Program program(context, source);
	 if (program.build({def_device}) != CL_SUCCESS) {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(def_device) << std::endl;
        exit(1);
    }
	cl::Buffer triangle_context_buffer(context, CL_MEM_READ_ONLY | CL_MEM_HOST_PTR, sizeof(float)*cs.triangles.size() * 9, cs.triangles.data());
	cl::Buffer triangle_buffer(context, CL_MEM_READ_ONLY | CL_MEM_HOST_PTR, sizeof(float)*cs.vpc.size(), cs.vpc.data());
	cl::Buffer counter(context, CL_MEM_READ_ONLY | CL_MEM_HOST_PTR, sizeof(uint32_t));
	cl::Buffer volume(context, CL_MEM_READ_ONLY | CL_MEM_HOST_PTR, sizeof(double));
	cl::Buffer COM(context, CL_MEM_READ_ONLY | CL_MEM_HOST_PTR, sizeof(float) * 3);
	cl::CommandQueue queue(context, def_device);
	queue.enqueueReadBuffer(triangle_buffer, CL_TRUE, 0, sizeof(float)*cs.vpc.size(), nullptr);
	queue.enqueueReadBuffer(triangle_context_buffer, CL_TRUE, 0, sizeof(uint32_t)*cs.triangles.size() * 3, nullptr);
	queue.enqueueReadBuffer(counter, CL_TRUE, 0, sizeof(uint64_t), nullptr);
	cl::KernelFunctor volume_calc(cl::Kernel(program, "volume_calc"), queue, cl::NullRange, cl::NDRange(cs.triangles.size()), cl::NullRange);
	volume_calc(triangle_buffer, triangle_context_buffer, counter, volume, COM);
	queue.enqueueReadBuffer(volume, CL_TRUE, 0, sizeof(double), nullptr);
	queue.enqueueReadBuffer(COM, CL_TRUE, 0, sizeof(double) * 3, nullptr);
};
