#include <iostream>
#include <cmath>
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <chrono>
#include "glmain.h"
#include "window.h"
#include "shader.h"
#include "3dvfunc.h"
#include "thread"


void Window::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        std::cout << SDL_GetError() << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "Initialized" << std::endl;
    }
    GL::gWindowWidth = {1280};
    GL::gWindowHeight = {720};
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    window = SDL_CreateWindow("Engine",
                              GL::gWindowWidth, GL::gWindowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    if (window == nullptr)
    {
        std::cout << SDL_GetError() << std::endl;
        return;
    }
    glContext = SDL_GL_CreateContext(window);
    
    if (SDL_GL_MakeCurrent(window, glContext) != true)
    {
        std::cout << SDL_GetError() << std::endl;
    }
    if (glContext == nullptr)
    {
        std::cout << SDL_GetError() << std::endl;
    }
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "glad couldn't initialize" << std::endl;
    }
    
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    GL::shader = new Shader("../lib/shader/triangle.vert",
    "../lib/shader/triangle.frag"
    );
    GL::shader->Use();
    
    GL::gCamera = {0.0f, -10.0f, -50.0f};
    GL::gCameraDesc = {0.0f, -10.0f, -49.0f, 0.0f, -9.0f, -50.0f, 1.0f, -10.0f, -50.0f};
    float translation[16] = {
        GL::gCameraDesc[6]-GL::gCamera[0], GL::gCameraDesc[7]-GL::gCamera[1], -(GL::gCameraDesc[8]-GL::gCamera[2]), 0.0f,
        GL::gCameraDesc[3]-GL::gCamera[0], GL::gCameraDesc[4]-GL::gCamera[1], -(GL::gCameraDesc[5]-GL::gCamera[2]), 0.0f,
        GL::gCameraDesc[0]-GL::gCamera[0], GL::gCameraDesc[1]-GL::gCamera[1], -(GL::gCameraDesc[2]-GL::gCamera[2]), 0.0f,
        -GL::gCamera[0], -GL::gCamera[1], GL::gCamera[2], 1.0f};
    
    //float translation[16] = { 1.0, 0, 0, 0,
    //                          0, 1.0, 0, 0, 
    //                          0, 0, -1.0, 0,
    //                          0, 10.0f , -50.0, 1.0};
    float perspective[16] = {
        1 / (0.839099 * GL::gWindowWidth / GL::gWindowHeight), 0.0f, 0.0f, 0.0f,
        0.0f, 1 / 0.839099, 0.0f, 0.0f,
        0.0f, 0.0f, -1.002002f, -1.0f,
        0.0f, 0.0f, -0.2002002f, 0.0f};
    //std::cout << "perspective[10]=" << perspective[10] << " perspective[14]=" << perspective[14] << std::endl;
    GLint cameraloc = glGetUniformLocation(GL::shader->program_index, "translation");
    glUniformMatrix4fv(cameraloc, 1, GL_FALSE, &translation[0]);
    //std::cout << "translation loc: " << cameraloc << std::endl;
    GLint perspectiveloc = glGetUniformLocation(GL::shader->program_index, "perspective");
    glUniformMatrix4fv(perspectiveloc, 1, GL_FALSE, &perspective[0]);
    //std::cout << "perspective loc: " << perspectiveloc << std::endl;

        
};
        void Window::Mainloop()
        {
            std::vector<std::string> filenames = {"../../example/humanoid_quad.obj"};
            GL::Compile(filenames);
            //GLuint tbo;
            //glGenBuffers(1, &tbo);
            //glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, tbo);
            //glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, (objIdent::objects.back().vpc.size())/3 * 4 * sizeof(float), nullptr, GL_DYNAMIC_READ);
            //glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);
            
            bool running = true;
            SDL_Event e;
            //glEnable(GL_RASTERIZER_DISCARD); // skip actual rendering while capturing
            //glBeginTransformFeedback(GL_TRIANGLES);
            //glBindVertexArray(GL::VAOvec.back());
            //glDrawElements(GL_TRIANGLES, objIdent::objects.back().triangles.size() * 3, GL_UNSIGNED_INT, nullptr);
            //glEndTransformFeedback();
            //glDisable(GL_RASTERIZER_DISCARD);
            //std::vector<float> result((objIdent::objects.back().vpc.size())/3  * 4);
            //glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, result.size() * sizeof(float), result.data());
            //for (size_t i = 0; i < result.size(); i += 4)
                //{std::cout << result[i] << " " << result[i+1] << " " << result[i+2] << " " << result[i+3] << std::endl;}
                //const bool *mousestate = SDL_GetMouseState(nullptr);
                while (running)
                {
                    while (SDL_PollEvent(&e) == true)
                    {
                        if (e.type == SDL_EVENT_QUIT)
                            running = false;
                        else if (e.type == SDL_EVENT_KEY_DOWN)
                            SDL_Log("Key pressed: %d", e.key.key);
                        else if (e.type == SDL_EVENT_WINDOW_RESIZED)
                        {
                            GL::gWindowWidth = e.window.data1;
                            GL::gWindowHeight = e.window.data2;
                            glViewport(0, 0, GL::gWindowWidth, GL::gWindowHeight);
                            float perspective[16] = {
                                1 / (0.839099 * GL::gWindowWidth / GL::gWindowHeight), 0.0f, 0.0f, 0.0f,
                                0.0f, 1 / 0.839099, 0.0f, 0.0f,
                                0.0f, 0.0f, -1.002002f, -1.0f,
                                0.0f, 0.0f, -0.2002002f, 0.0f};
                            GLint perspectiveloc = glGetUniformLocation(GL::shader->program_index, "perspective");
                            glUniformMatrix4fv(perspectiveloc, 1, GL_FALSE, &perspective[0]);
                        }
                        /*else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                        {
                            while(e.button.button == SDL_BUTTON_MIDDLE)
                            {
                                float h, v;
                                SDL_MouseButtonFlags mouse = SDL_GetRelativeMouseState(&h, &v);
                                if(h == 0 && v==0) {
                                    continue;
                                }else{
                                    std::cout << "horizontal: " << h << "vertical: " << v << std::endl;
                                }
                                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                            }
                        }*/
                       
                        else if (e.type == SDL_EVENT_MOUSE_WHEEL)
                        {
                            float diffx = (GL::gCameraDesc[0] - GL::gCamera[0]) * e.wheel.y;
                            float diffz = (GL::gCameraDesc[1] - GL::gCamera[1]) * e.wheel.y;
                            float diffy = (GL::gCameraDesc[2] - GL::gCamera[2]) * e.wheel.y;
                            if (e.wheel.direction == SDL_MOUSEWHEEL_NORMAL) {
                                std::cout << "horizontal: " << e.wheel.x << " " << "vertical: " << e.wheel.y << std::endl;
                                GL::gCamera[0] += diffx;
                                GL::gCamera[1] += diffy;
                                GL::gCamera[2] += diffz;
                                GL::gCameraDesc[0] += diffx;
                                GL::gCameraDesc[1] += diffy;
                                GL::gCameraDesc[2] += diffz;
                                GL::gCameraDesc[3] += diffx;
                                GL::gCameraDesc[4] += diffy;
                                GL::gCameraDesc[5] += diffz;
                                GL::gCameraDesc[6] += diffx;
                                GL::gCameraDesc[7] += diffy;
                                GL::gCameraDesc[8] += diffz;
                                
                                float translation[16] = {
                                    GL::gCameraDesc[6]-GL::gCamera[0], GL::gCameraDesc[7]-GL::gCamera[1], -(GL::gCameraDesc[8]-GL::gCamera[2]), 0.0f,
                                    GL::gCameraDesc[3]-GL::gCamera[0], GL::gCameraDesc[4]-GL::gCamera[1], -(GL::gCameraDesc[5]-GL::gCamera[2]), 0.0f,
                                    GL::gCameraDesc[0]-GL::gCamera[0], GL::gCameraDesc[1]-GL::gCamera[1], -(GL::gCameraDesc[2]-GL::gCamera[2]), 0.0f,
                                    -GL::gCamera[0], -GL::gCamera[1], GL::gCamera[2], 1.0f
                                };
                            
                                
                                float perspective[16] = {
                                    1 / (0.839099 * GL::gWindowWidth / GL::gWindowHeight), 0.0f, 0.0f, 0.0f,
                                    0.0f, 1 / 0.839099, 0.0f, 0.0f,
                                    0.0f, 0.0f, -1.002002f, -1.0f,
                                    0.0f, 0.0f, -0.2002002f, 0.0f};
                                GLint cameraloc = glGetUniformLocation(GL::shader->program_index, "translation");
                                glUniformMatrix4fv(cameraloc, 1, GL_FALSE, &translation[0]);
                                GLint perspectiveloc = glGetUniformLocation(GL::shader->program_index, "perspective");
                                glUniformMatrix4fv(perspectiveloc, 1, GL_FALSE, &perspective[0]);
                            }
                            else
                            {
                                std::cout << "horizontal: " << e.wheel.x << " " << "vertical: " << e.wheel.y << std::endl;
                                GL::gCamera[0] -= diffx;
                                GL::gCamera[1] -= diffy;
                                GL::gCamera[2] -= diffz;
                                GL::gCameraDesc[0] -= diffx;
                                GL::gCameraDesc[1] -= diffy;
                                GL::gCameraDesc[2] -= diffz;
                                GL::gCameraDesc[3] -= diffx;
                                GL::gCameraDesc[4] -= diffy;
                                GL::gCameraDesc[5] -= diffz;
                                GL::gCameraDesc[6] -= diffx;
                                GL::gCameraDesc[7] -= diffy;
                                GL::gCameraDesc[8] -= diffz;
                                float translation[16] = {
                                GL::gCameraDesc[6]-GL::gCamera[0], GL::gCameraDesc[7]-GL::gCamera[1], -(GL::gCameraDesc[8]-GL::gCamera[2]), 0.0f,
                                GL::gCameraDesc[3]-GL::gCamera[0], GL::gCameraDesc[4]-GL::gCamera[1], -(GL::gCameraDesc[5]-GL::gCamera[2]), 0.0f,
                                GL::gCameraDesc[0]-GL::gCamera[0], GL::gCameraDesc[1]-GL::gCamera[1], -(GL::gCameraDesc[2]-GL::gCamera[2]), 0.0f,
                                -GL::gCamera[0], -GL::gCamera[1], GL::gCamera[2], 1.0f};
                            
                                
                                float perspective[16] = {
                                    1 / (0.839099 * GL::gWindowWidth / GL::gWindowHeight), 0.0f, 0.0f, 0.0f,
                                    0.0f, 1 / 0.839099, 0.0f, 0.0f,
                                    0.0f, 0.0f, -1.002002f, -1.0f,
                                    0.0f, 0.0f, -0.2002002f, 0.0f};
                                GLint cameraloc = glGetUniformLocation(GL::shader->program_index, "translation");
                                glUniformMatrix4fv(cameraloc, 1, GL_FALSE, &translation[0]);
                                GLint perspectiveloc = glGetUniformLocation(GL::shader->program_index, "perspective");
                                glUniformMatrix4fv(perspectiveloc, 1, GL_FALSE, &perspective[0]);
                            }
                        }
                    }
                    GL::Draw(GL::gWindowWidth, GL::gWindowHeight);
                    SDL_GL_SwapWindow(window);
                }
        };
        void Window::Clean()
        {
            glDeleteVertexArrays(GL::VAOvec.size(), GL::VAOvec.data());
            glDeleteBuffers(GL::cVBOvec.size(), GL::cVBOvec.data());
            glDeleteBuffers(GL::pVBOvec.size(), GL::pVBOvec.data());
            glDeleteBuffers(GL::EBOvec.size(), GL::EBOvec.data());
            SDL_GL_DestroyContext(glContext);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }


    

