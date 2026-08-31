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
#include "3dvfunc.h"
#include "opencllink.h"

// #include <chrono>
// #include <thread>

  

  // Convert vector line to function form
  /*void funcVector(int num, std::string level) {
      crdsysdim state;
      std::transform(level.begin(), level.end(), level.begin(), [](unsigned char
  c) { return std::toupper(c); }); if (level == "XY") {state = crdsysdim::XY;}
      else if (level == "XZ") {state = crdsysdim::XZ;}
      else {state = crdsysdim::ZY;}

      float stpdom = 0.0f;
      float dx = 0.0f;
      float dy = 0.0f;

      switch (state) {
          case crdsysdim::XY:
              dx = cs.stpxcoords[num] - cs.xcoords[num];
              dy = cs.stpycoords[num] - cs.ycoords[num];
              stpdom = cs.stpycoords[num];
              nondom = cs.stpxcoords[num];
              break;
          case crdsysdim::XZ:
              dx = cs.stpxcoords[num] - cs.xcoords[num];
              dy = cs.stpzcoords[num] - cs.zcoords[num];
              stpdom = cs.stpzcoords[num];
              nondom = cs.stpxcoords[num];
              break;
          case crdsysdim::ZY:
              dx = cs.stpzcoords[num] - cs.zcoords[num];
              dy = cs.stpycoords[num] - cs.ycoords[num];
              stpdom = cs.stpycoords[num];
              nondom = cs.stpzcoords[num];
              break;
      }


      if (dx == 0.0f) {
          m = INFINITY;
      } else {
          m = dy / dx;
      }

      float cc = stpdom-m*nondom;

  }*/
  /*correction:my solution was after fixes: double degree = std::atan(dy/dx);
   * float cc= stpdom - std::tan(degree)*nondom; but chatgpt made me realize,
   * that tan(degree) is just 'm', and I don't have to deal with division by
   * zero and trig...(mentioned, because in this challenge ai isn't allowed to
   * make my code, at max give me hints) - this will be later repurposed and
   * refined, but this is one of my greatest creations for now, and I want to
   * preserve it in its almost original form in its original place*/



  

objIdent obj;


extern "C" __attribute__((visibility("default"))) void LoadOBJ(const char *path, uint32_t coordnum) {
  obj.objReader(path);
  obj.Triangulator(coordnum);
}

// opencl linker

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
    if (cs.fvi.empty())
    {
      return {NULL, 0};
    }
    else
    {
      return {cs.fvi.data(), cs.fvi.size()};
    }
    break;
    case 2:
    if (cs.vxc.empty())
    {
      return {NULL, 0};
    }
    else
    {
      
      CoordPointer[0] = cs.vxc.data();
      CoordPointer[1] = cs.vyc.data();
      CoordPointer[2] = cs.vzc.data();
      return {&CoordPointer[0], cs.fvi.size()};
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


// ONLY FOR RENDERED OBJECTS



/*float transvert = cs.vyc[i]*horizontal + cs.vyc[i]*vertical;
 float transhor = cs.vxc[i]*vertical*(-1) + cs.vxc[i]*horizontal;*/
