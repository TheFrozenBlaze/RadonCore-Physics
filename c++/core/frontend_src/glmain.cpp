#include "3dvfunc.h"
#include "glmain.h"
#include "shader.h"
#include <vector>
#include <algorithm>
#include <glad/glad.h>
#include <cmath>

GLuint VAO {};
GLuint cVBO {};
GLuint pVBO {};
GLuint EBO {};

std::vector<GLuint> GL::VAOvec;
std::vector<GLuint> GL::pVBOvec;
std::vector<GLuint> GL::cVBOvec;
std::vector<GLuint> GL::EBOvec;

uint16_t GL::gWindowHeight, GL::gWindowWidth;
std::array<float,3> GL::gCamera;
std::array<float,9> GL::gCameraDesc;
std::vector<Coord> objIdent::objects;

Shader *GL::shader;
void GL::Compile(const std::vector<std::string> &files)
{
    objIdent obj;
    obj.objects.reserve(obj.objects.size() + files.size());
    
    for (size_t i{}; i < files.size(); i++)
    {
        obj.objReader(files[i]);
        Coord &cs = obj.objects.back();
        std::vector<float> color(cs.vpc.size(), 1.0f);
        /*for(size_t i =0; i< cs.triangles.size(); i++) {
        std::cout << "Triangle: "<< cs.triangles[i][0] << " " << cs.triangles[i][1] << " " << cs.triangles[i][2] << std::endl;
        };
        for (size_t i {}; i <cs.vpc.size(); i++) {
            std::cout << cs.vpc[i] << std::endl;
        }*/
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        GL::VAOvec.emplace_back(VAO);
        glGenBuffers(1, &pVBO);
        glGenBuffers(1, &cVBO);
        glGenBuffers(1, &EBO);
        //ebo
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, cs.triangles.size() *3* sizeof(uint32_t), cs.triangles.data(),GL_STATIC_DRAW);
        GL::EBOvec.emplace_back(EBO);
        // position vbo
        glBindBuffer(GL_ARRAY_BUFFER, pVBO);
        glBufferData(GL_ARRAY_BUFFER, cs.vpc.size() * sizeof(float),cs.vpc.data(),GL_DYNAMIC_DRAW);

        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
        GL::pVBOvec.emplace_back(pVBO);
        glEnableVertexAttribArray(0);
        //color vbo
        glBindBuffer(GL_ARRAY_BUFFER, cVBO);
        glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_DYNAMIC_DRAW);
        GL::cVBOvec.emplace_back(cVBO);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        pVBO = 0;
        cVBO = 0;
        VAO = 0;
        EBO = 0;
    }
}
//void GL::Camera(std::array<float, 3> &tilt, std::array<float, 3> &camera, uint16_t screenWidth, uint16_t screenHeight) {
        /*float fovover2 = std::cos(0.6981317);
        objIdent obj;
                    
        for (size_t i{}; i < obj.objects.size(); i++){
            Coord &cs = obj.objects[i];
            float minxx = (tilt[0] * (cs.detailes[0][0] - camera[0]) + tilt[2] * (cs.detailes[0][2] - camera[2])) / std::sqrt((cs.detailes[0][0] - camera[0]) * (cs.detailes[0][0] - camera[0]) + (cs.detailes[0][2] - camera[2]) * (cs.detailes[0][2] - camera[2])),
                  minxy = (tilt[1] * (cs.detailes[0][1] - camera[1]) + tilt[2] * (cs.detailes[0][2] - camera[2])) / std::sqrt((cs.detailes[0][1] - camera[1]) * (cs.detailes[0][1] - camera[1]) + (cs.detailes[0][2] - camera[2]) * (cs.detailes[0][2] - camera[2])),
                  maxxx = (tilt[0] * (cs.detailes[1][0] - camera[0]) + tilt[2] * (cs.detailes[1][2] - camera[2])) / std::sqrt((cs.detailes[1][0] - camera[0]) * (cs.detailes[1][0] - camera[0]) + (cs.detailes[1][2] - camera[2]) * (cs.detailes[1][2] - camera[2])),
                  maxxy = (tilt[1] * (cs.detailes[1][1] - camera[1]) + tilt[2] * (cs.detailes[1][2] - camera[2])) / std::sqrt((cs.detailes[1][1] - camera[1]) * (cs.detailes[1][1] - camera[1]) + (cs.detailes[1][2] - camera[2]) * (cs.detailes[1][2] - camera[2])),

                  minyx = (tilt[0] * (cs.detailes[2][0] - camera[0]) + tilt[2] * (cs.detailes[2][2] - camera[2])) / std::sqrt((cs.detailes[2][0] - camera[0]) * (cs.detailes[2][0] - camera[0]) + (cs.detailes[2][2] - camera[2]) * (cs.detailes[2][2] - camera[2])),
                  minyy = (tilt[1] * (cs.detailes[2][1] - camera[1]) + tilt[2] * (cs.detailes[2][2] - camera[2])) / std::sqrt((cs.detailes[2][1] - camera[1]) * (cs.detailes[2][1] - camera[1]) + (cs.detailes[2][2] - camera[2]) * (cs.detailes[2][2] - camera[2])),
                  maxyx = (tilt[0] * (cs.detailes[3][0] - camera[0]) + tilt[2] * (cs.detailes[3][2] - camera[2])) / std::sqrt((cs.detailes[3][0] - camera[0]) * (cs.detailes[3][0] - camera[0]) + (cs.detailes[3][2] - camera[2]) * (cs.detailes[3][2] - camera[2])),
                  maxyy = (tilt[1] * (cs.detailes[3][1] - camera[1]) + tilt[2] * (cs.detailes[3][2] - camera[2])) / std::sqrt((cs.detailes[3][1] - camera[1]) * (cs.detailes[3][1] - camera[1]) + (cs.detailes[3][2] - camera[2]) * (cs.detailes[3][2] - camera[2])),

                  minzx = (tilt[0] * (cs.detailes[4][0] - camera[0]) + tilt[2] * (cs.detailes[4][2] - camera[2])) / std::sqrt((cs.detailes[4][0] - camera[0]) * (cs.detailes[5][0] - camera[0]) + (cs.detailes[4][2] - camera[2]) * (cs.detailes[4][2] - camera[2])),
                  minzy = (tilt[1] * (cs.detailes[4][1] - camera[1]) + tilt[2] * (cs.detailes[4][2] - camera[2])) / std::sqrt((cs.detailes[4][1] - camera[1]) * (cs.detailes[5][1] - camera[1]) + (cs.detailes[4][2] - camera[2]) * (cs.detailes[4][2] - camera[2])),
                  maxzx = (tilt[0] * (cs.detailes[5][0] - camera[0]) + tilt[2] * (cs.detailes[5][2] - camera[2])) / std::sqrt((cs.detailes[5][0] - camera[0]) * (cs.detailes[4][0] - camera[0]) + (cs.detailes[5][2] - camera[2]) * (cs.detailes[5][2] - camera[2])),
                  maxzy = (tilt[1] * (cs.detailes[5][1] - camera[1]) + tilt[2] * (cs.detailes[5][2] - camera[2])) / std::sqrt((cs.detailes[5][1] - camera[1]) * (cs.detailes[4][1] - camera[1]) + (cs.detailes[5][2] - camera[2]) * (cs.detailes[5][2] - camera[2]));
            if ((minxx > fovover2 && maxxx > fovover2) || minyx > fovover2 && maxyx > fovover2 && minzx)
                continue;
        }*/
//}
void GL::Draw(uint16_t screenWidth, uint16_t screenHeight)
    {

    objIdent obj;
    glClearColor(0.2f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, screenWidth, screenHeight);
    
    for (size_t i{}; i < obj.objects.size(); i++) {
        Coord &cs = obj.objects[i];
        glBindVertexArray(GL::VAOvec[i]);
        glDrawElements(GL_TRIANGLES, cs.triangles.size() * 3, GL_UNSIGNED_INT, nullptr);
    }
    }

