#include <iostream>
#include <cassert> 
#include "physics.h"

std::vector<cl::Platform> MyCL::platforms;
std::vector<cl::Device> MyCL::devices;
void MyCL::OpenCLInit()
{
    cl::Platform::get(&MyCL::platforms);
	assert(!MyCL::platforms.empty() && "No platforms found");	
	MyCL::devices.reserve(MyCL::platforms.size());
	for (size_t i{}; i < MyCL::platforms.size(); i++)
	{
		MyCL::platforms[i].getDevices(CL_DEVICE_TYPE_GPU, &MyCL::devices);
		
	}
	assert(!MyCL::devices.empty() && "No GPU? No simulation bozo");
};