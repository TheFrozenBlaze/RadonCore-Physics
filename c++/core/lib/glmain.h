#ifndef GLMAIN_H
#define GLMAIN_H
#include <vector>
#include <glad/glad.h>
#include <array>
#include "shader.h"
class Shader;
namespace GL
{
    extern Shader* shader;
    extern uint16_t gWindowHeight, gWindowWidth;
    extern std::array<float,3> gCamera;
    extern std::array<float,9> gCameraDesc;
    extern std::vector<GLuint> VAOvec;
    extern std::vector<GLuint> pVBOvec;
    extern std::vector<GLuint> cVBOvec;
    extern std::vector<GLuint> EBOvec;
    void Compile(const std::vector<std::string> &files);
    void Draw(uint16_t screenWidth, uint16_t screenHeight);
    void Camera();

}
#endif