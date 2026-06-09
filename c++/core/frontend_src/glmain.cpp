#include <glad/glad.h>
#include "glmain.h"
#include "shader.h"
#include <vector>

GLuint GL::VAO {};
GLuint GL::VBO {};
Shader* GL::shader = nullptr;

void GL::Context(int w, int h)
{
glViewport(0, 0, w, h);
glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT);
};
void GL::Compile() {
    const std::vector<GLfloat> vertices {
        -0.8f, 0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.2f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    glGenVertexArrays(1, &GL::VAO);
    glBindVertexArray(GL::VAO);
    glGenBuffers(1, &GL::VBO);
    glBindBuffer(GL_ARRAY_BUFFER, GL::VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    GL::shader = new Shader("/home/matyas/Documents/3Dengine/c++/core/lib/shader/triangle.vs", "/home/matyas/Documents/3Dengine/c++/core/lib/shader/triangle.fs");
    
}
void GL::PrematureDraw(uint16_t screenWidth, uint16_t screenHeight)
    {
    glClearColor(0.2f, 0.5f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, screenWidth, screenHeight);
    GL::shader->Use();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    }

void GL::Draw(std::vector<float>* x,std::vector<float>* y,std::vector<float>* z, std::vector<uint32_t>* triangles, size_t xs, size_t ys, size_t zs, size_t triangless) {

    
}