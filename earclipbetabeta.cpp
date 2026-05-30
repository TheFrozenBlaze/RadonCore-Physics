#include <list>
#include <array>
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <cstdint>
#include <algorithm>
#include <iterator>

void Triangulator() {
    for(size_t i = 0; i < cs.fvi.size(); i++) {
        float limit = 1e-6; //basicaly zero
        std::array<float, 3> normal = {0.0, 0.0, 0.0};
        uint8_t projection = 0;
            if(cs.fvi[i].ver.size() > 3) {
                normal = { 0.0, 0.0 , 0.0}; 
                float x, y, z, coordy, coordx, coordz;
                int f = 1;
                while(normal == std::array<float, 3>{0.0f, 0.0f, 0.0f}) { //calculatin the normal of the face based on it's vertices, if the normal is 0, then the face is degenerate and we will try to calculate the normal with different vertices until we get a non degenerate normal or we run out of vertices to try
                    if(f+1 == cs.fvi[i].ver.size()) {
                        std::cout << "INVALID FACE" << std::endl;
                        break;  
                    } else {
                    coordx = (cs.vyc[cs.fvi[i].ver[f].v-1]-cs.vyc[cs.fvi[i].ver[0].v-1])*(cs.vzc[cs.fvi[i].ver[1+f].v-1]-cs.vzc[cs.fvi[i].ver[0].v-1])-(cs.vyc[cs.fvi[i].ver[1+f].v-1]-cs.vyc[cs.fvi[i].ver[0].v-1])*(cs.vzc[cs.fvi[i].ver[0+f].v-1]-cs.vzc[cs.fvi[i].ver[0].v-1]); 
                    coordy = (cs.vzc[cs.fvi[i].ver[f].v-1]-cs.vzc[cs.fvi[i].ver[0].v-1])*(cs.vxc[cs.fvi[i].ver[1+f].v-1]-cs.vxc[cs.fvi[i].ver[0].v-1])-(cs.vzc[cs.fvi[i].ver[1+f].v-1]-cs.vzc[cs.fvi[i].ver[0].v-1])*(cs.vxc[cs.fvi[i].ver[0+f].v-1]-cs.vxc[cs.fvi[i].ver[0].v-1]);
                    coordz = (cs.vxc[cs.fvi[i].ver[f].v-1]-cs.vxc[cs.fvi[i].ver[0].v-1])*(cs.vyc[cs.fvi[i].ver[1+f].v-1]-cs.vyc[cs.fvi[i].ver[0].v-1])-(cs.vxc[cs.fvi[i].ver[1+f].v-1]-cs.vxc[cs.fvi[i].ver[0].v-1])*(cs.vyc[cs.fvi[i].ver[0+f].v-1]-cs.vyc[cs.fvi[i].ver[0].v-1]);
                    x = limit > std::fabs(coordx) && std::fabs(coordx) >= 0 ? 0.0f : coordx; 
                    y = limit > std::fabs(coordy) && std::fabs(coordy) >= 0 ? 0.0f : coordy;
                    z = limit > std::fabs(coordz) && std::fabs(coordz) >= 0 ? 0.0f : coordz;
                    normal = {
                        x, y, z
                    };
                        f++;
                    } 
                }
                /*std::cout << "Normal" << normal[0] << " Normal0" <<  normal[1] << " Normal1" << normal[2] << "Normal2" << std::endl;*/    
                f = 1;
                float min =std::fabs(*std::min_element(normal.begin(), normal.end())), max = std::fabs(*std::max_element(normal.begin(), normal.end()));//the absolute biggest element defines, which way would the projection distort the least, because of perpendicularity
                //if a normal is more parallel with the, let's say, x, then the projection on the yz plane would be distorted the least => eliminates the risk of wrong detection
                int8_t dor = limit >= max-min && max-min >= 0 ? 0 : (max-min > 0 ? 1 : -1);
                projection = dor == 0 ? std::distance(normal.begin(), std::max_element(normal.begin(), normal.end())) : dor > 0 ? std::distance(normal.begin(), std::max_element(normal.begin(), normal.end())) : std::distance(normal.begin(), std::min_element(normal.begin(), normal.end()));// if the absolutes of the min and max are equal, then it's just easier to choose max
                /*std::cout << "projection" << (int)projection << std::endl;*/
                std::list<uint32_t> vindices;
                std::list<uint32_t>::reverse_iterator relement;
                std::list<uint32_t>::iterator element;
                uint32_t v0, v1, v2;
                std::unordered_set<uint32_t> reflex;
                
                switch(projection) {
                    case 0:
                    std::list<uint32_t>::iterator back;
                    if(x > 0) { //we are ordering here based on winding. The normal tells us, which way are the vertices ordered
                        for(size_t j = 0; j < cs.fvi[i].ver.size(); j++) {
                            if(j<2){
                                vindices.push_back(j);
                            } else if(j >=2  && j < cs.fvi[i].ver.size()-1) {
                                vindices.push_back(j);
                                v2 = cs.fvi[i].ver[j-2].v-1;
                                v1 = cs.fvi[i].ver[j-1].v-1;
                                v0 = cs.fvi[i].ver[j].v - 1;
                                std::array<float, 2>rv1 = {cs.vyc[v1] - cs.vyc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vyc[v0] - cs.vyc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(j-1);
                                }
                            } else {
                                vindices.push_back(j);                                
                                v2 = cs.fvi[i].ver[j-2].v-1;
                                v1 = cs.fvi[i].ver[j-1].v-1;
                                v0 = cs.fvi[i].ver[j].v - 1;
                                std::array<float, 2>rv1 = {cs.vyc[v1] - cs.vyc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vyc[v0] - cs.vyc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(j-1);
                                }//until here, the cycle is same as above
                                v2 = v1;
                                v1 = v0;
                                v0 = cs.fvi[i].ver[0].v-1;
                                std::array<float, 2>rv1 = {cs.vyc[v1] - cs.vyc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vyc[v0] - cs.vyc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(j);
                                }
                                v2 = v1;
                                v1 = v0;
                                v0 = cs.fvi[i].ver[1].v - 1;
                                std::array<float, 2>rv1 = {cs.vyc[v1] - cs.vyc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vyc[v0] - cs.vyc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(0);
                                }
                            }
                        }
                    } else {
                        for(int j = 0; j < cs.fvi[i].ver.size(); j++) {
                            if(j<2){
                                vindices.push_back(cs.fvi[i].ver.size()-1-j);
                            } else if(j >=2 && j < cs.fvi[i].ver.size()-1) {
                                vindices.push_back(cs.fvi[i].ver.size()-1-j);
                                v2 = cs.fvi[i].ver[cs.fvi[i].ver.size()+1-j].v-1;
                                v1 = cs.fvi[i].ver[cs.fvi[i].ver.size()-j].v-1;
                                v0 = cs.fvi[i].ver[cs.fvi[i].ver.size()-1-j].v - 1;
                                std::array<float, 2>rv1 = {cs.vyc[v1] - cs.vyc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vyc[v0] - cs.vyc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(cs.fvi[i].ver.size()-j);
                                }
                            } else {
                                vindices.push_back(cs.fvi[i].ver.size()-1-j);
                                v2 = cs.fvi[i].ver[cs.fvi[i].ver.size()+1-j].v-1;
                                v1 = cs.fvi[i].ver[cs.fvi[i].ver.size()-j].v-1;
                                v0 = cs.fvi[i].ver[cs.fvi[i].ver.size()-1-j].v - 1;
                                std::array<float, 2>rv1 = {cs.vyc[v1] - cs.vyc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vyc[v0] - cs.vyc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(cs.fvi[i].ver.size()-j);
                                }//until here, the cycle is same as above
                                v2 = v1;
                                v1 = v0;
                                v0 = cs.fvi[i].ver[*(vindices.begin())].v-1;
                                std::array<float, 2>rv1 = {cs.vyc[v1] - cs.vyc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vyc[v0] - cs.vyc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(0);
                                }
                                v2 = v1;
                                v1 = v0;
                                v0 = cs.fvi[i].ver[*(std::next(vindices.begin(),1))].v - 1;
                                std::array<float, 2>rv1 = {cs.vyc[v1] - cs.vyc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vyc[v0] - cs.vyc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(cs.fvi[i].ver.size()-1);
                                }
                        }
                        std::list<uint32_t>::iterator step = {std::next(vindices.begin(), 1)};
                        uint32_t reflexelement {0}; 
                        uint32_t switcher;
                        bool valid;
                        std::list<uint32_t>::iterator firstelement {vindices.begin()};
                        std::list<uint32_t>::iterator nextnext {std::next(vindices.begin(), 1)};
                        std::list<uint32_t>::iterator prev1 {std::prev(vindices.end(), 1)};
                        std::list<uint32_t>::iterator prev2 {std::prev(vindices.end(), 2)};
                        while(vindices.size() >= 3) {
                            if(reflex.empty() == 1) {
                                cs.triangles.emplace_back(std::array<long unsigned int, 3>{cs.fvi[i].ver[vindices.begin()].v-1, cs.fvi[i].ver[*step].v-1, cs.fvi[i].ver[*(std::next(step, 1))].v-1});
                                step = vindices.erase(step);
                                            //std::cout<< "face valid" << std::endl;
                                            //std::cout k++;<< "I have removed: " << v1+1 << std::endl;
                                            //std::cout << "Triangle "<<cs.triangles[cs.triangles.size()-1][0] << " " << cs.triangles[cs.triangles.size()-1][1] << " "<< cs.triangles[cs.triangles.size()-1][2]<< std::endl;
                            } else {
                                if(reflex.find(*firstelement) != reflex.end()) {
                                    firstelement++;
                                    nextnext++;
                                    prev1++;
                                    prev2++;
                                    nextnext = nextnext == vindices.end() ? vindices.begin() : nextnext;
                                    prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
                                    prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
                                    firstelement = firstelement == vindices.end() ? vindices.begin() : firstelement;
                                } else {
                                    v0 = cs.fvi[i].ver[*prev1].v-1;
                                    reflexelement = reflex.find(*prev1) == reflex.end() ? 0 : (1 && (switcher =1));
                                    v1 =cs.fvi[i].ver[*nextnext].v-1;
                                    reflexelement = reflex.find(*nextnext) == reflex.end() ? (reflexelement && (switcher =1)) : reflexelement == 0? (reflexelement && (switcher = 2)) : reflexelement +=1;
                                    v2 =cs.fvi[i].ver[std::next(nextnext) == vindices.end() ? *vindices.begin() : *nextnext].v-1;
                                    
                                    if(reflexelement ==  reflex.size()) {
                                        cs.triangles.emplace_back(std::array<long unsigned int, 3>{v0, cs.fvi[i].ver[*firstelement].v-1, v1});
                                        firstelement = vindices.erase(firstelement);
                                        nextnext++;
                                        switch(switcher) {
                                        case 1:
                                        std::array<float, 2>rv1 = {cs.vyc[v0] - cs.vyc[cs.fvi[i].ver[*prev2].v-1],cs.vzc[v0] - cs.vzc[cs.fvi[i].ver[*prev2].v-1]};
                                        std::array<float, 2>rv2 = {cs.vyc[v1] - cs.vyc[cs.fvi[i].ver[*prev2].v-1],cs.vzc[v1] - cs.vzc[cs.fvi[i].ver[*prev2].v-1]};
                                        if(rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                            reflex.erase(*prev1);
                                        }
                                        break;
                                        case 2:
                                        std::array<float, 2>rv1 = {cs.vyc[v1] - cs.vyc[v0],cs.vzc[v1] - cs.vzc[v0]};
                                        std::array<float, 2>rv2 = {cs.vyc[v2] - cs.vyc[v0],cs.vzc[v2] - cs.vzc[v0]};
                                        if(rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                            reflex.erase(*firstelement);
                                        
                                        break;
                                        }
                                        }   
                                    } else if(reflexelement > 0) {      
                                        valid = false;            
                                        for(size_t j = 0; j < reflex.size(); j++) { 
                                            std::array<float, 2> p = {cs.vyc[cs.fvi[i].ver[*(std::next(reflex.begin(), j))].v-1], cs.vzc[cs.fvi[i].ver[*(std::next(reflex.begin(), j))].v-1]};//the vertex of the polygon
                                            std::array<float, 2> tp1 = {cs.vyc[v0], cs.vzc[v0]};
                                            std::array<float, 2> tp2 = {cs.vyc[v1], cs.vzc[v1]};
                                            std::array<float, 2> tp3 = {cs.vyc[v2], cs.vzc[v2]};
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
                                            BCP.product = BCP.ad - BCP.bc;//we are calculating the normals of the triangle's sides with an another polygon vertex(obv. we are taking each side and the current vertex to it),
                                            // we are taking the normal relative to a vertex of the potential triangle , and rotate through the triangle in ccw
                                            valid = (ABP.product > limit) && (ACP.product > limit) && (BCP.product > limit);
                                            if(valid == true) {// we exclude go step 1 forward in potential triangle vertices, because the last 1 was invalid
                                                //std::cout<< "face invalid" << std::endl;
                                                firstelement++;
                                                prev1++;
                                                prev2++;
                                                prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
                                                prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
                                                firstelement = firstelement == vindices.end() ? vindices.begin() : firstelement;
                                                break;
                                            }
                                        }
                                        if(valid == false) {
                                            cs.triangles.emplace_back(std::array<long unsigned int, 3>{v0, cs.fvi[i].ver[*firstelement].v-1, v1});                                        firstelement = vindices.erase(firstelement);

                                            switch(switcher) {
                                                case 1:
                                                std::array<float, 2>rv1 = {cs.vyc[v0] - cs.vyc[cs.fvi[i].ver[*prev2].v-1],cs.vzc[v0] - cs.vzc[cs.fvi[i].ver[*prev2].v-1]};
                                                std::array<float, 2>rv2 = {cs.vyc[v1] - cs.vyc[cs.fvi[i].ver[*prev2].v-1],cs.vzc[v1] - cs.vzc[cs.fvi[i].ver[*prev2].v-1]};
                                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                                    reflex.erase(*prev1);
                                                }
                                                break;
                                                case 2:
                                                std::array<float, 2>rv1 = {cs.vyc[v1] - cs.vyc[v0],cs.vzc[v1] - cs.vzc[v0]};
                                                std::array<float, 2>rv2 = {cs.vyc[v2] - cs.vyc[v0],cs.vzc[v2] - cs.vzc[v0]};
                                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                                    reflex.erase(*firstelement);
                                                
                                                break;
                                                }
                                            }   
                                        }
                                    } else {
                                        valid = false;
                                        for(size_t j = 0; j< reflex.size(); j++) { 
                                            std::array<float, 2> p = {cs.vyc[cs.fvi[i].ver[*(std::next(reflex.begin(), j))].v-1], cs.vzc[cs.fvi[i].ver[*(std::next(reflex.begin(), j))].v-1]};//the vertex of the polygon
                                            std::array<float, 2> tp1 = {cs.vyc[v0], cs.vzc[v0]};
                                            std::array<float, 2> tp2 = {cs.vyc[v1], cs.vzc[v1]};
                                            std::array<float, 2> tp3 = {cs.vyc[v2], cs.vzc[v2]};
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
                                            BCP.product = BCP.ad - BCP.bc;//we are calculating the normals of the triangle's sides with an another polygon vertex(obv. we are taking each side and the current vertex to it),
                                            // we are taking the normal relative to a vertex of the potential triangle , and rotate through the triangle in ccw
                                            valid = (ABP.product > limit) && (ACP.product > limit) && (BCP.product > limit);
                                            if(valid == true) {// we exclude go step 1 forward in potential triangle vertices, because the last 1 was invalid
                                                //std::cout<< "face invalid" << std::endl;
                                                firstelement++;
                                                prev1++;
                                                prev2++;
                                                prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
                                                prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
                                                firstelement = firstelement == vindices.end() ? vindices.begin() : firstelement;
                                                break;
                                            }
                                            }
                                            if (valid == false) {
                                                cs.triangles.emplace_back(std::array<long unsigned int, 3>{v0, cs.fvi[i].ver[*firstelement].v-1, v1});
                                                firstelement = vindices.erase(firstelement);
                                                firstelement++;
                                                prev1++;
                                                prev2++;
                                                prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
                                                prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
                                                firstelement = firstelement == vindices.end() ? vindices.begin() : firstelement;
                                            }
                                            continue;
                                    } 
                                }
                        }
                    break;
                case 1:
                    std::list<uint32_t>::iterator back;
                    if(x > 0) { //we are ordering here based on winding. The normal tells us, which way are the vertices ordered
                        for(size_t j = 0; j < cs.fvi[i].ver.size(); j++) {
                            if(j<2){
                                vindices.push_back(j);
                            } else if(j >=2  && j < cs.fvi[i].ver.size()-1) {
                                vindices.push_back(j);
                                v2 = cs.fvi[i].ver[j-2].v-1;
                                v1 = cs.fvi[i].ver[j-1].v-1;
                                v0 = cs.fvi[i].ver[j].v - 1;
                                std::array<float, 2>rv1 = {cs.vxc[v1] - cs.vxc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vxc[v0] - cs.vxc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(j-1);
                                }
                            } else {
                                vindices.push_back(j);                                
                                v2 = cs.fvi[i].ver[j-2].v-1;
                                v1 = cs.fvi[i].ver[j-1].v-1;
                                v0 = cs.fvi[i].ver[j].v - 1;
                                std::array<float, 2>rv1 = {cs.vxc[v1] - cs.vxc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vxc[v0] - cs.vxc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(j-1);
                                }//until here, the cycle is same as above
                                v2 = v1;
                                v1 = v0;
                                v0 = cs.fvi[i].ver[0].v-1;
                                std::array<float, 2>rv1 = {cs.vxc[v1] - cs.vxc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vxc[v0] - cs.vxc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(j);
                                }
                                v2 = v1;
                                v1 = v0;
                                v0 = cs.fvi[i].ver[1].v - 1;
                                std::array<float, 2>rv1 = {cs.vxc[v1] - cs.vxc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vxc[v0] - cs.vxc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(0);
                                }
                            }
                        }
                    } else {
                        for(int j = 0; j < cs.fvi[i].ver.size(); j++) {
                            if(j<2){
                                vindices.push_back(cs.fvi[i].ver.size()-1-j);
                            } else if(j >=2 && j < cs.fvi[i].ver.size()-1) {
                                vindices.push_back(cs.fvi[i].ver.size()-1-j);
                                v2 = cs.fvi[i].ver[cs.fvi[i].ver.size()+1-j].v-1;
                                v1 = cs.fvi[i].ver[cs.fvi[i].ver.size()-j].v-1;
                                v0 = cs.fvi[i].ver[cs.fvi[i].ver.size()-1-j].v - 1;
                                std::array<float, 2>rv1 = {cs.vxc[v1] - cs.vxc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vxc[v0] - cs.vxc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(cs.fvi[i].ver.size()-j);
                                }
                            } else {
                                vindices.push_back(cs.fvi[i].ver.size()-1-j);
                                v2 = cs.fvi[i].ver[cs.fvi[i].ver.size()+1-j].v-1;
                                v1 = cs.fvi[i].ver[cs.fvi[i].ver.size()-j].v-1;
                                v0 = cs.fvi[i].ver[cs.fvi[i].ver.size()-1-j].v - 1;
                                std::array<float, 2>rv1 = {cs.vxc[v1] - cs.vxc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vxc[v0] - cs.vxc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(cs.fvi[i].ver.size()-j);
                                }//until here, the cycle is same as above
                                v2 = v1;
                                v1 = v0;
                                v0 = cs.fvi[i].ver[*(vindices.begin())].v-1;
                                std::array<float, 2>rv1 = {cs.vxc[v1] - cs.vxc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vxc[v0] - cs.vxc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(0);
                                }
                                v2 = v1;
                                v1 = v0;
                                v0 = cs.fvi[i].ver[*(std::next(vindices.begin(),1))].v - 1;
                                std::array<float, 2>rv1 = {cs.vxc[v1] - cs.vxc[v2],cs.vzc[v1] - cs.vzc[v2]};
                                std::array<float, 2>rv2 = {cs.vxc[v0] - cs.vxc[v2],cs.vzc[v0] - cs.vzc[v2]};
                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0) {
                                    reflex.insert(cs.fvi[i].ver.size()-1);
                                }
                        }
                        std::list<uint32_t>::iterator step = {std::next(vindices.begin(), 1)};
                        uint32_t reflexelement {0}; 
                        uint32_t switcher;
                        bool valid;
                        std::list<uint32_t>::iterator firstelement {vindices.begin()};
                        std::list<uint32_t>::iterator nextnext {std::next(vindices.begin(), 1)};
                        std::list<uint32_t>::iterator prev1 {std::prev(vindices.end(), 1)};
                        std::list<uint32_t>::iterator prev2 {std::prev(vindices.end(), 2)};
                        while(vindices.size() >= 3) {
                            if(reflex.empty() == 1) {
                                cs.triangles.emplace_back(std::array<long unsigned int, 3>{cs.fvi[i].ver[vindices.begin()].v-1, cs.fvi[i].ver[*step].v-1, cs.fvi[i].ver[*(std::next(step, 1))].v-1});
                                step = vindices.erase(step);
                                            //std::cout<< "face valid" << std::endl;
                                            //std::cout k++;<< "I have removed: " << v1+1 << std::endl;
                                            //std::cout << "Triangle "<<cs.triangles[cs.triangles.size()-1][0] << " " << cs.triangles[cs.triangles.size()-1][1] << " "<< cs.triangles[cs.triangles.size()-1][2]<< std::endl;
                            } else {
                                if(reflex.find(*firstelement) != reflex.end()) {
                                    firstelement++;
                                    nextnext++;
                                    prev1++;
                                    prev2++;
                                    nextnext = nextnext == vindices.end() ? vindices.begin() : nextnext;
                                    prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
                                    prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
                                    firstelement = firstelement == vindices.end() ? vindices.begin() : firstelement;
                                } else {
                                    v0 = cs.fvi[i].ver[*prev1].v-1;
                                    reflexelement = reflex.find(*prev1) == reflex.end() ? 0 : (1 && (switcher =1));
                                    v1 =cs.fvi[i].ver[*nextnext].v-1;
                                    reflexelement = reflex.find(*nextnext) == reflex.end() ? (reflexelement && (switcher =1)) : reflexelement == 0? (reflexelement && (switcher = 2)) : reflexelement +=1;
                                    v2 =cs.fvi[i].ver[std::next(nextnext) == vindices.end() ? *vindices.begin() : *nextnext].v-1;
                                    
                                    if(reflexelement ==  reflex.size()) {
                                        cs.triangles.emplace_back(std::array<long unsigned int, 3>{v0, cs.fvi[i].ver[*firstelement].v-1, v1});
                                        firstelement = vindices.erase(firstelement);
                                        nextnext++;
                                        switch(switcher) {
                                        case 1:
                                        std::array<float, 2>rv1 = {cs.vxc[v0] - cs.vxc[cs.fvi[i].ver[*prev2].v-1],cs.vzc[v0] - cs.vzc[cs.fvi[i].ver[*prev2].v-1]};
                                        std::array<float, 2>rv2 = {cs.vxc[v1] - cs.vxc[cs.fvi[i].ver[*prev2].v-1],cs.vzc[v1] - cs.vzc[cs.fvi[i].ver[*prev2].v-1]};
                                        if(rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                            reflex.erase(*prev1);
                                        }
                                        break;
                                        case 2:
                                        std::array<float, 2>rv1 = {cs.vxc[v1] - cs.vxc[v0],cs.vzc[v1] - cs.vzc[v0]};
                                        std::array<float, 2>rv2 = {cs.vxc[v2] - cs.vxc[v0],cs.vzc[v2] - cs.vzc[v0]};
                                        if(rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                            reflex.erase(*firstelement);
                                        
                                        break;
                                        }
                                        }   
                                    } else if(reflexelement > 0) {      
                                        valid = false;            
                                        for(size_t j = 0; j < reflex.size(); j++) { 
                                            std::array<float, 2> p = {cs.vxc[cs.fvi[i].ver[*(std::next(reflex.begin(), j))].v-1], cs.vzc[cs.fvi[i].ver[*(std::next(reflex.begin(), j))].v-1]};//the vertex of the polygon
                                            std::array<float, 2> tp1 = {cs.vxc[v0], cs.vzc[v0]};
                                            std::array<float, 2> tp2 = {cs.vxc[v1], cs.vzc[v1]};
                                            std::array<float, 2> tp3 = {cs.vxc[v2], cs.vzc[v2]};
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
                                            BCP.product = BCP.ad - BCP.bc;//we are calculating the normals of the triangle's sides with an another polygon vertex(obv. we are taking each side and the current vertex to it),
                                            // we are taking the normal relative to a vertex of the potential triangle , and rotate through the triangle in ccw
                                            valid = (ABP.product > limit) && (ACP.product > limit) && (BCP.product > limit);
                                            if(valid == true) {// we exclude go step 1 forward in potential triangle vertices, because the last 1 was invalid
                                                //std::cout<< "face invalid" << std::endl;
                                                firstelement++;
                                                prev1++;
                                                prev2++;
                                                prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
                                                prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
                                                firstelement = firstelement == vindices.end() ? vindices.begin() : firstelement;
                                                break;
                                            }
                                        }
                                        if(valid == false) {
                                            cs.triangles.emplace_back(std::array<long unsigned int, 3>{v0, cs.fvi[i].ver[*firstelement].v-1, v1});                                        firstelement = vindices.erase(firstelement);

                                            switch(switcher) {
                                                case 1:
                                                std::array<float, 2>rv1 = {cs.vxc[v0] - cs.vxc[cs.fvi[i].ver[*prev2].v-1],cs.vzc[v0] - cs.vzc[cs.fvi[i].ver[*prev2].v-1]};
                                                std::array<float, 2>rv2 = {cs.vxc[v1] - cs.vxc[cs.fvi[i].ver[*prev2].v-1],cs.vzc[v1] - cs.vzc[cs.fvi[i].ver[*prev2].v-1]};
                                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                                    reflex.erase(*prev1);
                                                }
                                                break;
                                                case 2:
                                                std::array<float, 2>rv1 = {cs.vxc[v1] - cs.vxc[v0],cs.vzc[v1] - cs.vzc[v0]};
                                                std::array<float, 2>rv2 = {cs.vxc[v2] - cs.vxc[v0],cs.vzc[v2] - cs.vzc[v0]};
                                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                                    reflex.erase(*firstelement);
                                                
                                                break;
                                                }
                                            }   
                                        }
                                    } else {
                                        valid = false;
                                        for(size_t j = 0; j< reflex.size(); j++) { 
                                            std::array<float, 2> p = {cs.vxc[cs.fvi[i].ver[*(std::next(reflex.begin(), j))].v-1], cs.vzc[cs.fvi[i].ver[*(std::next(reflex.begin(), j))].v-1]};//the vertex of the polygon
                                            std::array<float, 2> tp1 = {cs.vxc[v0], cs.vzc[v0]};
                                            std::array<float, 2> tp2 = {cs.vxc[v1], cs.vzc[v1]};
                                            std::array<float, 2> tp3 = {cs.vxc[v2], cs.vzc[v2]};
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
                                            BCP.product = BCP.ad - BCP.bc;//we are calculating the normals of the triangle's sides with an another polygon vertex(obv. we are taking each side and the current vertex to it),
                                            // we are taking the normal relative to a vertex of the potential triangle , and rotate through the triangle in ccw
                                            valid = (ABP.product > limit) && (ACP.product > limit) && (BCP.product > limit);
                                            if(valid == true) {// we exclude go step 1 forward in potential triangle vertices, because the last 1 was invalid
                                                //std::cout<< "face invalid" << std::endl;
                                                firstelement++;
                                                prev1++;
                                                prev2++;
                                                prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
                                                prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
                                                firstelement = firstelement == vindices.end() ? vindices.begin() : firstelement;
                                                break;
                                            }
                                            }
                                            if (valid == false) {
                                                cs.triangles.emplace_back(std::array<long unsigned int, 3>{v0, cs.fvi[i].ver[*firstelement].v-1, v1});
                                                firstelement = vindices.erase(firstelement);
                                                firstelement++;
                                                prev1++;
                                                prev2++;
                                                prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
                                                prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
                                                firstelement = firstelement == vindices.end() ? vindices.begin() : firstelement;
                                            }
                                            continue;
                                            }
                                        }
                                    } 
                                }
                            }
                        }
                    }
                    break;
                        case 2:
                            std::list<uint32_t>::iterator back;
                            if (x > 0)
                            { // we are ordering here based on winding. The normal tells us, which way are the vertices ordered
                                for (size_t j = 0; j < cs.fvi[i].ver.size(); j++)
                                {
                                    if (j < 2)
                                    {
                                        vindices.push_back(j);
                                    }
                                    else if (j >= 2 && j < cs.fvi[i].ver.size() - 1)
                                    {
                                        vindices.push_back(j);
                                        v2 = cs.fvi[i].ver[j - 2].v - 1;
                                        v1 = cs.fvi[i].ver[j - 1].v - 1;
                                        v0 = cs.fvi[i].ver[j].v - 1;
                                        std::array<float, 2> rv1 = {cs.vxc[v1] - cs.vxc[v2], cs.vyc[v1] - cs.vyc[v2]};
                                        std::array<float, 2> rv2 = {cs.vxc[v0] - cs.vxc[v2], cs.vyc[v0] - cs.vyc[v2]};
                                        if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
                                        {
                                            reflex.insert(j - 1);
                                        }
                                    }
                                    else
                                    {
                                        vindices.push_back(j);
                                        v2 = cs.fvi[i].ver[j - 2].v - 1;
                                        v1 = cs.fvi[i].ver[j - 1].v - 1;
                                        v0 = cs.fvi[i].ver[j].v - 1;
                                        std::array<float, 2> rv1 = {cs.vxc[v1] - cs.vxc[v2], cs.vyc[v1] - cs.vyc[v2]};
                                        std::array<float, 2> rv2 = {cs.vxc[v0] - cs.vxc[v2], cs.vyc[v0] - cs.vyc[v2]};
                                        if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
                                        {
                                            reflex.insert(j - 1);
                                        } // until here, the cycle is same as above
                                        v2 = v1;
                                        v1 = v0;
                                        v0 = cs.fvi[i].ver[0].v - 1;
                                        std::array<float, 2> rv1 = {cs.vxc[v1] - cs.vxc[v2], cs.vyc[v1] - cs.vyc[v2]};
                                        std::array<float, 2> rv2 = {cs.vxc[v0] - cs.vxc[v2], cs.vyc[v0] - cs.vyc[v2]};
                                        if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
                                        {
                                            reflex.insert(j);
                                        }
                                        v2 = v1;
                                        v1 = v0;
                                        v0 = cs.fvi[i].ver[1].v - 1;
                                        std::array<float, 2> rv1 = {cs.vxc[v1] - cs.vxc[v2], cs.vyc[v1] - cs.vyc[v2]};
                                        std::array<float, 2> rv2 = {cs.vxc[v0] - cs.vxc[v2], cs.vyc[v0] - cs.vyc[v2]};
                                        if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
                                        {
                                            reflex.insert(0);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                for (int j = 0; j < cs.fvi[i].ver.size(); j++)
                                {
                                    if (j < 2)
                                    {
                                        vindices.push_back(cs.fvi[i].ver.size() - 1 - j);
                                    }
                                    else if (j >= 2 && j < cs.fvi[i].ver.size() - 1)
                                    {
                                        vindices.push_back(cs.fvi[i].ver.size() - 1 - j);
                                        v2 = cs.fvi[i].ver[cs.fvi[i].ver.size() + 1 - j].v - 1;
                                        v1 = cs.fvi[i].ver[cs.fvi[i].ver.size() - j].v - 1;
                                        v0 = cs.fvi[i].ver[cs.fvi[i].ver.size() - 1 - j].v - 1;
                                        std::array<float, 2> rv1 = {cs.vxc[v1] - cs.vxc[v2], cs.vyc[v1] - cs.vyc[v2]};
                                        std::array<float, 2> rv2 = {cs.vxc[v0] - cs.vxc[v2], cs.vyc[v0] - cs.vyc[v2]};
                                        if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
                                        {
                                            reflex.insert(cs.fvi[i].ver.size() - j);
                                        }
                                    }
                                    else
                                    {
                                        vindices.push_back(cs.fvi[i].ver.size() - 1 - j);
                                        v2 = cs.fvi[i].ver[cs.fvi[i].ver.size() + 1 - j].v - 1;
                                        v1 = cs.fvi[i].ver[cs.fvi[i].ver.size() - j].v - 1;
                                        v0 = cs.fvi[i].ver[cs.fvi[i].ver.size() - 1 - j].v - 1;
                                        std::array<float, 2> rv1 = {cs.vxc[v1] - cs.vxc[v2], cs.vyc[v1] - cs.vyc[v2]};
                                        std::array<float, 2> rv2 = {cs.vxc[v0] - cs.vxc[v2], cs.vyc[v0] - cs.vyc[v2]};
                                        if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
                                        {
                                            reflex.insert(cs.fvi[i].ver.size() - j);
                                        } // until here, the cycle is same as above
                                        v2 = v1;
                                        v1 = v0;
                                        v0 = cs.fvi[i].ver[*(vindices.begin())].v - 1;
                                        std::array<float, 2> rv1 = {cs.vxc[v1] - cs.vxc[v2], cs.vyc[v1] - cs.vyc[v2]};
                                        std::array<float, 2> rv2 = {cs.vxc[v0] - cs.vxc[v2], cs.vyc[v0] - cs.vyc[v2]};
                                        if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
                                        {
                                            reflex.insert(0);
                                        }
                                        v2 = v1;
                                        v1 = v0;
                                        v0 = cs.fvi[i].ver[*(std::next(vindices.begin(), 1))].v - 1;
                                        std::array<float, 2> rv1 = {cs.vxc[v1] - cs.vxc[v2], cs.vyc[v1] - cs.vyc[v2]};
                                        std::array<float, 2> rv2 = {cs.vxc[v0] - cs.vxc[v2], cs.vyc[v0] - cs.vyc[v2]};
                                        if (rv1[0] * rv2[1] - rv1[1] * rv2[0] <= 0)
                                        {
                                            reflex.insert(cs.fvi[i].ver.size() - 1);
                                        }
                                    }
                                    std::list<uint32_t>::iterator step = {std::next(vindices.begin(), 1)};
                                    uint32_t reflexelement{0};
                                    uint32_t switcher;
                                    bool valid;
                                    std::list<uint32_t>::iterator firstelement{vindices.begin()};
                                    std::list<uint32_t>::iterator nextnext{std::next(vindices.begin(), 1)};
                                    std::list<uint32_t>::iterator prev1{std::prev(vindices.end(), 1)};
                                    std::list<uint32_t>::iterator prev2{std::prev(vindices.end(), 2)};
                                    while (vindices.size() >= 3)
                                    {
                                        if (reflex.empty() == 1)
                                        {
                                            cs.triangles.emplace_back(std::array<long unsigned int, 3>{cs.fvi[i].ver[vindices.begin()].v - 1, cs.fvi[i].ver[*step].v - 1, cs.fvi[i].ver[*(std::next(step, 1))].v - 1});
                                            step = vindices.erase(step);
                                            //std::cout<< "face valid" << std::endl;
                                            //std::cout k++;<< "I have removed: " << v1+1 << std::endl;
                                            //std::cout << "Triangle "<<cs.triangles[cs.triangles.size()-1][0] << " " << cs.triangles[cs.triangles.size()-1][1] << " "<< cs.triangles[cs.triangles.size()-1][2]<< std::endl;
                            } else {
                                if(reflex.find(*firstelement) != reflex.end()) {
                                    firstelement++;
                                    nextnext++;
                                    prev1++;
                                    prev2++;
                                    nextnext = nextnext == vindices.end() ? vindices.begin() : nextnext;
                                    prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
                                    prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
                                    firstelement = firstelement == vindices.end() ? vindices.begin() : firstelement;
                                } else {
                                    v0 = cs.fvi[i].ver[*prev1].v-1;
                                    reflexelement = reflex.find(*prev1) == reflex.end() ? 0 : (1 && (switcher =1));
                                    v1 =cs.fvi[i].ver[*nextnext].v-1;
                                    reflexelement = reflex.find(*nextnext) == reflex.end() ? (reflexelement && (switcher =1)) : reflexelement == 0? (reflexelement && (switcher = 2)) : reflexelement +=1;
                                    v2 =cs.fvi[i].ver[std::next(nextnext) == vindices.end() ? *vindices.begin() : *nextnext].v-1;
                                    
                                    if(reflexelement ==  reflex.size()) {
                                        cs.triangles.emplace_back(std::array<long unsigned int, 3>{v0, cs.fvi[i].ver[*firstelement].v-1, v1});
                                        firstelement = vindices.erase(firstelement);
                                        nextnext++;
                                        switch(switcher) {
                                        case 1:
                                        std::array<float, 2>rv1 = {cs.vxc[v0] - cs.vxc[cs.fvi[i].ver[*prev2].v-1],cs.vyc[v0] - cs.vyc[cs.fvi[i].ver[*prev2].v-1]};
                                        std::array<float, 2>rv2 = {cs.vxc[v1] - cs.vxc[cs.fvi[i].ver[*prev2].v-1],cs.vyc[v1] - cs.vyc[cs.fvi[i].ver[*prev2].v-1]};
                                        if(rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                            reflex.erase(*prev1);
                                        }
                                        break;
                                        case 2:
                                        std::array<float, 2>rv1 = {cs.vxc[v1] - cs.vxc[v0],cs.vyc[v1] - cs.vyc[v0]};
                                        std::array<float, 2>rv2 = {cs.vxc[v2] - cs.vxc[v0],cs.vyc[v2] - cs.vyc[v0]};
                                        if(rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                            reflex.erase(*firstelement);
                                        
                                        break;
                                        }
                                        }   
                                    } else if(reflexelement > 0) {      
                                        valid = false;            
                                        for(size_t j = 0; j < reflex.size(); j++) { 
                                            std::array<float, 2> p = {cs.vxc[cs.fvi[i].ver[*(std::next(reflex.begin(), j))].v-1], cs.vyc[cs.fvi[i].ver[*(std::next(reflex.begin(), j))].v-1]};//the vertex of the polygon
                                            std::array<float, 2> tp1 = {cs.vxc[v0], cs.vyc[v0]};
                                            std::array<float, 2> tp2 = {cs.vxc[v1], cs.vyc[v1]};
                                            std::array<float, 2> tp3 = {cs.vxc[v2], cs.vyc[v2]};
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
                                            BCP.product = BCP.ad - BCP.bc;//we are calculating the normals of the triangle's sides with an another polygon vertex(obv. we are taking each side and the current vertex to it),
                                            // we are taking the normal relative to a vertex of the potential triangle , and rotate through the triangle in ccw
                                            valid = (ABP.product > limit) && (ACP.product > limit) && (BCP.product > limit);
                                            if(valid == true) {// we exclude go step 1 forward in potential triangle vertices, because the last 1 was invalid
                                                //std::cout<< "face invalid" << std::endl;
                                                firstelement++;
                                                prev1++;
                                                prev2++;
                                                prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
                                                prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
                                                firstelement = firstelement == vindices.end() ? vindices.begin() : firstelement;
                                                break;
                                            }
                                        }
                                        if(valid == false) {
                                            cs.triangles.emplace_back(std::array<long unsigned int, 3>{v0, cs.fvi[i].ver[*firstelement].v-1, v1});                                        firstelement = vindices.erase(firstelement);

                                            switch(switcher) {
                                                case 1:
                                                std::array<float, 2>rv1 = {cs.vxc[v0] - cs.vxc[cs.fvi[i].ver[*prev2].v-1],cs.vyc[v0] - cs.vyc[cs.fvi[i].ver[*prev2].v-1]};
                                                std::array<float, 2>rv2 = {cs.vxc[v1] - cs.vxc[cs.fvi[i].ver[*prev2].v-1],cs.vyc[v1] - cs.vyc[cs.fvi[i].ver[*prev2].v-1]};
                                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                                    reflex.erase(*prev1);
                                                }
                                                break;
                                                case 2:
                                                std::array<float, 2>rv1 = {cs.vxc[v1] - cs.vxc[v0],cs.vyc[v1] - cs.vyc[v0]};
                                                std::array<float, 2>rv2 = {cs.vxc[v2] - cs.vxc[v0],cs.vyc[v2] - cs.vyc[v0]};
                                                if(rv1[0] * rv2[1] - rv1[1] * rv2[0] >= 0) {
                                                    reflex.erase(*firstelement);
                                                
                                                break;
                                                }
                                            }   
                                        }
                                    } else {
                                        valid = false;
                                        for(size_t j = 0; j< reflex.size(); j++) { 
                                            std::array<float, 2> p = {cs.vxc[cs.fvi[i].ver[*(std::next(reflex.begin(), j))].v-1], cs.vyc[cs.fvi[i].ver[*(std::next(reflex.begin(), j))].v-1]};//the vertex of the polygon
                                            std::array<float, 2> tp1 = {cs.vxc[v0], cs.vyc[v0]};
                                            std::array<float, 2> tp2 = {cs.vxc[v1], cs.vyc[v1]};
                                            std::array<float, 2> tp3 = {cs.vxc[v2], cs.vyc[v2]};
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
                                            BCP.product = BCP.ad - BCP.bc;//we are calculating the normals of the triangle's sides with an another polygon vertex(obv. we are taking each side and the current vertex to it),
                                            // we are taking the normal relative to a vertex of the potential triangle , and rotate through the triangle in ccw
                                            valid = (ABP.product > limit) && (ACP.product > limit) && (BCP.product > limit);
                                            if(valid == true) {// we exclude go step 1 forward in potential triangle vertices, because the last 1 was invalid
                                                //std::cout<< "face invalid" << std::endl;
                                                firstelement++;
                                                prev1++;
                                                prev2++;
                                                prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
                                                prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
                                                firstelement = firstelement == vindices.end() ? vindices.begin() : firstelement;
                                                break;
                                            }
                                            }
                                            if (valid == false) {
                                                cs.triangles.emplace_back(std::array<long unsigned int, 3>{v0, cs.fvi[i].ver[*firstelement].v-1, v1});
                                                firstelement = vindices.erase(firstelement);
                                                firstelement++;
                                                prev1++;
                                                prev2++;
                                                prev2 = prev2 == vindices.end() ? vindices.begin() : prev2;
                                                prev1 = prev1 == vindices.end() ? vindices.begin() : prev1;
                                                firstelement = firstelement == vindices.end() ? vindices.begin() : firstelement;
                                            }
                                            continue;
                                            }
                                        }
                                    } 
                                }
                            }
                        }
                    }
                }
            }
        } else {
                cs.triangles.emplace_back(std::array<long unsigned int, 3>{cs.fvi[i].ver[0].v, cs.fvi[i].ver[1].v, cs.fvi[i].ver[2].v}); // we pass on shapes, that are triangles
                continue;
        }
    }
}