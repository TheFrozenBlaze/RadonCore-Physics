#include "clfunc.h"
#include "misc.h"

std::array<float, 3> MyCL::ForceApply(std::unique_ptr<Coord>& cs, std::vector<std::array<float, 3>>& applicable, std::unique_ptr<MyCL::Basic>& demand) {
	cl_int err;
	
	cl_float3 applyforce{0.0000f, 0.0000f, 0.0000f};
	for (size_t i{}; i < applicable.size(); i++)
	{
		applyforce.s[0] += applicable[i][0];
		applyforce.s[1] += applicable[i][1];
		applyforce.s[2] += applicable[i][2];
		//std::cout << "applicable " << applicable[i][0] << " " << applicable[i][1] << " " << applicable[i][2] << std::endl;
	}

	cl::Buffer point_buffer(demand->context, CL_MEM_READ_WRITE, sizeof(float) * cs->vpc.size());
	cl::Buffer apply(demand->context, CL_MEM_READ_ONLY, sizeof(cl_float3));
	err = demand->queue.enqueueWriteBuffer(point_buffer, CL_TRUE, 0, sizeof(float)*cs->vpc.size(), cs->vpc.data());
	if(err != CL_SUCCESS) std::cout << "writing point buffer " << err << std::endl;
	err = demand->queue.enqueueWriteBuffer(apply, CL_TRUE, 0, sizeof(cl_float3), &applyforce);
	if(err != CL_SUCCESS) std::cout << "force apply " << err << std::endl;
	static cl::Kernel fa_kernel = demand->kernel;
	fa_kernel.setArg(0, point_buffer);
	fa_kernel.setArg(1, apply);
	demand->queue.enqueueNDRangeKernel(fa_kernel, cl::NullRange, cl::NDRange(cs->vpc.size() / 3), cl::NullRange);
	demand->queue.finish();
	err = demand->queue.enqueueReadBuffer(point_buffer, CL_TRUE, 0, sizeof(float) * cs->vpc.size(), cs->vpc.data());
	if(err != CL_SUCCESS) std::cout << "reading point buffer " << err << std::endl;
	return std::array<float, 3>{applyforce.x, applyforce.y, applyforce.z};
}
