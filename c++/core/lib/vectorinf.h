#ifndef VECTORINF_H
#define VECTORINF_H
#include "3dvfunc.h"
#include <vector>
#include <cstdint>
#include <algorithm>


typedef struct {
  void *data;
  uint32_t size;
} Data;
float* CoordPointer[3];

objIdent obj;
extern "C" void LoadOBJ(const char *path, uint32_t coordnum) {
  obj.objReader(path);
  obj.Triangulator(coordnum);
}

extern "C" Data GetVector(uint32_t num, uint8_t vector, uint32_t raynum)
{
    Rays ray;
    Coord &cs = obj.objects[num];
    if(&obj.objects[num] == NULL) return {NULL, 0};
    switch(vector) {
    case 0:
    if (cs.usable.empty())
    {
      return {NULL, 0};
    }
    else
    {
      cs.usable.shrink_to_fit();
      return {cs.usable.data(), cs.usable.size()};
    }
    break;
    case 1:
    if (cs.face.empty())
    {
      return {NULL, 0};
    }
    else
    {
      return {cs.face.data(), cs.face.size()};
    }
    break;
    case 2:
    if (cs.vpc.empty())
    {
      return {NULL, 0};
    }
    else
    {
      
      
      return {cs.vpc.data(), cs.vpc.size()};
    }
    break;
    case 3:
    if (cs.triangles.empty())
    {
      return {NULL, 0};
    }
    else
    {
      
      return {cs.triangles.data(), cs.triangles.size()};
    }
    break;
    case 4:
    if (ray.stpxcoords.empty())
    {
      return {NULL, 0};
    }
    else
    {
      CoordPointer[0] = &(ray.stpxcoords[raynum]);
      CoordPointer[1] = &(ray.stpycoords[raynum]);
      CoordPointer[2] = &(ray.stpzcoords[raynum]);
      return {&CoordPointer[0], ray.stpxcoords.size()};
    }
    break;
    case 5:
    if (ray.xcoords.empty())
    {
      return {NULL, 0};
    }
    else
    {
      CoordPointer[0] = &(ray.xcoords[raynum]);
      CoordPointer[1] = &(ray.ycoords[raynum]);
      CoordPointer[2] = &(ray.zcoords[raynum]);
      return {&CoordPointer[0], ray.xcoords.size()};
    }
    break;
    }
  } 


#endif