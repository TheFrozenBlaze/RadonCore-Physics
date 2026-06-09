#ifndef GLMAIN_H
#define GLMAIN_H
#include <vector>
#include <glad/glad.h>
class Shader;
namespace GL {
    extern GLuint VAO;
    extern GLuint VBO;
    extern Shader* shader;
    void Context(int w, int h);
    void Compile();
    void Draw(std::vector<float> *x, std::vector<float> *y, std::vector<float> *z, std::vector<uint32_t> *triangles, size_t xs, size_t ys, size_t zs, size_t triangless);
    void PrematureDraw(uint16_t screenWidth, uint16_t screenHeight);
}
#endif