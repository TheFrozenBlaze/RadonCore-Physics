#ifndef CLFUNC_H
#define CLFUNC_H
#include <CL/opencl.hpp>
#include "Material.h"
#include "3dvfunc.h"
#include "time.h"
namespace MyCL 
{
    extern std::vector<cl::Platform> platforms;
    extern std::vector<cl::Device> devices;
    extern bool OpenCLInit();
    extern void MeshPopulator(Coord &cs, uint8_t precisiongradident);
    extern void BVH(Coord &cs, const std::vector<size_t> &rays);
    extern void ForceApply(Coord &cs, SimDet *det);
    extern void VolumeCalc(Coord &cs);
}
#endif 
