#include "clfunc.h"

std::vector<MyCL::Hit> MyCL::BVH(std::unique_ptr<Coord>& cs, Rays& rays, std::vector<float>* spec_rays, std::unique_ptr<MyCL::Basic>& demand) {

	cl_int err;
	
	std::vector<cl_float4> clraystp;
	std::vector<cl_float4> clray;

	size_t ray_count = (spec_rays == nullptr) ? rays.xcoords.size() : spec_rays->size();
	clraystp.reserve(ray_count);
	clray.reserve(ray_count);

	for (size_t i = 0; i < ray_count; ++i) {
		size_t idx = (spec_rays == nullptr) ? i : (*spec_rays)[i];
		clraystp.push_back(cl_float4{rays.stpxcoords[idx], rays.stpycoords[idx], rays.stpzcoords[idx], 0.0f});
		clray.push_back(cl_float4{rays.xcoords[idx], rays.ycoords[idx], rays.zcoords[idx], 0.0f});
	}

	uint32_t num_triangles = static_cast<uint32_t>(cs->triangles.size() / 3);
	uint32_t num_rays = static_cast<uint32_t>(clray.size());

	cl::Buffer triangles_buf(demand->context, CL_MEM_READ_ONLY, cs->triangles.size() * sizeof(uint32_t));
	cl::Buffer points_buf(demand->context, CL_MEM_READ_ONLY, cs->vpc.size() * sizeof(float));
	cl::Buffer raystp_buf(demand->context, CL_MEM_READ_ONLY, clraystp.size() * sizeof(cl_float4));
	cl::Buffer ray_buf(demand->context, CL_MEM_READ_ONLY, clray.size() * sizeof(cl_float4));
	
	cl_uint zero = 0;
	cl::Buffer counter_buf(demand->context, CL_MEM_READ_WRITE , sizeof(cl_uint));

	std::vector<MyCL::Hit> traversed(num_triangles * num_rays);
	cl::Buffer return_data(demand->context, CL_MEM_WRITE_ONLY, traversed.size() * sizeof(MyCL::Hit));
	
	err = demand->queue.enqueueWriteBuffer(counter_buf, CL_TRUE, 0,sizeof(cl_uint), &zero);
	if(err != CL_SUCCESS) std::cout << "counter_buf " << err << std::endl;
	
	err = demand->queue.enqueueWriteBuffer(triangles_buf, CL_TRUE, 0, cs->triangles.size() * sizeof(uint32_t), cs->triangles.data());
	if(err != CL_SUCCESS) std::cout << "triangles_buf " << err << std::endl;
	err = demand->queue.enqueueWriteBuffer(points_buf, CL_TRUE, 0, cs->vpc.size() * sizeof(float), cs->vpc.data());
	if(err != CL_SUCCESS) std::cout << "points_buf " << err << std::endl;
	err = demand->queue.enqueueWriteBuffer(raystp_buf, CL_TRUE, 0, clraystp.size() * sizeof(cl_float4), clraystp.data());
	if(err != CL_SUCCESS) std::cout << "clraystp " << err << std::endl;
	err = demand->queue.enqueueWriteBuffer(ray_buf, CL_TRUE, 0, clray.size() * sizeof(cl_float4), clray.data());
	if(err != CL_SUCCESS) std::cout << "ray " << err << std::endl;

	cl::Kernel BVH = demand->kernel;
	BVH.setArg(0, triangles_buf);
	BVH.setArg(1, points_buf);
	BVH.setArg(2, return_data);
	BVH.setArg(3, ray_buf);
	BVH.setArg(4, raystp_buf);
	BVH.setArg(5, counter_buf);
	std::cout << "--- HOST BUFFER SANITY CHECK ---" << std::endl;
	std::cout << "Rays count: " << clray.size() << std::endl;
	std::cout << "Ray Start [0]: " << clraystp[0].s[0] << ", " << clraystp[0].s[1] << ", " << clraystp[0].s[2] << std::endl;
	std::cout << "Ray End   [0]: " << clray[0].s[0] << ", " << clray[0].s[1] << ", " << clray[0].s[2] << std::endl;

	std::cout << "Triangles count (indices total): " << cs->triangles.size() << std::endl;
	if (!cs->triangles.empty()) {
		std::cout << "First Triangle Indices: " << cs->triangles[0][0] << ", " << cs->triangles[0][1] << ", " << cs->triangles[0][2] << std::endl;
	}

	std::cout << "Coords count (floats total): " << cs->vpc.size() << std::endl;
	if (cs->vpc.size() >= 3) {
		std::cout << "First Point Coord: (" << cs->vpc[0] << ", " << cs->vpc[1] << ", " << cs->vpc[2] << ")" << std::endl;
	}
	std::cout << "--------------------------------" << std::endl;
	err = demand->queue.enqueueNDRangeKernel(BVH, cl::NullRange, cl::NDRange(num_triangles, num_rays), cl::NullRange);
	if(err != CL_SUCCESS) std::cout << "kernel enqueue  " << err << std::endl;

	cl_uint result_count = 0;
	err = demand->queue.enqueueReadBuffer(counter_buf, CL_TRUE, 0, sizeof(cl_uint), &result_count);
	if(err != CL_SUCCESS) std::cout << "result count " << err << std::endl;
	if (result_count > 0)
	{
		traversed.resize(result_count);
		err = demand->queue.enqueueReadBuffer(return_data, CL_TRUE, 0, result_count * sizeof(MyCL::Hit), traversed.data());
		if(err != CL_SUCCESS) std::cout << "traversed " << err << std::endl;
		
		/*std::sort(traversed.begin(), traversed.end(), [](const MyCL::Hit &a, const MyCL::Hit &b)
				  {
			if (a.ray_index != b.ray_index) {
				return a.ray_index < b.ray_index;
			}
			return a.hitpos < b.hitpos; });*/
	} else {
		traversed.clear();
	}

	return traversed;
}