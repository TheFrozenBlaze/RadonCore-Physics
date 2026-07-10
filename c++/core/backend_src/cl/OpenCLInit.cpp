#include <iostream>
#include <cassert> 
#include <CL/opencl.hpp>
#include "clfunc.h"
#include "3dvfunc.h"
std::vector<cl::Platform> MyCL::platforms;
std::vector<cl::Device> MyCL::devices;
bool OpenCLInit()
{
    cl::Platform::get(&MyCL::platforms);
	assert(!MyCL::platforms.empty() && "No platforms found");	
	
	for (size_t i{}; i < MyCL::platforms.size(); i++)
	{
		MyCL::platforms[i].getDevices(CL_DEVICE_TYPE_GPU, &MyCL::devices);
	}
	assert(!MyCL::devices.empty() && "No GPU? No simulation bozo");
};