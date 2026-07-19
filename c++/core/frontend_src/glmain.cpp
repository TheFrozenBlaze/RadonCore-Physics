#include "3dvfunc.h"
#include "glmain.h"
#include "shader.h"
#include <vector>
#include <algorithm>
#include <glad/glad.h>
#include <cmath>
#include <cstdint>

GLuint VAO {};
GLuint cVBO {};
GLuint pVBO {};
GLuint EBO {};
GLuint lEBO{};

std::vector<GLuint> GL::VAOvec;
std::vector<GLuint> GL::pVBOvec;
std::vector<GLuint> GL::EBOvec;
std::vector<GLuint> GL::lEBOvec;

uint16_t GL::gWindowHeight, GL::gWindowWidth;
std::array<float,3> GL::gCamera;
std::array<std::array<float,3>, 3> GL::gCameraDesc;
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
            
            for (size_t i{}; i < cs.edge.size(); i++)
            {
                std::cout << "Edge" << " "<< cs.edge[i] << std::endl;
            }
            std::cout << "size" << " " << cs.vpc.size() << std::endl;
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

            glGenBuffers(1, &lEBO);
            // ebo

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, cs.triangles.size() * 3 * sizeof(uint32_t), cs.triangles.data(), GL_STATIC_DRAW);
            GL::EBOvec.emplace_back(EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, cs.edge.size() * sizeof(uint32_t), cs.edge.data(), GL_STATIC_DRAW);
            GL::lEBOvec.emplace_back(lEBO);
            // position vbo
            glBindBuffer(GL_ARRAY_BUFFER, pVBO);
            glBufferData(GL_ARRAY_BUFFER, cs.vpc.size() * sizeof(float), cs.vpc.data(), GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
            GL::pVBOvec.emplace_back(pVBO);
            glEnableVertexAttribArray(0);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            pVBO = 0;
            cVBO = 0;
            VAO = 0;
            EBO = 0;
        }
}

void GL::Draw(uint16_t *screenWidth, uint16_t* screenHeight)
    {

    objIdent obj;
    glClearColor(0.2f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, *screenWidth, *screenHeight);
    GLint colorLoc = glGetUniformLocation(GL::shader->program_index, "uColor");
    for (size_t i{}; i < obj.objects.size(); i++) {
        Coord &cs = obj.objects[i];

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);

        std::cout << colorLoc << std::endl;
        glUniform3f(colorLoc, 0.75f,0.75f,0.75f);
        glBindVertexArray(GL::VAOvec[i]);
        
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL::EBOvec[i]);
        glDrawElements(GL_TRIANGLES, cs.triangles.size() * 3, GL_UNSIGNED_INT, nullptr);
        std::cout << colorLoc << std::endl;
        glUniform3f(colorLoc, 0.f,0.f,0.f);
        glDisable(GL_POLYGON_OFFSET_FILL);
        //glDisable(GL_DEPTH_TEST);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL::lEBOvec[i]);
        glLineWidth(4.0f);
        glDrawElements(GL_LINES, cs.edge.size(), GL_UNSIGNED_INT, nullptr);

        //glEnable(GL_DEPTH_TEST);
    }
    }

