#ifndef GLMAIN_H
#define GLMAIN_H
#include <vector>
#include <glad/glad.h>
#include <array>
#include "shader.h"
#include <cstdint>
class Shader;
namespace GL
{
    extern Shader* shader;
    extern uint16_t gWindowHeight, gWindowWidth;
    extern std::array<float,3> gCamera;
    extern std::array<std::array<float,3>, 3> gCameraDesc;
    extern std::vector<std::pair<float, float>> gridvert;
    uint64_t Compile(SimDet &det);
    void DrawGrid(float prevx, float prevy, GLuint &grVAO, GLuint &grVBO);
    void Draw(SimDef &currentProj);
}
#endif