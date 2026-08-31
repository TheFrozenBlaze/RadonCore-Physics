#ifndef THREEVFUNC_H
#define THREEVFUNC_H

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>
#include <cassert>
#include <filesystem>
#include "physics.h"




class Vector
{
public:
		/*float a = 0.0f;
		float b = 0.0f;
		float c = 0.0f;
		float m = 0.0f; // steepness
		float cc = 0.0f; // function crossing the given axis*/

		/*enum class crdsysdim {
				XY,
				XZ,
				ZY
		};*/

		// Calculate vector length
	float lengthVector(float x, float y, float z, float stpx, float stpy, float stpz) {
		float ydxz = std::sqrt((x - stpx) * (x - stpx) + (y - stpy) * (y - stpy) + (z - stpz) * (z - stpz));
		return ydxz;
	}
};
class objIdent {
public:
inline uint64_t MakeEdge(uint32_t a, uint32_t b)
{
		if (a > b)
				std::swap(a, b);

		return (static_cast<uint64_t>(a) << 32) | b;
} 
		std::unique_ptr<Coord> objReader(const std::string &filename) {
			std::unique_ptr<Coord> cs = std::make_unique<Coord>();
			std::array<float, 3> minx, maxx, miny, maxy, minz, maxz;
			std::ifstream file(filename);
			if (!file.is_open())
			{
				std::cout << "Error reading file: " << filename << std::endl;
				EXIT_FAILURE;
			}

			cs->name = std::filesystem::path(filename).stem().string();
			std::string line;
			while (std::getline(file, line))
			{
				if (line.size() > 1 && line[0] == 'v' && line[1] == ' ')
				{
					std::istringstream iss(line);
					std::string v;
					float x, y, z;
					iss >> v >> x >> y >> z;
					minx = x < minx[0] ? std::array<float, 3>{x, y, z} : minx;
					maxx = x > maxx[0] ? std::array<float, 3>{x, y, z} : maxx;
					miny = y < miny[1] ? std::array<float, 3>{x, y, z} : miny;
					maxy = y > maxy[1] ? std::array<float, 3>{x, y, z} : maxy;
					minz = z < minz[2] ? std::array<float, 3>{x, y, z} : minz;
					maxz = z > maxz[2] ? std::array<float, 3>{x, y, z} : maxz;

					cs->vpc.push_back(x);
					cs->vpc.push_back(y);
					cs->vpc.push_back(z);
					// std::cout << "added: " << x << " " << y << " " << z << std::endl;
				}
				else if (line.size() > 1 && line[0] == 'f' && line[1] == ' ')
				{
					line.erase(0, 2);      // cuts of the 'f' and the ' ' from the beginning
					bool skip = {false}; // where the f-function writes its outputs (((2=v, 3=vt, 4=vn))
					int number = 0;
					std::string numberstring; // loads the integers here

					for (size_t i {}; i < line.size(); i++)
					{
							if (line[i] == ' ' || i == line.size() - 1)
							{ // where to write
									if (i == line.size() - 1 && line[i] != ' ')
									{ // end of line write
										if(skip == false) {
											numberstring.push_back(line[i]);
											cs->face.emplace_back(std::stoi(numberstring) - 1);
											numberstring.clear();
											//std::cout << "faceback" << cs->face.back() << std::endl;
											//std::cout << "before triangulator" << std::endl;
											
											//std::cout << "after triangulator" << std::endl;
										}
										Triangulator(*cs);
										cs->face.clear();
									}
									else if (line[i] == ' ' && i != line.size() - 1)
									{
										if(numberstring.empty() == 1){
										skip = false;
										continue;
										} else {
											cs->face.emplace_back(std::stoi(numberstring) - 1);                    
											numberstring.clear();
										}
									}
									
							} else {
								if (line[i] == '/')
									{
										if(numberstring.empty())
											continue;
										cs->face.emplace_back(std::stoi(numberstring) - 1);
										numberstring.clear();
										skip = 1;
										continue;
									} else {
										if(skip == true)
											continue;
										numberstring.push_back(line[i]);
									}
							}
						}
					} 
					else 
					{
						continue;
					}
				}
		cs->vpc.shrink_to_fit();
		//cs.face.shrink_to_fit();
		if(cs->vpc.size() == 0) {
			std::cout << "Something went wrong with the coords" << std::endl;
			EXIT_FAILURE;
		}
		/*if(cs.face.size() == 0) {
			std::cout << "something went wrong with the faces" << std::endl;
			std::cout << cs.face.size() << std::endl;
			return false;
		}*/
		for (size_t i{}; i < cs->triangles.size(); i++)
		{
			std::cout << "triangle " << cs->triangles[i][0] << " " << cs->triangles[i][1] << " " << cs->triangles[i][2] << std::endl;
		}

		cs->detailes[0] = minx;
		cs->detailes[1] = maxx;
		cs->detailes[2] = miny;
		cs->detailes[3] = maxy;
		cs->detailes[4] = minz;
		cs->detailes[5] = maxz;
		volcalcret vol = VolumeCalc(cs);
		//std::cout << "volume by default " << vol.comandvol.s[3] << std::endl;
		//std::cout << "COM " << vol.comandvol.s[0] << " " << vol.comandvol.s[1] << " " << vol.comandvol.s[2] << std::endl;

		cs->COM = std::array<float, 3>{vol.comandvol.s[0], vol.comandvol.s[1], vol.comandvol.s[2]};
		cs->volume = vol.comandvol.s[3];
		return cs;
		};

	// Check vector intersection
	/*bool vectorCheck(const std::string& level, int vecnum) {
			Vector::crdsysdim sst;
			 std::transform(level.begin(), level.end(), level.begin(), [](unsigned
	char c) { return std::toupper(c); }); if (level == "XY") {sst =
	Vector::crdsysdim::XY;
			}
			else if (level == "XZ")
			{sst = Vector::crdsysdim::XZ;
			}
			else {sst = Vector::crdsysdim::ZY;}


			float check;
			int above = 0;
			int below = 0;

			vec.funcVector(vecnum, level);

			for (size_t i = 0; i < cs.vxc.size(); i++) {
					switch (sst) {
							case Vector::crdsysdim::XY:
									check = (cs.vxc[i] * vec.m) + vec.cc;
									if (check == cs.vyc[i]) return true;
									else if (check > cs.vyc[i]) above++;
									else below++;
									break;


							case Vector::crdsysdim::XZ:
									check = (cs.vxc[i] * vec.m) + vec.cc;
									if (check == cs.vzc[i]) return true;
									else if (check > cs.vzc[i]) above++;
									else below++;
									break;

							case Vector::crdsysdim::ZY:
									check = (cs.vzc[i] * vec.m) + vec.cc;
									if (check == cs.vyc[i]) return true;
									else if (check > cs.vyc[i]) above++;
									else below++;
									break;
					}

					if (above > 0 && below > 0)
							return true;
			}
			return false;
	}*/

	/*this would be good if this was either 2d, or 3d with simple objects like
	 * cubes or whatever*/

	/*void AABB(uint32_t vecnum, uint32_t coordnum) {
		Coord& cs = objects[coordnum];
		Rays ray;
		for (size_t i = 0; i < cs.triangles.size(); i++)
		{
			std::vector<float> temporaryx;
			std::vector<float> temporaryz;
			std::vector<float> temporaryy;

			temporaryx = {cs.vxc[cs.triangles[i][0]], cs.vxc[cs.triangles[i][1]],
										cs.vxc[cs.triangles[i][2]]};
			temporaryy = {cs.vyc[cs.triangles[i][0]], cs.vyc[cs.triangles[i][1]],
										cs.vyc[cs.triangles[i][2]]};
			temporaryz = {cs.vzc[cs.triangles[i][0]], cs.vzc[cs.triangles[i][1]],
										cs.vzc[cs.triangles[i][2]]};
			auto mmx = std::minmax_element(temporaryx.begin(), temporaryx.end());
			auto mmy = std::minmax_element(temporaryy.begin(), temporaryy.end());
			auto mmz = std::minmax_element(temporaryz.begin(), temporaryz.end());
			float xmin = *mmx.first, xmax = *mmx.second;
			float ymin = *mmy.first, ymax = *mmy.second;
			float zmin = *mmz.first, zmax = *mmz.second;

			float vxmin = std::min(cs.xcoords[vecnum], cs.stpxcoords[vecnum]);
			float vxmax = std::max(cs.xcoords[vecnum], cs.stpxcoords[vecnum]);
			float vymin = std::min(cs.ycoords[vecnum], cs.stpycoords[vecnum]);
			float vymax = std::max(cs.ycoords[vecnum], cs.stpycoords[vecnum]);
			float vzmin = std::min(cs.zcoords[vecnum], cs.stpzcoords[vecnum]);
			float vzmax = std::max(cs.zcoords[vecnum], cs.stpzcoords[vecnum]);

			if (xmax < vxmin || xmin > vxmax)
				continue;
			if (ymax < vymin || ymin > vymax)
				continue;
			if (zmax < vzmin || zmin > vzmax)
				continue;

			cs.usable.push_back(i);
		}
		cs.usable.shrink_to_fit();
	};*/


	struct volcalcret
	{
		cl_float4 comandvol;
		cl_float3 inertia;
	};

