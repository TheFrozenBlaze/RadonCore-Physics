#include "3dvfunc.h"
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
               
int main() {
    objIdent obj;


    obj.objReader("/home/matyas/Downloads/humanoid_quad.obj");
    for(size_t i = 0; i < obj.objects[0].fvi.size(); i++) {
        for(size_t j =0; j < obj.objects[0].fvi[i].ver.size(); j++) {
            std::cout << obj.objects[0].fvi[i].ver[j].v << std::endl;
        }
    }
    clock_t before = clock();
    obj.Triangulator(0);
    clock_t duration = clock() - before;
    for(size_t i =0; i< obj.objects[0].triangles.size(); i++) {
        std::cout << "Triangle: "<< obj.objects[0].triangles[i][0] << " " << obj.objects[0].triangles[i][1] << " " << obj.objects[0].triangles[i][2] << std::endl;
    };
    std::cout << obj.objects[0].triangles.size() << std::endl;
    std::cout << "Triangulation duration: " << (float)duration/CLOCKS_PER_SEC << " seconds" << std::endl;
    obj.objects[0].stpxcoords.push_back(2.0f);
    obj.objects[0].stpycoords.push_back(0.0f);
    obj.objects[0].stpzcoords.push_back(0.0f);

    obj.objects[0].xcoords.push_back(2.0f);
    obj.objects[0].ycoords.push_back(0.0f);
    obj.objects[0].zcoords.push_back(20.0f);
    obj.AABB(0, 0);
    for(size_t i = 0; i< obj.objects[0].usable.size(); i++) {
        std::cout << "Usable: " << obj.objects[0].usable[i] << std::endl;
    }
    obj.objects[0].usable.clear();
    

    obj.MoellerTrumbore(0, 0);
    for(size_t i = 0; i< obj.objects[0].intersections[0].size(); i++) {
        if(i==0) {
            continue;
        } else {
        std::cout << "Crossed face: " << obj.objects[0].intersections[0][i] << std::endl;
        }
    }
    obj.objects[0].usable.clear();
    obj.objects[0].stpxcoords.push_back(-5.0f);
    obj.objects[0].stpycoords.push_back(0.0f);
    obj.objects[0].stpzcoords.push_back(11.0f);

    obj.objects[0].xcoords.push_back(10.0f);
    obj.objects[0].ycoords.push_back(0.0f);
    obj.objects[0].zcoords.push_back(11.0f);
    std::cout << "second face" << std::endl;
    obj.MoellerTrumbore(1, 0);
    for(size_t i = 0; i< obj.objects[0].intersections[1].size(); i++) {
        if(i==0) {
            continue;
        } else {
        std::cout << "Crossed face: " << obj.objects[0].intersections[1][i] << std::endl;
        }
    }
}

