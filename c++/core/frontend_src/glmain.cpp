#include <glad/glad.h>
#include "glmain.h"
#include <vector>

    
void GL::Context(int w, int h)
{
glViewport(0, 0, w, h);
glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT);
};
void GL::PrematureDraw() {
    GLuint VAO {};
    GLuint VBO {};
    const std::vector<GLfloat> vertices {
        -0.8f, 0.7f, 0.0f,
        0.0f, 0.2f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
}
void GL::Draw(std::vector<float>* x,std::vector<float>* y,std::vector<float>* z, std::vector<uint32_t>* triangles, size_t xs, size_t ys, size_t zs, size_t triangless) {

    
}