	volcalcret VolumeCalc(std::unique_ptr<Coord>& cs) {
		cl_int err;
		
		std::vector<cl::Device> devices;
		std::vector<cl::Platform> platforms;
		cl::Platform def_platform = MyCL::platforms[0];
		cl::Device def_device = MyCL::devices[0];
		cl::Context context({def_device});
		cl::Program::Sources source;
		std::ifstream kernel_code(RADON_SOURCE_DIR / std::filesystem::path("core/backend_src/cl/VolumeCalc.cl"));
		std::stringstream kss;
		kss << kernel_code.rdbuf();
		source.emplace_back(kss.str().c_str(), kss.str().length());
		cl::Program program(context, source);
		if (program.build({def_device}) != CL_SUCCESS)
		{
			std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(def_device) << std::endl;
			exit(1);
		}
	std::vector<cl_float4> out(cs->triangles.size());
	cl::Buffer triangle_context_buffer(context, CL_MEM_READ_ONLY, sizeof(uint32_t) * cs->triangles.size() * 3);
	cl::Buffer triangle_buffer(context, CL_MEM_READ_ONLY, sizeof(float)*cs->vpc.size());
	cl::Buffer OutBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float4) * cs->triangles.size());
	cl::CommandQueue queue(context, def_device);
	cl::Kernel volume_calc(program, "volume_calc");
	err = queue.enqueueWriteBuffer(triangle_buffer, CL_TRUE, 0, sizeof(float)*cs->vpc.size(), cs->vpc.data());
	std::cout << "triangle buffer enqueue" << err << std::endl;
	err = queue.enqueueWriteBuffer(triangle_context_buffer, CL_TRUE, 0, sizeof(uint32_t)*cs->triangles.size() * 3, cs->triangles.data());
	std::cout << "triangle context buffer enqueue" << err << std::endl;
	volume_calc.setArg(0, triangle_buffer);
	volume_calc.setArg(1, triangle_context_buffer);
	volume_calc.setArg(2, OutBuffer);
	err = queue.enqueueNDRangeKernel(volume_calc,cl::NullRange, cl::NDRange(cs->triangles.size()), cl::NullRange);
	std::cout << "enqueue kernel" << err << std::endl;
	err = queue.finish();
	std::cout << "finish kernel" << err << std::endl;
	queue.enqueueReadBuffer(OutBuffer, CL_TRUE, 0, sizeof(cl_float4) * cs->triangles.size(), out.data());
	float volume {};
	std::array<float, 3> COM{0,0,0};

	for (size_t i{}; i < out.size(); i++)
	{
		/*std::cout
		<< i
		<< "  V=" << out[i].w
		<< "  M=("
		<< out[i].x << ", "
		<< out[i].y << ", "
		<< out[i].z << ")" << std::endl;*/
		COM[0] += out[i].x;
		COM[1] += out[i].y;
		COM[2] += out[i].z;
		volume += out[i].w;
	}
	COM[0] /= volume;
	COM[1] /= volume;
	COM[2] /= volume;
	return  {
		cl_float4{COM[0], COM[1], COM[2], volume}, cl_float3{}
	};
	}
	// I would like to give credits and a huge thanks to 3Blue1Brown, who has
	// taught me the essence of algebra, and who taught me how to think in 3D.
	// Without his series creating this function would have been a much more
	// excruciating process, and I would have probably never finished it, so thank
	// you 3Blue1Brown!
	void Triangulator(Coord& cs) {
		std::unordered_set<uint64_t> edgeIndexSet;

		float limit = 1e-6; // basicaly zero
		std::array<float, 3> normal = {0.0, 0.0, 0.0};
		uint8_t projection = 0;
		//std::cout << "face.size() " << cs.face.size() << std::endl;
		/*for(size_t i {}; i < cs.face.size(); i++)
		{
			//std::cout << "face" << i << " " << cs.face[i] << std::endl;
		}*/
		if (cs.face.size() > 3)
		{
			normal = {0.0f, 0.0f, 0.0f};
			float x, y, z, coordy, coordx, coordz;
			int f = 1;
			while (
					normal ==
					std::array<float, 3>{
							0.0f, 0.0f,
							0.0f})
			{ // calculatin the normal of the face based on it's
				// vertices, if the normal is 0, then the face is
				// degenerate and we will try to calculate the normal
				// with different vertices until we get a non
				// degenerate normal or we run out of vertices to try
				if (f + 1 == cs.face.size())
				{
					std::cout << "INVALID FACE" << std::endl;
					break;
				}
				else
				{
					coordx = (cs.vpc[cs.face[f] * 3 + 1] -
										cs.vpc[cs.face[0] * 3 + 1]) *
											 (cs.vpc[cs.face[1 + f] * 3 + 2] -
												cs.vpc[cs.face[0] * 3 + 2]) -
									 (cs.vpc[cs.face[1 + f] * 3 + 1] -
										cs.vpc[cs.face[0] * 3 + 1]) *
											 (cs.vpc[cs.face[f] * 3 + 2] -
												cs.vpc[cs.face[0] * 3 + 2]);
					coordy = (cs.vpc[cs.face[f] * 3 + 2] -
										cs.vpc[cs.face[0] * 3 + 2]) *
											 (cs.vpc[cs.face[1 + f] * 3] -
												cs.vpc[cs.face[0] * 3]) -
									 (cs.vpc[cs.face[1 + f] * 3 + 2] -
										cs.vpc[cs.face[0] * 3 + 2]) *
											 (cs.vpc[cs.face[f] * 3] -
												cs.vpc[cs.face[0] * 3]);
					coordz = (cs.vpc[cs.face[f] * 3] -
										cs.vpc[cs.face[0] * 3]) *
											 (cs.vpc[cs.face[1 + f] * 3 + 1] -
												cs.vpc[cs.face[0] * 3 + 1]) -
									 (cs.vpc[cs.face[1 + f] * 3] -
										cs.vpc[cs.face[0] * 3]) *
											 (cs.vpc[cs.face[f] * 3 + 1] -
												cs.vpc[cs.face[0] * 3 + 1]);
					x = limit > std::fabs(coordx) && std::fabs(coordx) >= 0 ? 0.0f : coordx;
					y = limit > std::fabs(coordy) && std::fabs(coordy) >= 0 ? 0.0f : coordy;
					z = limit > std::fabs(coordz) && std::fabs(coordz) >= 0 ? 0.0f : coordz;
					normal = {x, y, z};
					f++;
				}
			}
			//std::cout << "Normal" << normal[0] << " Normal0" <<  normal[1] << " Normal1" << normal[2] << "Normal2" << std::endl;
			f = 1;
			float min = std::fabs(*std::min_element(normal.begin(), normal.end())),
						max = std::fabs(*std::max_element(
								normal.begin(),
								normal.end())); // the absolute biggest element defines, which
																// way would the projection distort the least,
																// because of perpendicularity
			// if a normal is more parallel with the, let's say, x, then the
			// projection on the yz plane would be distorted the least => eliminates
			// the risk of wrong detection
			int8_t dor =
					limit >= max - min && max - min >= 0 ? 0 : (max - min > 0 ? 1 : -1);
			projection =
					dor == 0
							? std::distance(normal.begin(),
															std::max_element(normal.begin(), normal.end()))
					: dor > 0
							? std::distance(normal.begin(),
															std::max_element(normal.begin(), normal.end()))
							: std::distance(
										normal.begin(),
										std::min_element(
												normal.begin(),
												normal.end())); // if the absolutes of the min and max
																				// are equal, then it's just easier to
																				// choose max
			//std::cout << "projection" << (int)projection << std::endl;
			std::list<uint32_t> vindices;        // list for easy traversal and deletion of vertices
			uint32_t v0, v1, v2;                 // used for triangle math
			std::unordered_set<uint32_t> reflex; // reflex vertices, unordered set is useful because of find
			std::array<float, 2> rv1;            // used for normal math
			std::array<float, 2> rv2;
			 //std::cout << "I made it here" << std::endl;
			switch (projection)
			{
			case 0:
			{
				std::list<uint32_t>::iterator back;
					// we are ordering here based on winding. The normal
					// tells us, which way are the vertices ordered
					for (size_t j{}; j < cs.face.size(); j++)
					{
						if (j < 2)
						{
							vindices.push_back(cs.face[j]);
						}
						else if (j >= 2 && j < cs.face.size() - 1)
						{
							vindices.push_back(cs.face[j]);
							v2 = cs.face[j - 2]; // caLculating normals=> optimization compared to the last version
							v1 = cs.face[j - 1];
							v0 = cs.face[j];
							rv1 = {cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1], cs.vpc[v1 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							rv2 = {cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1], cs.vpc[v0 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
							{
								reflex.insert(j - 1);
							}
						}
						else
						{
							vindices.push_back(cs.face[j]);
							v2 = cs.face[j - 2];
							v1 = cs.face[j - 1];
							v0 = cs.face[j];
							rv1 = {cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1], cs.vpc[v1 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							rv2 = {cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1], cs.vpc[v0 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
							{
								reflex.insert(j - 1);
							} // until here, the cycle is same as above
							v2 = v1;
							v1 = v0;
							v0 = cs.face[0];
							rv1 = {cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1], cs.vpc[v1 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							rv2 = {cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1], cs.vpc[v0 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
							{
								reflex.insert(j); // calculating the reflexness of the last vertex
							}
							v2 = v1;
							v1 = v0;
							v0 = cs.face[1];
							rv1 = {cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1], cs.vpc[v1 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							rv2 = {cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1], cs.vpc[v0 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
							{
								reflex.insert(0); // calculating the reflexness of the first vertex => all reflexness is calculated
							}
						}
					}

				{
					std::list<uint32_t>::iterator step = {
							std::next(vindices.begin(), 1)};
					uint32_t reflexelement{0};
					uint32_t switcher;
					bool valid;
					std::list<uint32_t>::iterator firstelement{vindices.begin()};
					std::list<uint32_t>::iterator nextnext{
							std::next(vindices.begin(), 1)};
					std::list<uint32_t>::iterator prev1{std::prev(vindices.end(), 1)};
					std::list<uint32_t>::iterator prev2{std::prev(vindices.end(), 2)};
					// std::cout << "I made it to while loop in case 0" << std::endl;
					reflex.clear();
					while (vindices.size() >= 3)
					{
						if (reflex.empty() == 1)
						{ // O(n) check for reflex vertices, if there are none, then we can be sure that all the triangles are valid,
							// and we can just triangulate the rest of the polygon without checks=> improvement compared to the last version,
							cs.triangles.emplace_back(std::array<uint32_t, 3>{
									*vindices.begin(),
									*step,
									*(std::next(step, 1))});
							uint64_t edge = MakeEdge(*vindices.begin(), *step);
							edgeIndexSet.insert(edge);
							edge = MakeEdge(*vindices.begin(), *(std::next(step, 1)));
							edgeIndexSet.insert(edge);
							edge = MakeEdge(*step, *(std::next(step, 1)));
							edgeIndexSet.insert(edge);

							step = vindices.erase(step);

							/*std::cout<< "face valid" << std::endl;
							std::cout << "I have removed: " << v1+1 << std::endl;
							std::cout << "Triangle: "
							 << cs.triangles[cs.triangles.size()-1][0] << " " 
							 << cs.triangles[cs.triangles.size()-1][1] <<
							cs.triangles[cs.triangles.size()-1][2]<< std::endl;*/
						}
						else
						{
							if (reflex.find(*firstelement) != reflex.end())
							{ // we can't cast a triangle with a reflex vertex
								firstelement++;
								nextnext++;
								prev1++;
								prev2++;
								nextnext = nextnext == vindices.end() ? vindices.begin()
																											: nextnext;
								prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
								prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
								firstelement = firstelement == vindices.end()
																	 ? vindices.begin()
																	 : firstelement;
							}
							else
							{
								v0 = cs.face[*prev1];                               // first vertex of the triangle
								reflexelement = reflex.find(*prev1) == reflex.end() // calculate reflex vertices in the triangle
																		? 0
																		: (1 && (switcher = 1));
								v1 = cs.face[*nextnext]; // the last vertex of the triangle
								reflexelement = reflex.find(*nextnext) == reflex.end()
																		? (reflexelement && (switcher = 1))
																: reflexelement == 0
																		? (reflexelement && (switcher = 2))
																		: reflexelement += 1;
								v2 = cs.face[std::next(nextnext) == vindices.end() // getting the next vertex after the triangles 3 vertices for future normal calculations
																 ? *vindices.begin()
																 : *nextnext];

								// I've  broken down the triangle cast into 3 parts:
								// 1.) there are as much reflex vertices as there are relfex vertices in the potential triangle
								//=> we justhave to reevaluate the reflex vertices in the triangle
								// 2.) there are more reflex vertices than there are reflex vertices in the potential triangle
								//=> we have to both reeavaluate the reflex vertices in the triangle, and check the rest of the reflex vertices, whether they are in the triangle or not
								// 3.) there are 0 relfex vertices in the potential triangle
								//=> we just have to check for reflex vertices in the triangle
								if (reflexelement == reflex.size())
								{
									cs.triangles.emplace_back(
											std::array<uint32_t, 3>{
													v0, cs.face[*firstelement], v1});
									uint64_t edge = MakeEdge(v0, cs.face[*firstelement]);
									edgeIndexSet.insert(edge);
									edge = MakeEdge(v0, v1);
									edgeIndexSet.insert(edge);
									edge = MakeEdge(cs.face[*firstelement], v1);
									edgeIndexSet.insert(edge);

									firstelement = vindices.erase(firstelement);
									nextnext++;
									switch (switcher)
									{
									case 1:
										rv1 = {cs.vpc[v0 * 3 + 1] - cs.vpc[cs.face[*prev2] * 3 + 1],
													 cs.vpc[v0 * 3 + 2] -
															 cs.vpc[cs.face[*prev2] * 3 + 2]};
										rv2 = {cs.vpc[v1 * 3 + 1] - cs.vpc[cs.face[*prev2] * 3 + 1],
													 cs.vpc[v1 * 3 + 2] -
															 cs.vpc[cs.face[*prev2] * 3 + 2]};
										if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
										{
											reflex.erase(*prev1);
										}
										break;
									case 2:
										rv1 = {cs.vpc[v1 * 3 + 1] - cs.vpc[v0 * 3 + 2],
													 cs.vpc[v1 * 3 + 2] - cs.vpc[v0 * 3 + 2]};
										rv2 = {cs.vpc[v2 * 3 + 1] - cs.vpc[v0 * 3 + 1],
													 cs.vpc[v2 * 3 + 2] - cs.vpc[v0 * 3 + 2]};
										if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
										{
											reflex.erase(*firstelement);

											break;
										}
									}
								}
								else if (reflexelement > 0)
								{
									valid = false;
									for (size_t j = 0; j < reflex.size(); j++)
									{
										std::array<float, 2> p = {
												cs.vpc[cs.face[*(std::next(reflex.begin(), j))] * 3 + 1],
												cs.vpc[cs.face[*(std::next(reflex.begin(), j))] * 3 + 2]}; // the vertex of the polygon
										std::array<float, 2> tp1 = {cs.vpc[v0 * 3 + 1], cs.vpc[v0 * 3 + 2]};
										std::array<float, 2> tp2 = {cs.vpc[v1 * 3 + 1], cs.vpc[v1 * 3 + 2]};
										std::array<float, 2> tp3 = {cs.vpc[v2 * 3 + 1], cs.vpc[v2 * 3 + 2]};
										struct CPpair
										{
											float ad = 0;
											float bc = 0;
											float product = ad - bc;
										} ABP, ACP, BCP;
										ABP.ad = (tp2[0] - tp1[0]) * (p[1] - tp1[1]);
										ABP.bc = (tp2[1] - tp1[1]) * (p[0] - tp1[0]);
										ACP.ad = (tp1[0] - tp3[0]) * (p[1] - tp3[1]);
										ACP.bc = (tp1[1] - tp3[1]) * (p[0] - tp3[0]);
										BCP.ad = (tp3[0] - tp2[0]) * (p[1] - tp2[1]);
										BCP.bc = (tp3[1] - tp2[1]) * (p[0] - tp2[0]);
										ABP.product = ABP.ad - ABP.bc;
										ACP.product = ACP.ad - ACP.bc;
										BCP.product =
												BCP.ad -
												BCP.bc; // we are calculating the normals of the
																// triangle's sides with an another polygon
																// vertex//(obv. we are taking each side and
																// the current vertex to it),
										// we are taking the normal relative to a vertex of the
										// potential triangle , and rotate through the triangle
										// in ccw
										valid = (ABP.product > limit) &&
														(ACP.product > limit) && (BCP.product > limit);
										if (valid == true)
										{ // we exclude go step 1 forward in
											// potential triangle vertices,
											// because the last 1 was invalid
											//std::cout<< "face invalid" << std::endl;
											firstelement++;
											prev1++;
											prev2++;
											prev2 = prev2 == vindices.end() ? vindices.begin()
																											: prev2;
											prev1 = prev1 == vindices.end() ? vindices.begin()
																											: prev1;
											firstelement = firstelement == vindices.end()
																				 ? vindices.begin()
																				 : firstelement;
											break;
										}
									}
									if (valid == false)
									{ // in the case of the triangle being valid
										cs.triangles.emplace_back(
												std::array<uint32_t, 3>{
														v0, cs.face[*firstelement], v1});
										uint64_t edge = MakeEdge(v0, cs.face[*firstelement]);
										edgeIndexSet.insert(edge);
										edge = MakeEdge(v0, v1);
										edgeIndexSet.insert(edge);
										edge = MakeEdge(cs.face[*firstelement], v1);
										edgeIndexSet.insert(edge);
										firstelement = vindices.erase(firstelement);

										switch (switcher)
										{
										case 1:
											rv1 = {cs.vpc[v0 * 3 + 1] - cs.vpc[cs.face[*prev2] * 3 + 1],
														 cs.vpc[v0 * 3 + 2] -
																 cs.vpc[cs.face[*prev2] * 3 + 2]};
											rv2 = {cs.vpc[v1 * 3 + 1] - cs.vpc[cs.face[*prev2] * 3 + 1],
														 cs.vpc[v1 * 3 + 2] -
																 cs.vpc[cs.face[*prev2] * 3 + 2]};
											if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
											{
												reflex.erase(*prev1);
											}
											break;
										case 2:
											rv1 = {cs.vpc[v1 * 3 + 1] - cs.vpc[v0 * 3 + 2],
														 cs.vpc[v1 * 3 + 2] - cs.vpc[v0 * 3 + 2]};
											rv2 = {cs.vpc[v2 * 3 + 1] - cs.vpc[v0 * 3 + 1],
														 cs.vpc[v2 * 3 + 2] - cs.vpc[v0 * 3 + 2]};
											if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
											{
												reflex.erase(*firstelement);

												break;
											}
										}
									}
								}
								else
								{
									valid = false;
									for (size_t j = 0; j < reflex.size(); j++)
									{
										std::array<float, 2> p = {
												cs.vpc[cs.face[*(std::next(reflex.begin(), j))] * 3 + 1],
												cs.vpc[cs.face[*(std::next(reflex.begin(), j))] * 3 + 2]}; // the vertex of the polygon
										std::array<float, 2> tp1 = {cs.vpc[v0 * 3 + 1], cs.vpc[v0 * 3 + 2]};
										std::array<float, 2> tp2 = {cs.vpc[v1 * 3 + 1], cs.vpc[v1 * 3 + 2]};
										std::array<float, 2> tp3 = {cs.vpc[v2 * 3 + 1], cs.vpc[v2 * 3 + 2]};
										struct CPpair
										{
											float ad = 0;
											float bc = 0;
											float product = ad - bc;
										} ABP, ACP, BCP;
										ABP.ad = (tp2[0] - tp1[0]) * (p[1] - tp1[1]);
										ABP.bc = (tp2[1] - tp1[1]) * (p[0] - tp1[0]);
										ACP.ad = (tp1[0] - tp3[0]) * (p[1] - tp3[1]);
										ACP.bc = (tp1[1] - tp3[1]) * (p[0] - tp3[0]);
										BCP.ad = (tp3[0] - tp2[0]) * (p[1] - tp2[1]);
										BCP.bc = (tp3[1] - tp2[1]) * (p[0] - tp2[0]);
										ABP.product = ABP.ad - ABP.bc;
										ACP.product = ACP.ad - ACP.bc;
										BCP.product =
												BCP.ad -
												BCP.bc; // we are calculating the normals of the
																// triangle's sides with an another polygon
																// vertex(obv. we are taking each side and
																// the current vertex to it),
										// we are taking the normal relative to a vertex of the
										// potential triangle , and rotate through the triangle
										// in ccw
										valid = (ABP.product > limit) &&
														(ACP.product > limit) && (BCP.product > limit);
										if (valid == true)
										{ // we exclude go step 1 forward in
											// potential triangle vertices,
											// because the last 1 was invalid
											// std::cout<< "face invalid" << std::endl;
											firstelement++;
											prev1++;
											prev2++;
											prev2 = prev2 == vindices.end() ? vindices.begin()
																											: prev2;
											prev1 = prev1 == vindices.end() ? vindices.begin()
																											: prev1;
											firstelement = firstelement == vindices.end()
																				 ? vindices.begin()
																				 : firstelement;
											break;
										}
									}
									if (valid == false)
									{
										cs.triangles.emplace_back(
												std::array<uint32_t, 3>{
														v0, cs.face[*firstelement], v1});
										uint64_t edge = MakeEdge(v0, cs.face[*firstelement]);
										edgeIndexSet.insert(edge);
										edge = MakeEdge(v0, v1);
										edgeIndexSet.insert(edge);
										edge = MakeEdge(cs.face[*firstelement], v1);
										edgeIndexSet.insert(edge);
										firstelement = vindices.erase(firstelement);
										firstelement++;
										prev1++;
										prev2++;
										prev2 =
												prev2 == vindices.end() ? vindices.begin() : prev2;
										prev1 =
												prev1 == vindices.end() ? vindices.begin() : prev1;
										firstelement = firstelement == vindices.end()
																			 ? vindices.begin()
																			 : firstelement;
									}
									continue;
								}
							}
						}
					}
					break;
				}
			}

			case 1:
			{
				std::list<uint32_t>::iterator back;
					// we are ordering here based on winding. The normal
					// tells us, which way are the vertices ordered
					for (size_t j{}; j < cs.face.size(); j++)
					{
						if (j < 2)
						{
							vindices.push_back(j);
						}
						else if (j >= 2 && j < cs.face.size() - 1)
						{
							vindices.push_back(j);
							v2 = cs.face[j - 2];
							v1 = cs.face[j - 1];
							v0 = cs.face[j];
							rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
							{
								reflex.insert(j - 1);
							}
						}
						else
						{
							vindices.push_back(j);
							v2 = cs.face[j - 2];
							v1 = cs.face[j - 1];
							v0 = cs.face[j];
							rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
							{
								reflex.insert(j - 1);
							} // until here, the cycle is same as above
							v2 = v1;
							v1 = v0;
							v0 = cs.face[0];
							rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
							{
								reflex.insert(j);
							}
							v2 = v1;
							v1 = v0;
							v0 = cs.face[1];
							rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 2] - cs.vpc[v2 * 3 + 2]};
							if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
							{
								reflex.insert(0);
							}
						}
					}
				

				std::list<uint32_t>::iterator step = {
						std::next(vindices.begin(), 1)};
				uint32_t reflexelement{0};
				uint32_t switcher;
				bool valid;
				std::list<uint32_t>::iterator firstelement{vindices.begin()};
				std::list<uint32_t>::iterator nextnext{
						std::next(vindices.begin(), 1)};
				std::list<uint32_t>::iterator prev1{
						std::prev(vindices.end(), 1)};
				std::list<uint32_t>::iterator prev2{
						std::prev(vindices.end(), 2)};
				reflex.clear();
				//std::cout << "I made it to while loop in case 1" << std::endl;
				while (vindices.size() >= 3)
				{
					//std::cout << " entered while loop" << std::endl;
					if (reflex.empty() == 1)
					{
						//std::cout << "reflex is empty" << std::endl;
						cs.triangles.emplace_back(
								std::array<uint32_t, 3>{
										cs.face[*vindices.begin()],
										cs.face[*step],
										cs.face[*(std::next(step, 1))]});
						uint64_t edge = MakeEdge(cs.face[*vindices.begin()], cs.face[*step]);
						edgeIndexSet.insert(edge);
						edge = MakeEdge(cs.face[*vindices.begin()], cs.face[*(std::next(step, 1))]);
						edgeIndexSet.insert(edge);
						edge = MakeEdge(cs.face[*step], cs.face[*(std::next(step, 1))]);
						edgeIndexSet.insert(edge);
						/*std::cout << "Triangle: " << cs.face[*vindices.begin()] - 1 << " "
											<< cs.face[*step] - 1 << " "
											<< cs.face[*(std::next(step, 1))] - 1 << std::endl;*/
						step = vindices.erase(step);

						/*std::cout<< "face valid" << std::endl;
						std::cout << "I have removed: " << v1+1 <<
						std::endl;
						std::cout << "Triangle"<<cs.triangles[cs.triangles.size()-1][0] << " " <<
						cs.triangles[cs.triangles.size()-1][1] << " "<<
						cs.triangles[cs.triangles.size()-1][2]<< std::endl;*/
					}
					else
					{
						//std::cout << "else" << std::endl;
						if (reflex.find(*firstelement) != reflex.end())
						{
							firstelement++;
							nextnext++;
							prev1++;
							prev2++;
							nextnext = nextnext == vindices.end() ? vindices.begin()
																										: nextnext;
							prev2 =
									prev2 == vindices.end() ? vindices.begin() : prev2;
							prev1 =
									prev1 == vindices.end() ? vindices.begin() : prev1;
							firstelement = firstelement == vindices.end()
																 ? vindices.begin()
																 : firstelement;
						}
						else
						{
							v0 = cs.face[*prev1];
							reflexelement = reflex.find(*prev1) == reflex.end()
																	? 0
																	: (1 && (switcher = 1));
							v1 = cs.face[*nextnext];
							reflexelement = reflex.find(*nextnext) == reflex.end()
																	? (reflexelement && (switcher = 1))
															: reflexelement == 0
																	? (reflexelement && (switcher = 2))
																	: reflexelement += 1;
							v2 = cs.face[std::next(nextnext) == vindices.end()
															 ? *vindices.begin()
															 : *nextnext];

							if (reflexelement == reflex.size())
							{
								cs.triangles.emplace_back(
										std::array<uint32_t, 3>{
												v0, cs.face[*firstelement], v1});
								uint64_t edge = MakeEdge(v0, cs.face[*firstelement]);
								edgeIndexSet.insert(edge);
								edge = MakeEdge(v0, v1);
								edgeIndexSet.insert(edge);
								edge = MakeEdge(cs.face[*firstelement], v1);
								edgeIndexSet.insert(edge);
								firstelement = vindices.erase(firstelement);
								std::cout << "reflex.size() is reflexelement" << std::endl;
								std::cout << "Triangle: " << v0 << " "
											<< cs.face[*firstelement] - 1 << " "
											<< v1 << std::endl;
								nextnext++;
								switch (switcher)
								{
								case 1:
									rv1 = {cs.vpc[v0 * 3] -
														 cs.vpc[cs.face[*prev2] * 3],
												 cs.vpc[v0 * 3 + 2] -
														 cs.vpc[cs.face[*prev2] * 3 + 2]};
									rv2 = {cs.vpc[v1 * 3] -
														 cs.vpc[cs.face[*prev2] * 3],
												 cs.vpc[v1 * 3 + 2] -
														 cs.vpc[cs.face[*prev2] * 3 + 2]};
									if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
									{
										reflex.erase(*prev1);
									}
									break;
								case 2:
									rv1 = {cs.vpc[v1 * 3] - cs.vpc[v0 * 3],
												 cs.vpc[v1 * 3 + 2] - cs.vpc[v0 * 3 + 2]};
									rv2 = {cs.vpc[v2 * 3] - cs.vpc[v0 * 3],
												 cs.vpc[v2 * 3 + 2] - cs.vpc[v0 * 3 + 2]};
									if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
									{
										reflex.erase(*firstelement);

										break;
									}
								}
							}
							else if (reflexelement > 0)
							{
								std::cout << "reflex.size() is > reflexelement" << std::endl;
								valid = false;
								for (size_t j = 0; j < reflex.size(); j++)
								{
									std::array<float, 2> p = {
											cs.vpc[cs.face[*(std::next(reflex.begin(), j))] * 3],
											cs.vpc[cs.face[*(std::next(reflex.begin(), j))] * 3 + 2]}; // the vertex of the polygon
									std::array<float, 2> tp1 = {cs.vpc[v0 * 3], cs.vpc[v0 * 3 + 2]};
									std::array<float, 2> tp2 = {cs.vpc[v1 * 3], cs.vpc[v1 * 3 + 2]};
									std::array<float, 2> tp3 = {cs.vpc[v2 * 3], cs.vpc[v2 * 3 + 2]};
									struct CPpair
									{
										float ad = 0;
										float bc = 0;
										float product = ad - bc;
									} ABP, ACP, BCP;
									ABP.ad = (tp2[0] - tp1[0]) * (p[1] - tp1[1]);
									ABP.bc = (tp2[1] - tp1[1]) * (p[0] - tp1[0]);
									ACP.ad = (tp1[0] - tp3[0]) * (p[1] - tp3[1]);
									ACP.bc = (tp1[1] - tp3[1]) * (p[0] - tp3[0]);
									BCP.ad = (tp3[0] - tp2[0]) * (p[1] - tp2[1]);
									BCP.bc = (tp3[1] - tp2[1]) * (p[0] - tp2[0]);
									ABP.product = ABP.ad - ABP.bc;
									ACP.product = ACP.ad - ACP.bc;
									BCP.product =
											BCP.ad -
											BCP.bc; // we are calculating the normals of the
															// triangle's sides with an another
															// polygon vertex(obv. we are taking
															// each side and the current vertex to
															// it),
									// we are taking the normal relative to a vertex of
									// the potential triangle , and rotate through the
									// triangle in ccw
									valid = (ABP.product > limit) &&
													(ACP.product > limit) &&
													(BCP.product > limit);
									if (valid ==
											true)
									{ // we exclude go step 1 forward in
										// potential triangle vertices, because
										// the last 1 was invalid
										//std::cout<< "face invalid" << std::endl;
										firstelement++;
										prev1++;
										prev2++;
										prev2 = prev2 == vindices.end() ? vindices.begin()
																										: prev2;
										prev1 = prev1 == vindices.end() ? vindices.begin()
																										: prev1;
										firstelement = firstelement == vindices.end()
																			 ? vindices.begin()
																			 : firstelement;
										break;
									}
								}
								if (valid == false)
								{
									cs.triangles.emplace_back(
											std::array<uint32_t, 3>{
													v0, cs.face[*firstelement],
													v1});
									uint64_t edge = MakeEdge(v0, cs.face[*firstelement]);
									edgeIndexSet.insert(edge);
									edge = MakeEdge(v0, v1);
									edgeIndexSet.insert(edge);
									edge = MakeEdge(cs.face[*firstelement], v1);
									edgeIndexSet.insert(edge);
									firstelement = vindices.erase(firstelement);

									/*std::cout << "Triangle: " << v0 << " "
											<< cs.face[*firstelement] - 1 << " "
											<< v1 << std::endl;*/
									switch (switcher)
									{
									case 1:
										rv1 = {cs.vpc[v0 * 3] -
															 cs.vpc[cs.face[*prev2] * 3],
													 cs.vpc[v0 * 3 + 2] -
															 cs.vpc[cs.face[*prev2] * 3 + 2]};
										rv2 = {cs.vpc[v1 * 3] -
															 cs.vpc[cs.face[*prev2] * 3],
													 cs.vpc[v1 * 3 + 2] -
															 cs.vpc[cs.face[*prev2] * 3 + 2]};
										if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
										{
											reflex.erase(*prev1);
										}
										break;
									case 2:
										rv1 = {cs.vpc[v1 * 3] - cs.vpc[v0 * 3],
													 cs.vpc[v1 * 3 + 2] - cs.vpc[v0 * 3 + 2]};
										rv2 = {cs.vpc[v2 * 3] - cs.vpc[v0 * 3],
													 cs.vpc[v2 * 3 + 2] - cs.vpc[v0 * 3 + 2]};
										if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
										{
											reflex.erase(*firstelement);

											break;
										}
									}
								}
							}
							else
							{
								valid = false;
								std::cout << "reflexelement is 0" << std::endl;
								for (size_t j = 0; j < reflex.size(); j++)
								{
									std::array<float, 2> p = {
											cs.vpc[cs.face[*(std::next(reflex.begin(), j))] * 3],
											cs.vpc[cs.face[*(std::next(reflex.begin(), j))] * 3 + 2]}; // the vertex of the polygon
									std::array<float, 2> tp1 = {cs.vpc[v0 * 3], cs.vpc[v0 * 3 + 2]};
									std::array<float, 2> tp2 = {cs.vpc[v1 * 3], cs.vpc[v1 * 3 + 2]};
									std::array<float, 2> tp3 = {cs.vpc[v2 * 3], cs.vpc[v2 * 3 + 2]};
									struct CPpair
									{
										float ad = 0;
										float bc = 0;
										float product = ad - bc;
									} ABP, ACP, BCP;
									ABP.ad = (tp2[0] - tp1[0]) * (p[1] - tp1[1]);
									ABP.bc = (tp2[1] - tp1[1]) * (p[0] - tp1[0]);
									ACP.ad = (tp1[0] - tp3[0]) * (p[1] - tp3[1]);
									ACP.bc = (tp1[1] - tp3[1]) * (p[0] - tp3[0]);
									BCP.ad = (tp3[0] - tp2[0]) * (p[1] - tp2[1]);
									BCP.bc = (tp3[1] - tp2[1]) * (p[0] - tp2[0]);
									ABP.product = ABP.ad - ABP.bc;
									ACP.product = ACP.ad - ACP.bc;
									BCP.product =
											BCP.ad -
											BCP.bc; // we are calculating the normals of the
															// triangle's sides with an another
															// polygon vertex(obv. we are taking
															// each side and the current vertex to
															// it),
									// we are taking the normal relative to a vertex of
									// the potential triangle , and rotate through the
									// triangle in ccw
									valid = (ABP.product > limit) &&
													(ACP.product > limit) &&
													(BCP.product > limit);
									if (valid ==
											true)
									{ // we exclude go step 1 forward in
										// potential triangle vertices, because
										// the last 1 was invalid
										std::cout<< "face invalid" << std::endl;
										firstelement++;
										prev1++;
										prev2++;
										prev2 = prev2 == vindices.end() ? vindices.begin()
																										: prev2;
										prev1 = prev1 == vindices.end() ? vindices.begin()
																										: prev1;
										firstelement = firstelement == vindices.end()
																			 ? vindices.begin()
																			 : firstelement;
										break;
									}
								}
								if (valid == false)
								{
									cs.triangles.emplace_back(
											std::array<uint32_t, 3>{
													v0, cs.face[*firstelement],
													v1});
									uint64_t edge = MakeEdge(v0, cs.face[*firstelement]);
									edgeIndexSet.insert(edge);
									edge = MakeEdge(v0, v1);
									edgeIndexSet.insert(edge);
									edge = MakeEdge(cs.face[*firstelement], v1);
									edgeIndexSet.insert(edge);
									std::cout << "Triangle: " << v0 << " "<< cs.face[*firstelement] - 1 << " "<< v1 << std::endl;
									firstelement = vindices.erase(firstelement);
									firstelement++;
									prev1++;
									prev2++;
									prev2 = prev2 == vindices.end() ? vindices.begin()
																									: prev2;
									prev1 = prev1 == vindices.end() ? vindices.begin()
																									: prev1;
									firstelement = firstelement == vindices.end()
																		 ? vindices.begin()
																		 : firstelement;
								}
								continue;
							}
						}
					}
				}

				break;
			}
			case 2:
			{
				std::list<uint32_t>::iterator back;
					// we are ordering here based on winding. The normal
					// tells us, which way are the vertices ordered
					for (size_t j = 0; j < cs.face.size(); j++)
					{
						if (j < 2)
						{
							vindices.push_back(j);
						}
						else if (j >= 2 && j < cs.face.size() - 1)
						{
							vindices.push_back(j);
							v2 = cs.face[j - 2];
							v1 = cs.face[j - 1];
							v0 = cs.face[j];
							rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
							rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
							if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
							{
								reflex.insert(j - 1);
							}
						}
						else
						{
							vindices.push_back(j);
							v2 = cs.face[j - 2];
							v1 = cs.face[j - 1];
							v0 = cs.face[j];
							rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
							rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
							if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
							{
								reflex.insert(j - 1);
							} // until here, the cycle is same as above
							v2 = v1;
							v1 = v0;
							v0 = cs.face[0];
							rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
							rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
							if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
							{
								reflex.insert(j);
							}
							v2 = v1;
							v1 = v0;
							v0 = cs.face[1];
							rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
							rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
							if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
							{
								reflex.insert(0);
							}
						}
					}
				
				std::list<uint32_t>::iterator step = {
						std::next(vindices.begin(), 1)};
				uint32_t reflexelement{0};
				uint32_t switcher;
				bool valid;
				std::list<uint32_t>::iterator firstelement{vindices.begin()};
				std::list<uint32_t>::iterator nextnext{
						std::next(vindices.begin(), 1)};
				std::list<uint32_t>::iterator prev1{
						std::prev(vindices.end(), 1)};
				std::list<uint32_t>::iterator prev2{
						std::prev(vindices.end(), 2)};
				reflex.clear();
				//std::cout << "I made it to while loop in case 2" << std::endl;
				while (vindices.size() >= 3)
				{
					if (reflex.empty() == 1)
					{
						cs.triangles.emplace_back(std::array<uint32_t, 3>{
								cs.face[*vindices.begin()],
								cs.face[*step],
								cs.face[*(std::next(step, 1))]});
						uint64_t edge = MakeEdge(cs.face[*vindices.begin()], cs.face[*step]);
						edgeIndexSet.insert(edge);
						edge = MakeEdge(cs.face[*vindices.begin()], cs.face[*(std::next(step, 1))]);
						edgeIndexSet.insert(edge);
						edge = MakeEdge(cs.face[*step], cs.face[*(std::next(step, 1))]);
						edgeIndexSet.insert(edge);
						step = vindices.erase(step);
						//std::cout<< "face valid" << std::endl;
						//std::cout << "I have removed: " << v1+1 << std::endl;
						//std::cout << "Triangle"<<cs.triangles[cs.triangles.size()-1][0] << " " <<cs.triangles[cs.triangles.size()-1][1] << " "<<cs.triangles[cs.triangles.size()-1][2]<< std::endl;
					}
					else
					{
						if (reflex.find(*firstelement) != reflex.end())
						{
							firstelement++;
							nextnext++;
							prev1++;
							prev2++;
							nextnext = nextnext == vindices.end() ? vindices.begin()
																										: nextnext;
							prev2 =
									prev2 == vindices.end() ? vindices.begin() : prev2;
							prev1 =
									prev1 == vindices.end() ? vindices.begin() : prev1;
							firstelement = firstelement == vindices.end()
																 ? vindices.begin()
																 : firstelement;
						}
						else
						{
							v0 = cs.face[*prev1];
							reflexelement = reflex.find(*prev1) == reflex.end()
																	? 0
																	: (1 && (switcher = 1));
							v1 = cs.face[*nextnext];
							reflexelement = reflex.find(*nextnext) == reflex.end()
																	? (reflexelement && (switcher = 1))
															: reflexelement == 0
																	? (reflexelement && (switcher = 2))
																	: reflexelement += 1;
							v2 = cs.face[std::next(nextnext) == vindices.end()
															 ? *vindices.begin()
															 : *nextnext];

							if (reflexelement == reflex.size())
							{
								cs.triangles.emplace_back(
										std::array<uint32_t, 3>{
												v0, cs.face[*firstelement], v1});
								uint64_t edge = MakeEdge(v0, cs.face[*firstelement]);
								edgeIndexSet.insert(edge);
								edge = MakeEdge(v0, v1);
								edgeIndexSet.insert(edge);
								edge = MakeEdge(cs.face[*firstelement], v1);
								edgeIndexSet.insert(edge);
								firstelement = vindices.erase(firstelement);
								nextnext++;
								switch (switcher)
								{
								case 1:
									rv1 = {
											cs.vpc[v0 * 3] - cs.vpc[cs.face[*prev2] * 3],
											cs.vpc[v0 * 3 + 1] - cs.vpc[cs.face[*prev2] * 3 + 1]};
									rv2 = {
											cs.vpc[v1 * 3] - cs.vpc[cs.face[*prev2] * 3],
											cs.vpc[v1 * 3 + 1] - cs.vpc[cs.face[*prev2] * 3 + 1]};
									if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
									{
										reflex.erase(*prev1);
									}
									break;
								case 2:
									rv1 = {cs.vpc[v1 * 3] - cs.vpc[v0 * 3],
												 cs.vpc[v1 * 3 + 1] - cs.vpc[v0 * 3 + 1]};
									rv2 = {cs.vpc[v2 * 3] - cs.vpc[v0 * 3],
												 cs.vpc[v2 * 3 + 1] - cs.vpc[v0 * 3 + 1]};
									if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
									{
										reflex.erase(*firstelement);

										break;
									}
								}
							}
							else if (reflexelement > 0)
							{
								valid = false;
								for (size_t j = 0; j < reflex.size(); j++)
								{
									std::array<float, 2> p = {
											cs.vpc[cs.face[*(std::next(reflex.begin(), j))] * 3],
											cs.vpc[cs.face[*(std::next(reflex.begin(), j))] * 3 + 1]}; // the vertex of the polygon
									std::array<float, 2> tp1 = {cs.vpc[v0 * 3], cs.vpc[v0 * 3 + 1]};
									std::array<float, 2> tp2 = {cs.vpc[v1 * 3], cs.vpc[v1 * 3 + 1]};
									std::array<float, 2> tp3 = {cs.vpc[v2 * 3], cs.vpc[v2 * 3 + 1]};
									struct CPpair
									{
										float ad = 0;
										float bc = 0;
										float product = ad - bc;
									} ABP, ACP, BCP;
									ABP.ad = (tp2[0] - tp1[0]) * (p[1] - tp1[1]);
									ABP.bc = (tp2[1] - tp1[1]) * (p[0] - tp1[0]);
									ACP.ad = (tp1[0] - tp3[0]) * (p[1] - tp3[1]);
									ACP.bc = (tp1[1] - tp3[1]) * (p[0] - tp3[0]);
									BCP.ad = (tp3[0] - tp2[0]) * (p[1] - tp2[1]);
									BCP.bc = (tp3[1] - tp2[1]) * (p[0] - tp2[0]);
									ABP.product = ABP.ad - ABP.bc;
									ACP.product = ACP.ad - ACP.bc;
									BCP.product =
											BCP.ad -
											BCP.bc; // we are calculating the normals of the
															// triangle's sides with an another
															// polygon vertex(obv. we are taking each
															// side and the current vertex to it),
									// we are taking the normal relative to a vertex of
									// the potential triangle , and rotate through the
									// triangle in ccw
									valid = (ABP.product > limit) &&
													(ACP.product > limit) &&
													(BCP.product > limit);
									if (valid ==
											true)
									{ // we exclude go step 1 forward in
										// potential triangle vertices, because
										// the last 1 was invalid
										//std::cout<< "face invalid" << std::endl;
										firstelement++;
										prev1++;
										prev2++;
										prev2 = prev2 == vindices.end() ? vindices.begin()
																										: prev2;
										prev1 = prev1 == vindices.end() ? vindices.begin()
																										: prev1;
										firstelement = firstelement == vindices.end()
																			 ? vindices.begin()
																			 : firstelement;
										break;
									}
								}
								if (valid == false)
								{
									cs.triangles.emplace_back(
											std::array<uint32_t, 3>{
													v0, cs.face[*firstelement], v1});
									uint64_t edge = MakeEdge(v0, cs.face[*firstelement]);
									edgeIndexSet.insert(edge);
									edge = MakeEdge(v0, v1);
									edgeIndexSet.insert(edge);
									edge = MakeEdge(cs.face[*firstelement], v1);
									edgeIndexSet.insert(edge);
									firstelement = vindices.erase(firstelement);

									switch (switcher)
									{
									case 1:
										rv1 = {
												cs.vpc[v0 * 3] - cs.vpc[cs.face[*prev2] * 3],
												cs.vpc[v0 * 3 + 1] - cs.vpc[cs.face[*prev2] * 3 + 1]};
										rv2 = {
												cs.vpc[v1 * 3] - cs.vpc[cs.face[*prev2] * 3],
												cs.vpc[v1 * 3 + 1] - cs.vpc[cs.face[*prev2] * 3 + 1]};
										if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
										{
											reflex.erase(*prev1);
										}
										break;
									case 2:
										rv1 = {cs.vpc[v1 * 3] - cs.vpc[v0 * 3],
													 cs.vpc[v1 * 3 + 1] - cs.vpc[v0 * 3 + 1]};
										rv2 = {cs.vpc[v2 * 3] - cs.vpc[v0 * 3],
													 cs.vpc[v2 * 3 + 1] - cs.vpc[v0 * 3 + 1]};
										if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
										{
											reflex.erase(*firstelement);

											break;
										}
									}
								}
							}
							else
							{
								valid = false;
								for (size_t j = 0; j < reflex.size(); j++)
								{
									std::array<float, 2> p = {
											cs.vpc[cs.face[*(std::next(reflex.begin(), j))] * 3],
											cs.vpc[cs.face[*(std::next(reflex.begin(), j))] * 3 + 1]}; // the vertex of the polygon
									std::array<float, 2> tp1 = {cs.vpc[v0 * 3], cs.vpc[v0 * 3 + 1]};
									std::array<float, 2> tp2 = {cs.vpc[v1 * 3], cs.vpc[v1 * 3 + 1]};
									std::array<float, 2> tp3 = {cs.vpc[v2 * 3], cs.vpc[v2 * 3 + 1]};
									struct CPpair
									{
										float ad = 0;
										float bc = 0;
										float product = ad - bc;
									} ABP, ACP, BCP;
									ABP.ad = (tp2[0] - tp1[0]) * (p[1] - tp1[1]);
									ABP.bc = (tp2[1] - tp1[1]) * (p[0] - tp1[0]);
									ACP.ad = (tp1[0] - tp3[0]) * (p[1] - tp3[1]);
									ACP.bc = (tp1[1] - tp3[1]) * (p[0] - tp3[0]);
									BCP.ad = (tp3[0] - tp2[0]) * (p[1] - tp2[1]);
									BCP.bc = (tp3[1] - tp2[1]) * (p[0] - tp2[0]);
									ABP.product = ABP.ad - ABP.bc;
									ACP.product = ACP.ad - ACP.bc;
									BCP.product =
											BCP.ad -
											BCP.bc; // we are calculating the normals of the
															// triangle's sides with an another
															// polygon vertex(obv. we are taking each
															// side and the current vertex to it),
									// we are taking the normal relative to a vertex of
									// the potential triangle , and rotate through the
									// triangle in ccw
									valid = (ABP.product > limit) &&
													(ACP.product > limit) &&
													(BCP.product > limit);
									if (valid ==
											true)
									{ // we exclude go step 1 forward in
										// potential triangle vertices, because
										// the last 1 was invalid
										//std::cout<< "face invalid" << std::endl;
										firstelement++;
										prev1++;
										prev2++;
										prev2 = prev2 == vindices.end() ? vindices.begin()
																										: prev2;
										prev1 = prev1 == vindices.end() ? vindices.begin()
																										: prev1;
										firstelement = firstelement == vindices.end()
																			 ? vindices.begin()
																			 : firstelement;
										break;
									}
								}
								if (valid == false)
								{
									cs.triangles.emplace_back(
											std::array<uint32_t, 3>{
													v0, cs.face[*firstelement], v1});
									uint64_t edge = MakeEdge(v0, cs.face[*firstelement]);
									edgeIndexSet.insert(edge);
									edge = MakeEdge(v0, v1);
									edgeIndexSet.insert(edge);
									edge = MakeEdge(cs.face[*firstelement], v1);
									edgeIndexSet.insert(edge);
									firstelement = vindices.erase(firstelement);
									firstelement++;
									prev1++;
									prev2++;
									prev2 = prev2 == vindices.end() ? vindices.begin()
																									: prev2;
									prev1 = prev1 == vindices.end() ? vindices.begin()
																									: prev1;
									firstelement = firstelement == vindices.end()
																		 ? vindices.begin()
																		 : firstelement;
								}
								continue;
							}
						}
					}
				}
				break;
			}
			}
			cs.edge.reserve(edgeIndexSet.size() * 2);
			for (uint64_t index : edgeIndexSet)
			{
				cs.edge.push_back(index >> 32);
				cs.edge.push_back(index & 0xFFFFFFFF);
			}
			}
			else
			{
					cs.triangles.emplace_back(std::array<uint32_t, 3>{
					cs.face[0], cs.face[1],
					cs.face[2]});
					
					cs.edge.reserve(6);
					cs.edge.push_back(cs.face[0]);
					cs.edge.push_back(cs.face[1]);
					cs.edge.push_back(cs.face[0]);
					cs.edge.push_back(cs.face[2]);
					cs.edge.push_back(cs.face[1]);
					cs.edge.push_back(cs.face[2]);
				}

				 // we pass on shapes, that are triangles
			}
			//std::cout << "edge set size " << edgeIndexSet.size() << std::endl;
	};

/*void Triangulator(Coord& cs) {
	   float limit = 1e-6; // basicaly zero
   std::array<float, 3> normal = {0.0, 0.0, 0.0};
   uint8_t projection = 0;
   if (cs.face.size() > 3)
   {
	   normal = {0.0, 0.0, 0.0};
	   float x, y, z, coordy, coordx, coordz;
	   uint f = 1;
	   while (
		   normal ==
		   std::array<float, 3>{
			   0.0f, 0.0f,
			   0.0f})
	   { // calculatin the normal of the face based on it's
		   // vertices, if the normal is 0, then the face is
		   // degenerate and we will try to calculate the normal
		   // with different vertices until we get a non
		   // degenerate normal or we run out of vertices to try
		   if (f + 1 == cs.face.size())
		   {
		   std::cout << "INVALID FACE" << std::endl;
		   break;
		   }
		   else
		   {
		   coordx = (cs.vpc[cs.face[f] * 3 + 1] -
					   cs.vpc[cs.face[0] * 3 + 1]) *
					   (cs.vpc[cs.face[1 + f] * 3 + 2] -
						   cs.vpc[cs.face[0] * 3 + 2]) -
				   (cs.vpc[cs.face[1 + f] * 3 + 1] -
					   cs.vpc[cs.face[0] * 3 + 1]) *
					   (cs.vpc[cs.face[f] * 3 + 2] -
						   cs.vpc[cs.face[0] * 3 + 2]);
		   coordy = (cs.vpc[cs.face[f] * 3 + 2] -
					   cs.vpc[cs.face[0] * 3 + 2]) *
					   (cs.vpc[cs.face[1 + f] * 3] -
						   cs.vpc[cs.face[0] * 3]) -
				   (cs.vpc[cs.face[1 + f] * 3 + 2] -
					   cs.vpc[cs.face[0] * 3 + 2]) *
					   (cs.vpc[cs.face[f] * 3] -
						   cs.vpc[cs.face[0] * 3]);
		   coordz = (cs.vpc[cs.face[f] * 3] -
					   cs.vpc[cs.face[0] * 3]) *
					   (cs.vpc[cs.face[1 + f] * 3 + 1] -
						   cs.vpc[cs.face[0] * 3 + 1]) -
				   (cs.vpc[cs.face[1 + f] * 3] -
					   cs.vpc[cs.face[0] * 3]) *
					   (cs.vpc[cs.face[f] * 3 + 1] -
						   cs.vpc[cs.face[0] * 3 + 1]);
		   x = limit > std::fabs(coordx) && std::fabs(coordx) >= 0 ? 0.0f
																   : coordx;
		   y = limit > std::fabs(coordy) && std::fabs(coordy) >= 0 ? 0.0f
																   : coordy;
		   z = limit > std::fabs(coordz) && std::fabs(coordz) >= 0 ? 0.0f
																   : coordz;
		   normal = {x, y, z};
		   f++;
		   }
	   }
	   float min =std::fabs(*std::min_element(normal.begin(), normal.end())), max = std::fabs(*std::max_element(normal.begin(), normal.end()));
	   int8_t dor = limit >= max-min && max-min >= 0 ? 0 : (max-min > 0 ? 1 : -1);
	   projection = dor == 0 ? std::distance(normal.begin(), std::max_element(normal.begin(), normal.end())) : dor > 1 ? std::distance(normal.begin(), std::max_element(normal.begin(), normal.end())) : std::distance(normal.begin(), std::max_element(normal.begin(), normal.end()));
	   std::vector<std::pair<size_t, bool>> vindices;

	   switch(projection) {
		   case 0:
			   {if(x > 0) {
				   for(size_t j {}; j < cs.face.size(); j++) {
				   vindices.emplace_back(j, true);
				   }
			   } else {
					   for(size_t j{}; j < cs.face.size(); j++) {
				   vindices.emplace_back(cs.face.size()-1-j, true);
				   }
			   }
			   uint k = 0;
			   float v0, v1, v2;
			   while(vindices.size() > 3) {
				   if(k == vindices.size())
				   {
					   k = 0;
				   }
				   v0 = cs.face[k];
				   v1 = k+1 == vindices.size() ? cs.face[0] : cs.face[k+1];
				   v2 = k+2 >= vindices.size() ? cs.face[k+2-vindices.size()] : cs.face[k+2];
				   std::array<float, 2>rv1 = {cs.vpc[v1 * 3 + 1] - cs.vpc[v0 * 3 + 1],cs.vpc[v1 * 3 + 2] - cs.vpc[v0 * 3 + 2]};
				   std::array<float, 2>rv2 = {cs.vpc[v2 * 3 + 1] - cs.vpc[v0 * 3 + 1],cs.vpc[v2 * 3 + 2] - cs.vpc[v0 * 3 + 2]};
				   bool reflexcheck = rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0 ? true : false;
				   if(reflexcheck == true)
				   {
					   k++;
					   continue;
				   } else {
					   bool valid = true;
					   for(size_t l = k+2; l < vindices.size()+1 && l != k ; l++)
					   {
						   if(l == vindices.size()) {
							   l = 0;
						   }
						   if(vindices[l].second == false) {
							   continue;
						   } else {
						   std::array<float, 2> p = {cs.vpc[(cs.face[vindices[l].first]) * 3 + 1], cs.vpc[(cs.face[vindices[l].first]) * 3 + 2]};
						   std::array<float, 2> tp1 = {cs.vpc[v0 * 3 + 1], cs.vpc[v0 * 3 + 2]};
						   std::array<float, 2> tp2 = {cs.vpc[v1 * 3 + 1], cs.vpc[v1 * 3 + 2]};
						   std::array<float, 2> tp3 = {cs.vpc[v2 * 3 + 1], cs.vpc[v2 * 3 + 2]};
						   struct CPpair {
							   float ad;
							   float bc;
							   float product()
							   {
								   return ad - bc;
							   }
						   } ABP, ACP, BCP;
							   ABP.ad = (tp2[0] - tp1[0]) * (p[1] - tp1[1]);
							   ABP.bc = (tp2[1] - tp1[1]) * (p[0] - tp1[0]);
							   ACP.ad = (tp3[0] - tp3[0]) * (p[1] - tp1[1]);
							   ACP.bc = (tp3[1] - tp3[1]) * (p[0] - tp1[0]);
							   BCP.ad = (tp3[0] - tp2[0]) * (p[1] - tp2[1]);
							   BCP.bc = (tp3[1] - tp2[1]) * (p[0] - tp2[0]);
							   bool hasNeg = (ABP.product() < 0) || (ACP.product() < 0) || (BCP.product() < 0);
							   bool hasPos = (ABP.product() > 0) || (ACP.product() > 0) || (BCP.product() > 0);
							   valid = !(hasNeg && hasPos);
						   }
					   }
				   }
			   }}
			   break;
			   case 1:
				   {if(y > 0) {
					   for(int j = 0; j < cs.face.size(); j++) {
				   vindices.emplace_back(j, true);
				   }
				   } else {
					   for(int j = 0; j < cs.face.size(); j++) {
				   vindices.emplace_back(cs.face.size()-1-j, true);
				   }
				   }
				   uint k = 0;
				   float v0, v1, v2;
				   while(vindices.size() > 3) {
					   if(k == vindices.size())
					   {
						   k = 0;
					   }
					   v0 = cs.face[k];
					   v1 = k+1 == vindices.size() ? cs.face[0] : cs.face[k+1];
					   v2 = k+2 >= vindices.size() ? cs.face[k+2-vindices.size()] : cs.face[k+2];
					   std::array<float, 2>rv1 = {cs.vpc[v1 * 3] - cs.vpc[v0 * 3],cs.vpc[v1 * 3 + 2] - cs.vpc[v0 * 3 + 2]};
					   std::array<float, 2>rv2 = {cs.vpc[v2 * 3] - cs.vpc[v0 * 3],cs.vpc[v2 * 3 + 2] - cs.vpc[v0 * 3 + 2]};
					   bool reflexcheck = rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0 ? true : false;
					   if(reflexcheck == true)
					   {
						   k++;
						   continue;
					   } else {
						   bool valid = true;
						   for(size_t l = k+2; l < vindices.size()+1 && l != k ; l++)
						   {
							   if(l == vindices.size()) {
								   l = 0;
							   }
							   if(vindices[l].second == false) {
								   continue;
							   } else {
							   std::array<float, 2> p = {cs.vpc[(cs.face[vindices[l].first]) * 3 + 1], cs.vpc[(cs.face[vindices[l].first]) * 3 + 2]};
							   std::array<float, 2> tp1 = {cs.vpc[v0 * 3 + 0], cs.vpc[v0 * 3 + 2]};
							   std::array<float, 2> tp2 = {cs.vpc[v1 * 3 + 0], cs.vpc[v1 * 3 + 2]};
							   std::array<float, 2> tp3 = {cs.vpc[v2 * 3 + 0], cs.vpc[v2 * 3 + 2]};
							   struct CPpair {
								   float ad;
								   float bc;
								   float product()
								   {
									   return ad - bc;
								   }
							   } ABP, ACP, BCP;
								   ABP.ad = (tp2[0] - tp1[0]) * (p[1] - tp1[1]);
								   ABP.bc = (tp2[1] - tp1[1]) * (p[0] - tp1[0]);
								   ACP.ad = (tp1[0] - tp3[0]) * (p[1] - tp3[1]);
								   ACP.bc = (tp1[1] - tp3[1]) * (p[0] - tp3[0]);
								   BCP.ad = (tp3[0] - tp2[0]) * (p[1] - tp2[1]);
								   BCP.bc = (tp3[1] - tp2[1]) * (p[0] - tp2[0]);
								   bool hasNeg = (ABP.product() < 0) || (ACP.product() < 0) || (BCP.product() < 0);
								   bool hasPos = (ABP.product() > 0) || (ACP.product() > 0) || (BCP.product() > 0);
								   valid = !(hasNeg && hasPos);
							   }
						   }
					   }
				   }}
				   break;
				   case 2:
					   {if(z > 0) {
						   for(int j = 0; j < cs.face.size(); j++) {
					   vindices.emplace_back(j, true);
					   }
					   } else {
						   for(int j = 0; j < cs.face.size(); j++) {
					   vindices.emplace_back(cs.face.size()-1-j, true);
					   }
					   }
					   uint k = 0;
					   float v0, v1, v2;
					   while(vindices.size() > 3) {
					   if(k == vindices.size())
					   {
						   k = 0;
					   }
					   v0 = cs.face[k];
					   v1 = k+1 == vindices.size() ? cs.face[0] : cs.face[k+1];
					   v2 = k+2 >= vindices.size() ? cs.face[k+2-vindices.size()] : cs.face[k+2];
					   std::array<float, 2>rv1 = {cs.vpc[v1 * 3] - cs.vpc[v0 * 3],cs.vpc[v1 * 3 + 1] - cs.vpc[v0 * 3 + 1]};
					   std::array<float, 2>rv2 = {cs.vpc[v2 * 3] - cs.vpc[v0 * 3],cs.vpc[v2 * 3 + 1] - cs.vpc[v0 * 3 + 1]};
					   bool reflexcheck = rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0 ? true : false;
					   if(reflexcheck == true)
					   {
						   k++;
						   continue;
					   } else {
						   bool valid = true;
						   for(size_t l = k+2; l < vindices.size()+1 && l != k ; l++)
						   {
							   if(l == vindices.size()) {
								   l = 0;
							   }
							   if(vindices[l].second == false) {
								   continue;
							   } else {
							   std::array<float, 2> p = {cs.vpc[(cs.face[vindices[l].first]) * 3 + 0], cs.vpc[(cs.face[vindices[l].first]) * 3 + 1]};
							   std::array<float, 2> tp1 = {cs.vpc[v0 * 3 + 0], cs.vpc[v0 * 3 + 1]};
							   std::array<float, 2> tp2 = {cs.vpc[v1 * 3 + 0], cs.vpc[v1 * 3 + 1]};
							   std::array<float, 2> tp3 = {cs.vpc[v2 * 3 + 0], cs.vpc[v2 * 3 + 1]};
							   struct CPpair {
								   float ad;
								   float bc;
								   float product()
								   {
									   return ad - bc;
								   }
							   } ABP, ACP, BCP;
								   ABP.ad = (tp2[0] - tp1[0]) * (p[1] - tp1[1]);
								   ABP.bc = (tp2[1] - tp1[1]) * (p[0] - tp1[0]);
								   ACP.ad = (tp3[0] - tp3[0]) * (p[1] - tp1[1]);
								   ACP.bc = (tp3[1] - tp3[1]) * (p[0] - tp1[0]);
								   BCP.ad = (tp3[0] - tp2[0]) * (p[1] - tp2[1]);
								   BCP.bc = (tp3[1] - tp2[1]) * (p[0] - tp2[0]);
								   bool hasNeg = (ABP.product() < 0) || (ACP.product() < 0) || (BCP.product() < 0);
								   bool hasPos = (ABP.product() > 0) || (ACP.product() > 0) || (BCP.product() > 0);
								   valid = !(hasNeg && hasPos);
							   }
						   }
					   }
				   }}
				   break;
			   }
	   } else {
		   cs.triangles.emplace_back(std::array<uint32_t, 3>{cs.face[0], cs.face[1], cs.face[2]});
	   }

   };*/
// depricated triangulator


/*class Transformation : public objIdent{
public:

		
		void linearTrans(int lane, float length, uint32_t coordnum) {
		enum class dimlane { X, Y, Z };
		dimlane dimension;
		if (lane == 1) {
			dimension = dimlane::X;
		} else if (lane == 2) {
			dimension = dimlane::Y;
		} else {
			dimension = dimlane::Z;
		}
		Coord &cs = objects[coordnum];
		// applies the same transition in one dimension on every single vertex
		switch (dimension) {
		case dimlane::X:
			for (size_t i = 0; i < (cs.vpc.size()/3); i++) {
				float trans = cs.vpc[i*3] + length;
				cs.tvxc.push_back(trans);
			}
			cs.vpc.swap(cs.tvxc);
			cs.tvxc.clear();
			break;

		case dimlane::Y:
			for (size_t i = 0; i < cs.vyc.size(); i++) {
				float trans = cs.vyc[i] + length;
				cs.tvyc.push_back(trans);
			}
			cs.vyc.swap(cs.tvyc);
			cs.tvyc.clear();
			break;

		case dimlane::Z:
			for (size_t i = 0; i < cs.vzc.size(); i++) {
				float trans = cs.vzc[i] + length;
				cs.tvzc.push_back(trans);
			}
			cs.vzc.swap(cs.tvzc);
			cs.tvzc.clear();
			break;
		}
	};
	// rotates the object with 2D matrices(just like freecad)
	void turnTrans(int type, int degree, uint32_t coordnum) {
		enum class axis { XY, YZ, ZX };
		axis chosen;
		if (type == 1) {
			chosen = axis::XY;
		} else if (type == 2) {
			chosen = axis::YZ;
		} else {
			chosen = axis::ZX;
		}
		float Radian = static_cast<float>(degree * M_PI / 180);
		float vertical = std::sin(Radian);
		float horizontal = std::cos(Radian);
		Coord &cs = objects[coordnum];
		switch (chosen) {
		case axis::XY:
			for (size_t i = 0; i < cs.vxc.size(); i++) {
				float transvert = cs.vyc[i] * horizontal + cs.vxc[i] * vertical;
				float transhor = cs.vyc[i] * vertical * (-1) + cs.vxc[i] * horizontal;
				cs.tvxc.push_back(transhor);
				cs.tvyc.push_back(transvert);
			}
			cs.vxc.swap(cs.tvxc);
			cs.tvxc.clear();
			cs.vyc.swap(cs.tvyc);
			cs.tvyc.clear();

			break;

		case axis::YZ:
			for (size_t i = 0; i < cs.vyc.size(); i++) {
				float transvert = cs.vzc[i] * horizontal + cs.vyc[i] * vertical;
				float transhor = cs.vzc[i] * vertical * (-1) + cs.vyc[i] * horizontal;
				cs.tvyc.push_back(transhor);
				cs.tvzc.push_back(transvert);
			}
			cs.vzc.swap(cs.tvzc);
			cs.tvzc.clear();
			cs.vyc.swap(cs.tvyc);
			cs.tvyc.clear();
			break;

		case axis::ZX:
			for (size_t i = 0; i < cs.vxc.size(); i++) {
				float transvert = cs.vxc[i] * horizontal + cs.vzc[i] * vertical;
				float transhor = cs.vxc[i] * vertical * (-1) + cs.vzc[i] * horizontal;
				cs.tvxc.push_back(transhor);
				cs.tvzc.push_back(transvert);
			}
			cs.vxc.swap(cs.tvxc);
			cs.tvxc.clear();
			cs.vzc.swap(cs.tvzc);
			cs.tvzc.clear();
			break;
		}
}
};*/

#endif 