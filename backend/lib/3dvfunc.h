#ifndef THREEVFUNC_H
#define THREEVFUNC_H

#include <vector>
#include <cmath>
#include <string>
#include <cstdint>
#include <array>
#include <chrono>
#include <thread>

struct Coord {
public:
    //endpoint
    std::vector<float> xcoords;
    std::vector<float> ycoords;
    std::vector<float> zcoords;
    //startpoint
    std::vector<float> stpxcoords;
    std::vector<float> stpycoords;
    std::vector<float> stpzcoords;
    // length
    std::vector<float> xlen;
    std::vector<float> ylen;
    std::vector<float> zlen;
    // vertices
    std::vector<float> vxc;
    std::vector<float> vyc;
    std::vector<float> vzc;
    //vertex normals
    std::vector<float> vnxc;
    std::vector<float> vnyc;
    std::vector<float> vnzc;
    // transformed vertices
    std::vector<float> tvxc;
    std::vector<float> tvyc;
    std::vector<float> tvzc;
    //usable faces(that could be intersected)
    std::vector<int> usable;
    std::vector<std::array<uint64_t, 3>> triangles;

    struct Face {
        struct Vertex { uint32_t v, vt, vn; };
        std::vector<Face::Vertex> ver;
    };

    std::vector<Face> fvi;
    std::vector<std::vector<uint32_t>> intersections;
};
class Vector {
public:
    Coord cs;
    float xl = 0.0f;
    float yl = 0.0f;
    float zl = 0.0f;
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;
    float m = 0.0f; // steepness
    float cc = 0.0f; // function crossing the given axis

    enum class crdsysdim {
        XY,
        XZ,
        ZY
    };

    // Calculate vector length
    float lengthVector(float x, float y, float z, float stpx, float stpy, float stpz) {
        xl = std::fabs(stpx - x);
        cs.xlen.push_back(xl);

        yl = std::fabs(stpy - y);
        cs.ylen.push_back(yl);

        zl = std::fabs(stpz - z);
        cs.zlen.push_back(zl);

        float xzd = std::sqrt(xl * xl + zl * zl);
        float ydxz = std::sqrt(xzd * xzd + yl * yl);

        return ydxz;
    }
};
class objIdent {
public:
    Vector vec;
    Coord cs;
    int vectorindex = 0;
    int faceindex = 0;
    
    void objReader(const std::string& filename);
    void AABB(uint32_t vecnum);
    void Triangulator();
    void MoellerTrumbore(uint32_t vecnum);
};

class Transformation {
public:
    Coord cs;
    
    void linearTrans(int lane, float length);
    void turnTrans(int type, int degree);
};

#endif 