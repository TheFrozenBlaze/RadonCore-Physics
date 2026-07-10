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


struct Coord {
public:
    // vertices
    std::vector<float> vpc;
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
    std::vector<std::array<uint32_t, 3>> triangles;
    std::vector<uint32_t> face;
    std::vector<std::vector<uint32_t>> intersections;
    std::array<std::array<float, 3>, 6> detailes;
};
class Rays {
  public:
    //endpoint
    std::vector<float> xcoords;
    std::vector<float> ycoords;
    std::vector<float> zcoords;
    //startpoint
    std::vector<float> stpxcoords;
    std::vector<float> stpycoords;
    std::vector<float> stpzcoords;
};
class Vector
{
public:
    Coord cs;
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
    float lengthVector(float x, float y, float z, float stpx, float stpy,
                     float stpz) {
      float ydxz = std::sqrt((x - stpx) * (x - stpx) + (y - stpy) * (y - stpy) + (z - stpz) * (z - stpz));
      return ydxz;
  }
};
class objIdent {
public:
    static std::vector<Coord> objects;
    
    void objReader(const std::string &filename) {
      objects.emplace_back();
      Coord &cs = objects.back();
      std::array<float, 3> minx, maxx, miny, maxy, minz, maxz;
      std::ifstream file(filename);
      if (!file.is_open())
      {
        std::cout << "Error reading file: " << filename << std::endl;
        return;
      }

    std::string line;
    while (std::getline(file, line)) {
      if (line.size() > 1 && line[0] == 'v' && line[1] == ' ') {
        std::istringstream iss(line);
        std::string v;
        float x, y, z;
        iss >> v >> x >> y >> z;
        minx = x < minx[0] ? std::array<float,3>{x, y,z} : minx;
        maxx = x > maxx[0] ? std::array<float,3>{x, y,z} : maxx;
        miny = y < miny[1] ? std::array<float,3>{x, y,z} : miny;
        maxy = y > maxy[1] ? std::array<float,3>{x, y,z} : maxy;
        minz = z < minz[2] ? std::array<float,3>{x, y,z} : minz;
        maxz = z > maxz[2] ? std::array<float,3>{x, y,z} : maxz;
        
        cs.vpc.push_back(x);
        cs.vpc.push_back(y);
        cs.vpc.push_back(z);
        //std::cout << "added: " << x << " " << y << " " << z << std::endl;
      }
      else if (line.size() > 1 && line[0] == 'f' && line[1] == ' ')
      {
          line.erase(0, 2);      // cuts of the 'f' and the ' ' from the beginning
          int point = 0; // where the f-function writes its outputs (((2=v, 3=vt, 4=vn))
          int number = 0;
          std::string numberstring; // loads the integers here

          for (size_t i {}; i < line.size(); i++)
          {
              if (line[i] == ' ' || i == line.size() - 1)
              { // where to write
                  if (i == line.size() - 1 && line[i] != ' ')
                  { // end of line write
                      numberstring.push_back(line[i]);
                      
                      
                      cs.face.emplace_back((std::stoi(numberstring)+(cs.vpc.size())/3) % (cs.vpc.size()/3 + 1));
                      numberstring.clear();
                      Triangulator(objects.size()-1);
                      cs.face.clear();
                      
                  }
                  else if (line[i] == ' ' && i != line.size() - 1)
                  {
                    if(numberstring.empty() == 1){
                    point = 0;
                    continue;
                    } else {
                      
                      cs.face.emplace_back((std::stoi(numberstring)+(cs.vpc.size())/3) % (cs.vpc.size()/3 + 1));                    
                      numberstring.clear();
                    }
                  } else if(point == 1) {
                    continue;
                  }  
              } else {
                if (line[i] == '/')
                  {
                    if(numberstring.empty())
                      continue;
                    cs.face.emplace_back((std::stoi(numberstring) + (cs.vpc.size()) / 3) % (cs.vpc.size() / 3 + 1));
                    numberstring.clear();
                    point = 1;
                    continue;
                  } else {
                    numberstring.push_back(line[i]);
                  }
              }
            }
          }
        }
    cs.vpc.shrink_to_fit();
    cs.face.shrink_to_fit();
    cs.detailes[0] = minx;
    cs.detailes[1] = maxx;
    cs.detailes[2] = miny;
    cs.detailes[3] = maxy;
    cs.detailes[4] = minz;
    cs.detailes[5] = maxz;
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

  // I would like to give credits and a huge thanks to 3Blue1Brown, who has
  // thought me the essence of algebra, and who thought me how to think in 3D.
  // Without his series creating this function would have been a much more
  // excruciating process, and I would have probably never finished it, so thank
  // you 3Blue1Brown!
  void Triangulator(uint32_t coordnum) {
    Coord& cs = objects[coordnum];
      float limit = 1e-6; // basicaly zero
      std::array<float, 3> normal = {0.0, 0.0, 0.0};
      uint8_t projection = 0;
      if (cs.face.size() > 3) {
        normal = {0.0, 0.0, 0.0};
        float x, y, z, coordy, coordx, coordz;
        int f = 1;
        while (
            normal ==
            std::array<float, 3>{
                0.0f, 0.0f,
                0.0f}) { // calculatin the normal of the face based on it's
                         // vertices, if the normal is 0, then the face is
                         // degenerate and we will try to calculate the normal
                         // with different vertices until we get a non
                         // degenerate normal or we run out of vertices to try
          if (f + 1 == cs.face.size()) {
            std::cout << "INVALID FACE" << std::endl;
            break;
          } else {
            coordx = (cs.vpc[cs.face[f]*3+1] -
                      cs.vpc[cs.face[0]*3+1]) *
                         (cs.vpc[cs.face[1 + f]*3+2] -
                          cs.vpc[cs.face[0]*3+2]) -
                     (cs.vpc[cs.face[1 + f]*3+1] -
                      cs.vpc[cs.face[0]*3+1]) *
                         (cs.vpc[cs.face[f]*3+2] -
                          cs.vpc[cs.face[0]*3+2]);
            coordy = (cs.vpc[cs.face[f]*3 +2] -
                      cs.vpc[cs.face[0]*3 +2]) *
                         (cs.vpc[cs.face[1 + f]*3] -
                          cs.vpc[cs.face[0]*3]) -
                     (cs.vpc[cs.face[1 + f]*3 +2] -
                      cs.vpc[cs.face[0]*3 +2]) *
                         (cs.vpc[cs.face[f]*3] -
                          cs.vpc[cs.face[0]*3]);
            coordz = (cs.vpc[cs.face[f]*3] -
                      cs.vpc[cs.face[0]*3]) *
                         (cs.vpc[cs.face[1 + f]*3+1] -
                          cs.vpc[cs.face[0]*3 +1]) -
                     (cs.vpc[cs.face[1 + f]*3] -
                      cs.vpc[cs.face[0]*3]) *
                         (cs.vpc[cs.face[f]*3+1] -
                          cs.vpc[cs.face[0]*3+1]);
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
        /*std::cout << "Normal" << normal[0] << " Normal0" <<  normal[1] << "
         * Normal1" << normal[2] << "Normal2" << std::endl;*/
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
        /*std::cout << "projection" << (int)projection << std::endl;*/
        std::list<uint32_t> vindices;//list for easy traversal and deletion of vertices
        uint32_t v0, v1, v2; //used for triangle math
        std::unordered_set<uint32_t> reflex; //reflex vertices, unordered set is useful because of find
        std::array<float, 2> rv1; //used for normal math
        std::array<float, 2> rv2;
        //std::cout << "I made it here" << std::endl;
        switch (projection)
        {
        case 0: {
          std::list<uint32_t>::iterator back;
          if (x > 0) { // we are ordering here based on winding. The normal
                       // tells us, which way are the vertices ordered
            for (size_t j {}; j < cs.face.size(); j++) {
              if (j < 2) {
                vindices.push_back(j);
              } else if (j >= 2 && j < cs.face.size() - 1) {
                vindices.push_back(j);
                v2 = cs.face[j - 2];//caLculating normals=> optimization compared to the last version 
                v1 = cs.face[j - 1];
                v0 = cs.face[j];
                rv1 = {cs.vpc[v1*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v1*3 +2] - cs.vpc[v2*3 +2]};
                rv2 = {cs.vpc[v0*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v0*3 +2] - cs.vpc[v2*3 + 2]};
                if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                  reflex.insert(j - 1);
                }
              } else {
                vindices.push_back(j);
                v2 = cs.face[j - 2];
                v1 = cs.face[j - 1];
                v0 = cs.face[j];
                rv1 = {cs.vpc[v1*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v1*3 +2] - cs.vpc[v2*3 +2]};
                rv2 = {cs.vpc[v0*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v0*3 +2] - cs.vpc[v2*3 + 2]};
                if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                  reflex.insert(j - 1);
                } // until here, the cycle is same as above
                v2 = v1;
                v1 = v0;
                v0 = cs.face[0];
                rv1 = {cs.vpc[v1*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v1*3 +2] - cs.vpc[v2*3 +2]};
                rv2 = {cs.vpc[v0*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v0*3 +2] - cs.vpc[v2*3 + 2]};
                if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                  reflex.insert(j); // calculating the reflexness of the last vertex
                }
                v2 = v1;
                v1 = v0;
                v0 = cs.face[1];
                rv1 = {cs.vpc[v1*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v1*3 +2] - cs.vpc[v2*3 +2]};
                rv2 = {cs.vpc[v0*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v0*3 +2] - cs.vpc[v2*3 + 2]};
                if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                  reflex.insert(0); //calculating the reflexness of the first vertex => all reflexness is calculated
                }
              }
            }
          } else {
            for (size_t j {}; j < cs.face.size(); j++) {
              if (j < 2) {
                vindices.push_back(cs.face.size() - 1 - j);
              } else if (j >= 2 && j < cs.face.size() - 1) {
                vindices.push_back(cs.face.size() - 1 - j);
                v2 = cs.face[cs.face.size() + 1 - j];
                v1 = cs.face[cs.face.size() - j];
                v0 = cs.face[cs.face.size() - 1 - j];
                rv1 = {cs.vpc[v1*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v1*3 +2] - cs.vpc[v2*3 +2]};
                rv2 = {cs.vpc[v0*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v0*3 +2] - cs.vpc[v2*3 + 2]};
                if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                  reflex.insert(cs.face.size() - j);
                }
              } else {
                vindices.push_back(cs.face.size() - 1 - j);
                v2 = cs.face[cs.face.size() + 1 - j];
                v1 = cs.face[cs.face.size() - j];
                v0 = cs.face[cs.face.size() - 1 - j];
                rv1 = {cs.vpc[v1*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v1*3 +2] - cs.vpc[v2*3 +2]};
                rv2 = {cs.vpc[v0*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v0*3 +2] - cs.vpc[v2*3 + 2]};
                if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                  reflex.insert(cs.face.size() - j);
                } // until here, the cycle is same as above
                v2 = v1;
                v1 = v0;
                v0 = cs.face[*(vindices.begin())];
                rv1 = {cs.vpc[v1*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v1*3 +2] - cs.vpc[v2*3 +2]};
                rv2 = {cs.vpc[v0*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v0*3 +2] - cs.vpc[v2*3 + 2]};
                if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                  reflex.insert(0);
                }
                v2 = v1;
                v1 = v0;
                v0 = cs.face[*(std::next(vindices.begin(), 1))];
                rv1 = {cs.vpc[v1*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v1*3 +2] - cs.vpc[v2*3 +2]};
                rv2 = {cs.vpc[v0*3 +1] - cs.vpc[v2*3 +1], cs.vpc[v0*3 +2] - cs.vpc[v2*3 + 2]};
                if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                  reflex.insert(cs.face.size() - 1);
                }
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
              //std::cout << "I made it to while loop in case 0" << std::endl;
              reflex.clear();
              while (vindices.size() >= 3)
              {
                  if (reflex.empty() == 1)
                  { // O(n) check for reflex vertices, if there are none, then we can be sure that all the triangles are valid,\
                      // and we can just triangulate the rest of the polygon without checks=> imporvement compared to the last version,
                      cs.triangles.emplace_back(std::array<uint32_t, 3>{
                          cs.face[*vindices.begin()],
                          cs.face[*step],
                          cs.face[*(std::next(step, 1))]});
                      step = vindices.erase(step);
                      // std::cout<< "face valid" << std::endl;
                      // std::cout k++;<< "I have removed: " << v1+1 << std::endl;
                      // std::cout << "Triangle
                      // "<<cs.triangles[cs.triangles.size()-1][0] << " " <<
                      // cs.triangles[cs.triangles.size()-1][1] << " "<<
                      // cs.triangles[cs.triangles.size()-1][2]<< std::endl;
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
                          v0 = cs.face[*prev1];                   // first vertex of the triangle
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
                              firstelement = vindices.erase(firstelement);
                              nextnext++;
                              switch (switcher)
                              {
                              case 1:
                                  rv1 = {cs.vpc[v0*3+1] - cs.vpc[cs.face[*prev2]*3+1],
                                         cs.vpc[v0*3+2] -
                                             cs.vpc[cs.face[*prev2]*3 +2]};
                                  rv2 = {cs.vpc[v1*3+1] - cs.vpc[cs.face[*prev2]*3+1],
                                         cs.vpc[v1*3+2] -
                                             cs.vpc[cs.face[*prev2]*3+2]};
                                  if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
                                  {
                                      reflex.erase(*prev1);
                                  }
                                  break;
                              case 2:
                                  rv1 = {cs.vpc[v1*3+1] - cs.vpc[v0*3+2],
                                         cs.vpc[v1*3+2] - cs.vpc[v0*3+2]};
                                  rv2 = {cs.vpc[v2*3 +1] - cs.vpc[v0*3+1],
                                         cs.vpc[v2*3+2] - cs.vpc[v0*3+2]};
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
                                      cs.vpc[cs.face[*(std::next(reflex.begin(), j))]*3+1],
                                      cs.vpc[cs.face[*(std::next(reflex.begin(), j))]*3+2]}; // the vertex of the polygon
                                  std::array<float, 2> tp1 = {cs.vpc[v0*3+1], cs.vpc[v0*3+2]};
                                  std::array<float, 2> tp2 = {cs.vpc[v1*3+1], cs.vpc[v1*3+2]};
                                  std::array<float, 2> tp3 = {cs.vpc[v2*3+1], cs.vpc[v2*3+2]};
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
                              { // in the case of the triangle being valid
                                  cs.triangles.emplace_back(
                                      std::array<uint32_t, 3>{
                                          v0, cs.face[*firstelement], v1});
                                  firstelement = vindices.erase(firstelement);

                                  switch (switcher)
                              {
                              case 1:
                                  rv1 = {cs.vpc[v0*3+1] - cs.vpc[cs.face[*prev2]*3+1],
                                         cs.vpc[v0*3+2] -
                                             cs.vpc[cs.face[*prev2]*3 +2]};
                                  rv2 = {cs.vpc[v1*3+1] - cs.vpc[cs.face[*prev2]*3+1],
                                         cs.vpc[v1*3+2] -
                                             cs.vpc[cs.face[*prev2]*3+2]};
                                  if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0)
                                  {
                                      reflex.erase(*prev1);
                                  }
                                  break;
                              case 2:
                                  rv1 = {cs.vpc[v1*3+1] - cs.vpc[v0*3+2],
                                         cs.vpc[v1*3+2] - cs.vpc[v0*3+2]};
                                  rv2 = {cs.vpc[v2*3 +1] - cs.vpc[v0*3+1],
                                         cs.vpc[v2*3+2] - cs.vpc[v0*3+2]};
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
                                      cs.vpc[cs.face[*(std::next(reflex.begin(), j))]*3+1],
                                      cs.vpc[cs.face[*(std::next(reflex.begin(), j))]*3+2]}; // the vertex of the polygon
                                  std::array<float, 2> tp1 = {cs.vpc[v0*3+1], cs.vpc[v0*3+2]};
                                  std::array<float, 2> tp2 = {cs.vpc[v1*3+1], cs.vpc[v1*3+2]};
                                  std::array<float, 2> tp3 = {cs.vpc[v2*3+1], cs.vpc[v2*3+2]};
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
              
            case 1: {
              std::list<uint32_t>::iterator back;
              if (x > 0) { // we are ordering here based on winding. The normal
                           // tells us, which way are the vertices ordered
                for (size_t j {}; j < cs.face.size(); j++) {
                  if (j < 2) {
                    vindices.push_back(j);
                  } else if (j >= 2 && j < cs.face.size() - 1) {
                    vindices.push_back(j);
                    v2 = cs.face[j - 2];
                    v1 = cs.face[j - 1];
                    v0 = cs.face[j];
                    rv1 = {cs.vpc[v1*3] - cs.vpc[v2*3], cs.vpc[v1*3+2] - cs.vpc[v2*3+2]};
                    rv2 = {cs.vpc[v0*3] - cs.vpc[v2*3], cs.vpc[v0*3+2] - cs.vpc[v2*3+2]};
                    if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                      reflex.insert(j - 1);
                    }
                  } else {
                    vindices.push_back(j);
                    v2 = cs.face[j - 2];
                    v1 = cs.face[j - 1];
                    v0 = cs.face[j];
                    rv1 = {cs.vpc[v1*3] - cs.vpc[v2*3], cs.vpc[v1*3+2] - cs.vpc[v2*3+2]};
                    rv2 = {cs.vpc[v0*3] - cs.vpc[v2*3], cs.vpc[v0*3+2] - cs.vpc[v2*3+2]};
                    if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                      reflex.insert(j - 1);
                    } // until here, the cycle is same as above
                    v2 = v1;
                    v1 = v0;
                    v0 = cs.face[0];
                    rv1 = {cs.vpc[v1*3] - cs.vpc[v2*3], cs.vpc[v1*3+2] - cs.vpc[v2*3+2]};
                    rv2 = {cs.vpc[v0*3] - cs.vpc[v2*3], cs.vpc[v0*3+2] - cs.vpc[v2*3+2]};
                    if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                      reflex.insert(j);
                    }
                    v2 = v1;
                    v1 = v0;
                    v0 = cs.face[1];
                    rv1 = {cs.vpc[v1*3] - cs.vpc[v2*3], cs.vpc[v1*3+2] - cs.vpc[v2*3+2]};
                    rv2 = {cs.vpc[v0*3] - cs.vpc[v2*3], cs.vpc[v0*3+2] - cs.vpc[v2*3+2]};
                    if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                      reflex.insert(0);
                    }
                  }
                }
              } else {
                for (size_t j {}; j < cs.face.size(); j++) {
                  if (j < 2) {
                    vindices.push_back(cs.face.size() - 1 - j);
                  } else if (j >= 2 && j < cs.face.size() - 1) {
                    vindices.push_back(cs.face.size() - 1 - j);
                    v2 = cs.face[cs.face.size() + 1 - j];
                    v1 = cs.face[cs.face.size() - j];
                    v0 = cs.face[cs.face.size() - 1 - j];
                    rv1 = {cs.vpc[v1*3] - cs.vpc[v2*3], cs.vpc[v1*3+2] - cs.vpc[v2*3+2]};
                    rv2 = {cs.vpc[v0*3] - cs.vpc[v2*3], cs.vpc[v0*3+2] - cs.vpc[v2*3+2]};
                    if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                      reflex.insert(cs.face.size() - j);
                    }
                  } else {
                    vindices.push_back(cs.face.size() - 1 - j);
                    v2 = cs.face[cs.face.size() + 1 - j];
                    v1 = cs.face[cs.face.size() - j];
                    v0 = cs.face[cs.face.size() - 1 - j];
                    rv1 = {cs.vpc[v1*3] - cs.vpc[v2*3], cs.vpc[v1*3+2] - cs.vpc[v2*3+2]};
                    rv2 = {cs.vpc[v0*3] - cs.vpc[v2*3], cs.vpc[v0*3+2] - cs.vpc[v2*3+2]};
                    if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                      reflex.insert(cs.face.size() - j);
                    } // until here, the cycle is same as above
                    v2 = v1;
                    v1 = v0;
                    v0 = cs.face[*(vindices.begin())];
                    rv1 = {cs.vpc[v1*3] - cs.vpc[v2*3], cs.vpc[v1*3+2] - cs.vpc[v2*3+2]};
                    rv2 = {cs.vpc[v0*3] - cs.vpc[v2*3], cs.vpc[v0*3+2] - cs.vpc[v2*3+2]};
                    if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                      reflex.insert(0);
                    }
                    v2 = v1;
                    v1 = v0;
                    v0 = cs.face[*(std::next(vindices.begin(), 1))];
                    rv1 = {cs.vpc[v1*3] - cs.vpc[v2*3], cs.vpc[v1*3+2] - cs.vpc[v2*3+2]};
                    rv2 = {cs.vpc[v0*3] - cs.vpc[v2*3], cs.vpc[v0*3+2] - cs.vpc[v2*3+2]};
                    if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                      reflex.insert(cs.face.size() - 1);
                    }
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
                        
