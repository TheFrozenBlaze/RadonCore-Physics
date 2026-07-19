#include "clfunc.h"
#include "misc.h"

extern "C" void ForceApply(SimDet& det) {
    cl::Platform def_platform = MyCL::platforms[0];
	cl::Device def_device = MyCL::devices[0];
	cl::Context context({def_device});
	cl::Program::Sources source;
	std::array<float, 3> applyforce {0.f, 0.f, 0.f};
	for (size_t i{}; i < det.forces.size(); i++)
	{
		applyforce += det.forces[i];
	}
	std::string kernel_code = "__kernel void force_apply(__global const float* coords, __global float* out, __constant float* force)"
							  "{"
							  "uint i;"
							  "i = get_global_id(0);"
							  "out[i*3] = force.x + coords[i*3];"
							  "out[i*3+1] = force.y + coords[i*3+1];"
							  "out[i*3+2] = force.z + coords[i*3+2];"
							  "}";
	source.push_back({kernel_code.c_str(), kernel_code.length()});
	cl::Program program(context, source);
	 if (program.build({def_device}) != CL_SUCCESS) {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(def_device) << std::endl;
        exit(1);
    }
	cl::Buffer point_buffer(context, CL_MEM_READ_ONLY | CL_MEM_HOST_PTR, sizeof(float) * det.cs.vpc.size(), det.cs.vpc.data());
	cl::Buffer OutBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_PTR, sizeof(float) * det.cs.vpc.size());
	cl::Buffer apply(context, CL_MEM_READ_ONLY | CL_MEM_HOST_PTR, sizeof(float) * 3, applyforce.data());
	cl::CommandQueue queue(context, def_device);
	queue.enqueueWriteBuffer(point_buffer, CL_TRUE, 0, sizeof(float)*det.cs.vpc.size(), det.cs.vpc.data());
	queue.enqueueWriteBuffer(apply, CL_TRUE, 0, sizeof(float) * 3, applyforce.data());
	cl::KernelFunctor force_apply(cl::Kernel(program, "force_apply"), queue, cl::NullRange, cl::NDRange(det.cs.vpc.size()), cl::NullRange);
	force_apply(point_buffer, OutBuffer, apply);
	det.cs.vpc.clear();
	queue.enqueueReadBuffer(point_buffer, CL_TRUE, 0, sizeof(float) * det.cs.vpc.size(), det.cs.vpc.data());
}

