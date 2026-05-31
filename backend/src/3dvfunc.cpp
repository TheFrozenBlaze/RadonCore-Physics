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
extern "C" {

void LoadOBJ(const char *path, uint32_t coordnum) {
  obj.objReader(path);
  obj.Triangulator(coordnum);
}
}
extern "C" {
void *Element(uint8_t vecnum, uint32_t coordnum) {

  switch (vecnum)
  {
  case 0: {
    float *pointer_to_my_vector = &obj.objects[coordnum].vxc[0];
    return pointer_to_my_vector;
  }
  case 1: {
    float *pointer_to_my_vector = &obj.objects[coordnum].vyc[0];
    return pointer_to_my_vector;
  }
  case 2: {
    float *pointer_to_my_vector = &obj.objects[coordnum].vzc[0];
    return pointer_to_my_vector;
  }
  case 3: {
    void *triangle_data = &obj.objects[coordnum].triangles[0][0];
    return triangle_data;
  }
  }
};
}

extern "C" {
uint64_t Size(uint8_t vecnum,uint32_t coordnum) {
  switch (vecnum) {
  case 0:
    return obj.objects[coordnum].vxc.size();
  case 1:
    return obj.objects[coordnum].vyc.size();
  case 2:
    return obj.objects[coordnum].vzc.size();
  case 3:
    return obj.objects[coordnum].triangles.size();
  }
};
}

// ONLY FOR RENDERED OBJECTS



/*float transvert = cs.vyc[i]*horizontal + cs.vyc[i]*vertical;
 float transhor = cs.vxc[i]*vertical*(-1) + cs.vxc[i]*horizontal;*/