                        /*std::cout << "Triangle: " << cs.fvi[i].ver[*vindices.begin()].v - 1 << " "
                                  << cs.fvi[i].ver[*step].v - 1 << " "
                                  << cs.fvi[i].ver[*(std::next(step, 1))].v - 1 << std::endl;*/
                        step = vindices.erase(step);

                        // std::cout<< "face valid" << std::endl;
                        // std::cout k++;<< "I have removed: " << v1+1 <<
                        // std::endl; std::cout << "Triangle
                        // "<<cs.triangles[cs.triangles.size()-1][0] << " " <<
                        // cs.triangles[cs.triangles.size()-1][1] << " "<<
                        // cs.triangles[cs.triangles.size()-1][2]<< std::endl;
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
                        } else {
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

                        if (reflexelement == reflex.size()) {
                          cs.triangles.emplace_back(
                              std::array<uint32_t, 3>{
                                  v0, cs.face[*firstelement], v1});
                          firstelement = vindices.erase(firstelement);
                          /*std::cout << "reflex.size() is reflexelement" << std::endl;
                          std::cout << "Triangle: " << v0 << " "
                                << cs.fvi[i].ver[*firstelement].v - 1 << " "
                                << v1 << std::endl;*/
                          nextnext++;
                          switch (switcher) {
                          case 1:
                            rv1 = {cs.vpc[v0*3] -
                                       cs.vpc[cs.face[*prev2]*3],
                                   cs.vpc[v0*3+2] -
                                       cs.vpc[cs.face[*prev2]*3+2]};
                            rv2 = {cs.vpc[v1*3] -
                                       cs.vpc[cs.face[*prev2]*3],
                                   cs.vpc[v1*3+2] -
                                       cs.vpc[cs.face[*prev2]*3+2]};
                            if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                              reflex.erase(*prev1);
                            }
                            break;
                          case 2:
                            rv1 = {cs.vpc[v1*3] - cs.vpc[v0*3],
                                   cs.vpc[v1*3+2] - cs.vpc[v0*3+2]};
                            rv2 = {cs.vpc[v2*3] - cs.vpc[v0*3],
                                   cs.vpc[v2*3+2] - cs.vpc[v0*3+2]};
                            if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                              reflex.erase(*firstelement);

                              break;
                            }
                          }
                        } else if (reflexelement > 0) {
                          //std::cout << "reflex.size() is > reflexelement" << std::endl;
                          valid = false;
                          for (size_t j = 0; j < reflex.size(); j++) {
                            std::array<float, 2> p = {
                                cs.vpc[cs.face[*(std::next(reflex.begin(), j))]*3],
                                cs.vpc[cs.face[*(std::next(reflex.begin(), j))]*3+2]}; // the vertex of the polygon
                            std::array<float, 2> tp1 = {cs.vpc[v0*3], cs.vpc[v0*3+2]};
                            std::array<float, 2> tp2 = {cs.vpc[v1*3], cs.vpc[v1*3+2]};
                            std::array<float, 2> tp3 = {cs.vpc[v2*3], cs.vpc[v2*3+2]};
                            struct CPpair {
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
                                true) { // we exclude go step 1 forward in
                                        // potential triangle vertices, because
                                        // the last 1 was invalid
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
                          if (valid == false) {
                            cs.triangles.emplace_back(
                                std::array<uint32_t, 3>{
                                    v0, cs.face[*firstelement],
                                    v1});
                            firstelement = vindices.erase(firstelement);
                            
                            /*std::cout << "Triangle: " << v0 << " "
                                << cs.fvi[i].ver[*firstelement].v - 1 << " "
                                << v1 << std::endl;*/
                            switch (switcher) {
                            case 1:
                              rv1 = {cs.vpc[v0*3] -
                                        cs.vpc[cs.face[*prev2]*3],
                                    cs.vpc[v0*3+2] -
                                        cs.vpc[cs.face[*prev2]*3+2]};
                              rv2 = {cs.vpc[v1*3] -
                                        cs.vpc[cs.face[*prev2]*3],
                                    cs.vpc[v1*3+2] -
                                        cs.vpc[cs.face[*prev2]*3+2]};
                              if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                reflex.erase(*prev1);
                              }
                              break;
                            case 2:
                              rv1 = {cs.vpc[v1*3] - cs.vpc[v0*3],
                                    cs.vpc[v1*3+2] - cs.vpc[v0*3+2]};
                              rv2 = {cs.vpc[v2*3] - cs.vpc[v0*3],
                                    cs.vpc[v2*3+2] - cs.vpc[v0*3+2]};
                              if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                reflex.erase(*firstelement);

                                break;
                              }
                            }
                          }
                        } else {
                          valid = false;
                          //std::cout << "reflexelement is 0" << std::endl;
                          for (size_t j = 0; j < reflex.size(); j++) {
                            std::array<float, 2> p = {
                                cs.vpc[cs.face[*(std::next(reflex.begin(), j))]*3],
                                cs.vpc[cs.face[*(std::next(reflex.begin(), j))]*3+2]}; // the vertex of the polygon
                            std::array<float, 2> tp1 = {cs.vpc[v0*3], cs.vpc[v0*3+2]};
                            std::array<float, 2> tp2 = {cs.vpc[v1*3], cs.vpc[v1*3+2]};
                            std::array<float, 2> tp3 = {cs.vpc[v2*3], cs.vpc[v2*3+2]};
                            struct CPpair {
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
                                true) { // we exclude go step 1 forward in
                                        // potential triangle vertices, because
                                        // the last 1 was invalid
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
                          if (valid == false) {
                            cs.triangles.emplace_back(
                                std::array<uint32_t, 3>{
                                    v0, cs.face[*firstelement],
                                    v1});
                                    //std::cout << "Triangle: " << v0 << " "<< cs.fvi[i].ver[*firstelement].v - 1 << " "<< v1 << std::endl;
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
          case 2: {
            std::list<uint32_t>::iterator back;
            if (x > 0) { // we are ordering here based on winding. The normal
                         // tells us, which way are the vertices ordered
              for (size_t j = 0; j < cs.face.size(); j++) {
                if (j < 2) {
                  vindices.push_back(j);
                } else if (j >= 2 && j < cs.face.size() - 1) {
                  vindices.push_back(j);
                  v2 = cs.face[j - 2];
                  v1 = cs.face[j - 1];
                  v0 = cs.face[j];
                  rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                    reflex.insert(j - 1);
                  }
                } else {
                  vindices.push_back(j);
                  v2 = cs.face[j - 2];
                  v1 = cs.face[j - 1];
                  v0 = cs.face[j];
                  rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                    reflex.insert(j - 1);
                  } // until here, the cycle is same as above
                  v2 = v1;
                  v1 = v0;
                  v0 = cs.face[0];
                  rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                    reflex.insert(j);
                  }
                  v2 = v1;
                  v1 = v0;
                  v0 = cs.face[1];
                  rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                    reflex.insert(0);
                  }
                }
              }
            } else {
              for (int j = 0; j < cs.face.size(); j++) {
                if (j < 2) {
                  vindices.push_back(cs.face.size() - 1 - j);
                } else if (j >= 2 && j < cs.face.size() - 1) {
                  vindices.push_back(cs.face.size() - 1 - j);
                  v2 = cs.face[cs.face.size() + 1 - j];
                  v1 = cs.face[cs.face.size() - j];
                  v0 = cs.face[cs.face.size() - 1 - j];
                  rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                    reflex.insert(cs.face.size() - j);
                  }
                } else {
                  vindices.push_back(cs.face.size() - 1 - j);
                  v2 = cs.face[cs.face.size() + 1 - j];
                  v1 = cs.face[cs.face.size() - j];
                  v0 = cs.face[cs.face.size() - 1 - j];
                  rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                    reflex.insert(cs.face.size() - j);
                  } // until here, the cycle is same as above
                  v2 = v1;
                  v1 = v0;
                  v0 = cs.face[*(vindices.begin())];
                  rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                    reflex.insert(0);
                  }
                  v2 = v1;
                  v1 = v0;
                  v0 = cs.face[*(std::next(vindices.begin(), 1))];
                  rv1 = {cs.vpc[v1 * 3] - cs.vpc[v2 * 3], cs.vpc[v1 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  rv2 = {cs.vpc[v0 * 3] - cs.vpc[v2 * 3], cs.vpc[v0 * 3 + 1] - cs.vpc[v2 * 3 + 1]};
                  if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                    reflex.insert(cs.face.size() - 1);
                  }
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
                while (vindices.size() >= 3) {
                  if (reflex.empty() == 1) {
                    cs.triangles.emplace_back(std::array<uint32_t, 3>{
                        cs.face[*vindices.begin()],
                        cs.face[*step],
                        cs.face[*(std::next(step, 1))]});
                    step = vindices.erase(step);
                    // std::cout<< "face valid" << std::endl;
                    // std::cout k++;<< "I have removed: " << v1+1 << std::endl;
                    // std::cout << "Triangle
                    // "<<cs.triangles[cs.triangles.size()-1][0] << " " <<
                    // cs.triangles[cs.triangles.size()-1][1] << " "<<
                    // cs.triangles[cs.triangles.size()-1][2]<< std::endl;
                  } else {
                    if (reflex.find(*firstelement) != reflex.end()) {
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
                    } else {
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

                      if (reflexelement == reflex.size()) {
                        cs.triangles.emplace_back(
                            std::array<uint32_t, 3>{
                                v0, cs.face[*firstelement], v1});
                        firstelement = vindices.erase(firstelement);
                        nextnext++;
                        switch (switcher) {
                        case 1:
                          rv1 = {
                              cs.vpc[v0*3] - cs.vpc[cs.face[*prev2]*3],
                              cs.vpc[v0*3+1] - cs.vpc[cs.face[*prev2]*3+1]};
                          rv2 = {
                              cs.vpc[v1*3] - cs.vpc[cs.face[*prev2]*3],
                              cs.vpc[v1*3+1] - cs.vpc[cs.face[*prev2]*3+1]};
                          if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                            reflex.erase(*prev1);
                          }
                          break;
                        case 2:
                          rv1 = {cs.vpc[v1*3] - cs.vpc[v0*3],
                                 cs.vpc[v1*3+1] - cs.vpc[v0*3+1]};
                          rv2 = {cs.vpc[v2*3] - cs.vpc[v0*3],
                                 cs.vpc[v2*3+1] - cs.vpc[v0*3+1]};
                          if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                            reflex.erase(*firstelement);

                            break;
                          }
                        }
                      } else if (reflexelement > 0) {
                        valid = false;
                        for (size_t j = 0; j < reflex.size(); j++) {
                          std::array<float, 2> p = {
                              cs.vpc[cs.face[*(std::next(reflex.begin(), j))]*3],
                              cs.vpc[cs.face[*(std::next(reflex.begin(), j))]*3+1]}; // the vertex of the polygon
                          std::array<float, 2> tp1 = {cs.vpc[v0*3], cs.vpc[v0*3+1]};
                          std::array<float, 2> tp2 = {cs.vpc[v1*3], cs.vpc[v1*3+1]};
                          std::array<float, 2> tp3 = {cs.vpc[v2*3], cs.vpc[v2*3+1]};
                          struct CPpair {
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
                              true) { // we exclude go step 1 forward in
                                      // potential triangle vertices, because
                                      // the last 1 was invalid
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
                        if (valid == false) {
                          cs.triangles.emplace_back(
                              std::array<uint32_t, 3>{
                                  v0, cs.face[*firstelement], v1});
                          firstelement = vindices.erase(firstelement);

                          switch (switcher) {
                          case 1:
                            rv1 = {
                                cs.vpc[v0*3] - cs.vpc[cs.face[*prev2]*3],
                                cs.vpc[v0*3+1] - cs.vpc[cs.face[*prev2]*3+1]};
                            rv2 = {
                                cs.vpc[v1*3] - cs.vpc[cs.face[*prev2]*3],
                                cs.vpc[v1*3+1] - cs.vpc[cs.face[*prev2]*3+1]};
                            if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                              reflex.erase(*prev1);
                            }
                            break;
                          case 2:
                            rv1 = {cs.vpc[v1*3] - cs.vpc[v0*3],
                                  cs.vpc[v1*3+1] - cs.vpc[v0*3+1]};
                            rv2 = {cs.vpc[v2*3] - cs.vpc[v0*3],
                                  cs.vpc[v2*3+1] - cs.vpc[v0*3+1]};
                            if (rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                              reflex.erase(*firstelement);

                              break;
                            }
                          }
                        }
                      } else {
                        valid = false;
                        for (size_t j = 0; j < reflex.size(); j++) {
                          std::array<float, 2> p = {
                              cs.vpc[cs.face[*(std::next(reflex.begin(), j))]*3],
                              cs.vpc[cs.face[*(std::next(reflex.begin(), j))]*3+1]}; // the vertex of the polygon
                          std::array<float, 2> tp1 = {cs.vpc[v0*3], cs.vpc[v0*3+1]};
                          std::array<float, 2> tp2 = {cs.vpc[v1*3], cs.vpc[v1*3+1]};
                          std::array<float, 2> tp3 = {cs.vpc[v2*3], cs.vpc[v2*3+1]};
                          struct CPpair {
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
                              true) { // we exclude go step 1 forward in
                                      // potential triangle vertices, because
                                      // the last 1 was invalid
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
                        if (valid == false) {
                          cs.triangles.emplace_back(
                              std::array<uint32_t, 3>{
                                  v0, cs.face[*firstelement], v1});
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
          cs.triangles.shrink_to_fit();
      }
      else
      {
        cs.triangles.emplace_back(std::array<uint32_t, 3>{
            cs.face[0], cs.face[1],
            cs.face[2]}); // we pass on shapes, that are triangles
      }
    }
  
void MoellerTrumbore(uint32_t vecnum, uint32_t coordnum)
{
  // AABB(vecnum, coordnum);
  Coord &cs = objects[coordnum];
  Rays ray;
  float facedotprod;
  cs.intersections.resize(cs.intersections.size() + 1);
  cs.intersections.back().emplace_back(vecnum);
  for (size_t i = 0; i < cs.usable.size(); i++)
  {
    size_t j = cs.usable[i];
    std::array<float, 3> v0 = {
        cs.vpc[cs.triangles[j][0]*3] - ray.stpxcoords[vecnum],
        cs.vpc[cs.triangles[j][0]*3+1] - ray.stpycoords[vecnum],
        cs.vpc[cs.triangles[j][0]*3+2] - ray.stpzcoords[vecnum]};
    std::array<float, 3> v1 = {
        cs.vpc[cs.triangles[j][1]*3] - ray.stpxcoords[vecnum],
        cs.vpc[cs.triangles[j][1]*3+1] - ray.stpycoords[vecnum],
        cs.vpc[cs.triangles[j][1]*3+2] - ray.stpzcoords[vecnum]};
    std::array<float, 3> v2 = {
        cs.vpc[cs.triangles[j][2]*3] - ray.stpxcoords[vecnum],
        cs.vpc[cs.triangles[j][2]*3+1] - ray.stpycoords[vecnum],
        cs.vpc[cs.triangles[j][2]*3+2] - ray.stpzcoords[vecnum]};
    std::array<float, 3> edge1 = {v1[0] - v0[0], v1[1] - v0[1],
                                  v1[2] - v0[2]};
    std::array<float, 3> edge2 = {v2[0] - v0[0], v2[1] - v0[1],
                                  v2[2] - v0[2]};
    std::array<float, 3> normal = {edge2[2] * edge1[1] - edge2[1] * edge1[2],
                                   edge2[0] * edge1[2] - edge2[2] * edge1[0],
                                   edge2[1] * edge1[0] - edge2[0] * edge1[1]};
    facedotprod = (ray.xcoords[vecnum] - ray.stpxcoords[vecnum]) * normal[0] +
                  (ray.ycoords[vecnum] - ray.stpycoords[vecnum]) * normal[1] +
                  (ray.zcoords[vecnum] - ray.stpzcoords[vecnum]) * normal[2];
    if (std::fabs(facedotprod) < 1e-6)
      continue; // Ray is parallel to triangle
    float t =
        -(-(normal[0] * v0[0] + normal[1] * v0[1] + normal[2] * v0[2])) /
        (normal[0] * (ray.xcoords[vecnum] - ray.stpxcoords[vecnum]) +
         normal[1] * (ray.ycoords[vecnum] - ray.stpycoords[vecnum]) +
         normal[2] * (ray.zcoords[vecnum] - ray.stpzcoords[vecnum]));
    if (t < 0 || t > 1)
    {
      std::cerr << "Vector meets the plane outside \n";
      continue;
    }
    std::array<float, 3> P = {
        t * (ray.xcoords[vecnum] - ray.stpxcoords[vecnum]),
        t * (ray.ycoords[vecnum] - ray.stpycoords[vecnum]),
        t * (ray.zcoords[vecnum] - ray.stpzcoords[vecnum])};
    std::array<float, 3> v0v1P = {
        (v1[1] - v0[1]) * (P[2] - v0[2]) - (v1[2] - v0[2]) * (P[1] - v0[1]),
        (v1[2] - v0[2]) * (P[0] - v0[0]) - (v1[0] - v0[0]) * (P[2] - v0[2]),
        (v1[0] - v0[0]) * (P[1] - v0[1]) - (v1[1] - v0[1]) * (P[0] - v0[0])};
    if ((v0v1P[0] * normal[0] + v0v1P[1] * normal[1] + v0v1P[2] * normal[2]) <
        0)
      continue;
    std::array<float, 3> v1v2P = {
        (v2[1] - v1[1]) * (P[2] - v1[2]) - (v2[2] - v1[2]) * (P[1] - v1[1]),
        (v2[2] - v1[2]) * (P[0] - v1[0]) - (v2[0] - v1[0]) * (P[2] - v1[2]),
        (v2[0] - v1[0]) * (P[1] - v1[1]) - (v2[1] - v1[1]) * (P[0] - v1[0])};
    if (v1v2P[0] * normal[0] + v1v2P[1] * normal[1] + v1v2P[2] * normal[2] <
        0)
      continue;
    std::array<float, 3> v2v0P = {
        (v0[1] - v2[1]) * (P[2] - v2[2]) - (v0[2] - v2[2]) * (P[1] - v2[1]),
        (v0[2] - v2[2]) * (P[0] - v2[0]) - (v0[0] - v2[0]) * (P[2] - v2[2]),
        (v0[0] - v2[0]) * (P[1] - v2[1]) - (v0[1] - v2[1]) * (P[0] - v2[0])};
    if (v2v0P[0] * normal[0] + v2v0P[1] * normal[1] + v2v0P[2] * normal[2] <
        0)
      continue;

    cs.intersections.back().emplace_back(j);
  }
  if (cs.intersections.back().size() == 1)
  {
    cs.intersections.resize(cs.intersections.size() - 1);
  }
  cs.usable.clear();
}
};

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