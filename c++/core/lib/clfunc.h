#ifndef CLFUNC_H
#define CLFUNC_H
#include <CL/opencl.hpp>
#include "Material.h"
#include "3dvfunc.h"
#include "physics.h"
namespace MyCL 
{
    typedef struct volcalcret
    {
        cl_float4 comandvol;
        cl_float3 inertia;
    };
    extern std::vector<cl::Platform> platforms;
    extern std::vector<cl::Device> devices;
    extern bool OpenCLInit();
    extern void MeshPopulator(Coord &cs, uint8_t precisiongradident);
    extern void BVH(Coord &cs, const std::vector<size_t> &rays);
    extern void ForceApply(SimDet& det);
    extern volcalcret VolumeCalc(Coord &cs);
}
#endif 
