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
std::vector<Coord> objIdent::objects;        
int main() {

    objIdent obj;

    obj.objReader("../../example/humanoid_quad.obj");
    for(size_t i =0; i< obj.objects[0].triangles.size(); i++) {
        std::cout << "Triangle: "<< obj.objects[0].triangles[i][0] << " " << obj.objects[0].triangles[i][1] << " " << obj.objects[0].triangles[i][2] << std::endl;
    };
    obj.objReader("../../example/monkey.obj");
    for(size_t i =0; i< obj.objects[1].triangles.size(); i++) {
        std::cout << "Triangle: "<< obj.objects[1].triangles[i][0] << " " << obj.objects[1].triangles[i][1] << " " << obj.objects[1].triangles[i][2] << std::endl;
    };
    /*obj.objects[0].stpxcoords.push_back(2.0f);
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
    }*/
}

