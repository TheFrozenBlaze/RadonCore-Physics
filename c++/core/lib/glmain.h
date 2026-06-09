#ifndef GLMAIN_H
#define GLMAIN_H
#include <vector>

namespace GL {
    void Context(int w, int h);
    void Draw(std::vector<float>* x,std::vector<float>* y,std::vector<float>* z, std::vector<uint32_t>* triangles, size_t xs, size_t ys, size_t zs, size_t triangless);
    void PrematureDraw();
}
#endif