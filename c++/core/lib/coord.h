#ifndef COORD_H
#define COORD_H 
#include <vector>
#include <array>
#include <cstdint>
#include <string>

struct Coord {
public:

    // vertices
    std::vector<float> vpc;
    std::string name;
    // vertex normals
    std::vector<float> vnxc;
    std::vector<float> vnyc;
    std::vector<float> vnzc;
    
    //usable faces(that could be intersected)
    std::vector<int> usable;
    std::vector<std::array<uint32_t, 3>> triangles;
    std::vector<uint32_t> face;
    std::vector<std::vector<uint32_t>> intersections;
    std::array<std::array<float, 3>, 6> detailes;
    std::vector<uint32_t> edge;
};

#endif