#include <iostream>
#include <cmath>
#include "myimgui.h"
#include <glad/glad.h>
#include "window.h"
#include "shader.h"

bool Window::MMB() {
    SDL_PumpEvents();

    SDL_MouseButtonFlags buttons = SDL_GetMouseState(nullptr, nullptr);

    if(buttons & SDL_BUTTON_MMASK)
    {
        //std::cout << "trying to rotate" << std::endl;
        return true;
    } else {
        return false;
    }
};

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
    
    
    GL::shader = new Shader("../core/lib/shader/triangle.vert",
    "../core/lib/shader/triangle.frag"
    );
    GL::shader->Use();
    
    GL::gCamera = {0.0f, -10.0f, -20.0f};
    GL::gCameraDesc = {1.0f, -10.0f, -20.0f, 0.0f, -9.0f, -20.0f ,0.0f, -10.0f, -19.0f };
float translation[16] = {
        GL::gCameraDesc[0][0]-GL::gCamera[0], GL::gCameraDesc[1][0]-GL::gCamera[0], -(GL::gCameraDesc[2][0]-GL::gCamera[0]), 0.0f,
        GL::gCameraDesc[0][1]-GL::gCamera[1], GL::gCameraDesc[1][1]-GL::gCamera[1], -(GL::gCameraDesc[2][1]-GL::gCamera[1]), 0.0f,
        GL::gCameraDesc[0][2]-GL::gCamera[2], GL::gCameraDesc[1][2]-GL::gCamera[2], -(GL::gCameraDesc[2][2]-GL::gCamera[2]), 0.0f,
        -(GL::gCamera[0]), -GL::gCamera[1], GL::gCamera[2], 1.0f};
    
    //float translation[16] = { 1.0, 0, 0, 0,
    //                          0, 1.0, 0, 0, 
    //                          0, 0, -1.0, 0,
    //                          0, 10.0f , -50.0, 1.0};
    float perspective[16] = {
        static_cast<float>(1 / (0.839099 * GL::gWindowWidth / GL::gWindowHeight)), 0.0f, 0.0f, 0.0f,
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

            std::cout << "new cycle" << std::endl;

            GLuint gridVAO {};
            GLuint gridVBO {};
            glGenVertexArrays(1, &gridVAO);
            glBindVertexArray(gridVAO);
            glGenBuffers(1, &gridVBO);
            glBindBuffer(GL_ARRAY_BUFFER, gridVBO); 
            glVertexAttribPointer(1, 2,  GL_FLOAT, GL_FALSE, sizeof(std::pair<float, float>), (void*)0);
            glEnableVertexAttribArray(1);
            int minx = std::floor(GL::gCamera[0]-100)/10;
            int maxx = std::floor(GL::gCamera[0]+100)/10;
            int minz = std::floor(GL::gCamera[2]-100)/10;
            int maxz = std::floor(GL::gCamera[2]+100)/10;
            for (int x = minx; x <= maxx; x++)
            {
                GL::gridvert.emplace_back(std::pair<float,float>{x * 10.f, minz * 10.f});
                GL::gridvert.emplace_back(std::pair<float,float>{x * 10.f, maxz * 10.f});
            }

            for (int z = minz; z <= maxz; z++)
            {
                GL::gridvert.emplace_back(std::pair<float,float>{minx * 10.f, z * 10.f});
                GL::gridvert.emplace_back(std::pair<float,float>{maxx * 10.f, z * 10.f});
            }

            //std::vector<std::string> filenames = {"../../example/humanoid_quad.obj"};
            //GL::Compile(filenames);
            bool running = true;
            SDL_Event e;
            
            const bool *keyboard = SDL_GetKeyboardState(nullptr);
            float rad = 0.01745329;
            int8_t fovdeg = 40;
            float FOV = std::tan(rad*fovdeg);
            float prevx {};
            float prevy {};
            bool showImportPopup{false};
            GUI::GUIInit(window);
            while (running)
            {
                prevx = GL::gCamera[0];
                prevy = GL::gCamera[2];
                
                while (SDL_PollEvent(&e) == true)
                {
                    ImGui_ImplSDL3_ProcessEvent(&e);
                    if (e.type == SDL_EVENT_QUIT)
                        running = false;
                    
                            
                    else if (e.type == SDL_EVENT_WINDOW_RESIZED)
                    {
                            GL::gWindowWidth = e.window.data1;
                            GL::gWindowHeight = e.window.data2;
                            glViewport(0, 0, GL::gWindowWidth, GL::gWindowHeight);
                            float perspective[16] = {
                                static_cast<float>(1 / (FOV * GL::gWindowWidth / GL::gWindowHeight)), 0.0f, 0.0f, 0.0f,
                                0.0f, 1 / FOV, 0.0f, 0.0f,
                                0.0f, 0.0f, -1.002002f, -1.0f,
                                0.0f, 0.0f, -0.2002002f, 0.0f};
                            GLint perspectiveloc = glGetUniformLocation(GL::shader->program_index, "perspective");
                            glUniformMatrix4fv(perspectiveloc, 1, GL_FALSE, &perspective[0]);
                    }
                        else if ((e.type == SDL_EVENT_MOUSE_MOTION && MMB()))
                        {
                            
                            GL::gCameraDesc[0] = {
                                GL::gCamera[0] + (static_cast<float>((GL::gCameraDesc[0][2] - GL::gCamera[2]) * sin(rad * e.motion.xrel * 0.1) + (GL::gCameraDesc[0][0] - GL::gCamera[0]) * cos(rad * e.motion.xrel * 0.1))),
                                GL::gCameraDesc[0][1],
                                GL::gCamera[2] + (static_cast<float>((GL::gCameraDesc[0][2] - GL::gCamera[2]) * cos(rad * e.motion.xrel * 0.1) + (GL::gCameraDesc[0][0] - GL::gCamera[0]) * (-sin(rad * e.motion.xrel * 0.1))))
                                };
                            GL::gCameraDesc[1] = {
                                GL::gCamera[0] + (static_cast<float>((GL::gCameraDesc[1][2] - GL::gCamera[2]) * sin(rad * e.motion.xrel * 0.1) + (GL::gCameraDesc[1][0] - GL::gCamera[0]) * cos(rad * e.motion.xrel * 0.1))),
                                GL::gCameraDesc[1][1],
                                GL::gCamera[2] + (static_cast<float>((GL::gCameraDesc[1][2] - GL::gCamera[2]) * cos(rad * e.motion.xrel * 0.1) + (GL::gCameraDesc[1][0] - GL::gCamera[0]) * (-sin(rad * e.motion.xrel * 0.1))))
                                };
                            GL::gCameraDesc[2] = {
                                GL::gCamera[0] + (static_cast<float>((GL::gCameraDesc[2][2] - GL::gCamera[2]) * sin(rad * e.motion.xrel * 0.1) + (GL::gCameraDesc[2][0] - GL::gCamera[0]) * cos(rad * e.motion.xrel * 0.1))),
                                GL::gCameraDesc[2][1],
                                GL::gCamera[2] + (static_cast<float>((GL::gCameraDesc[2][2] - GL::gCamera[2]) * cos(rad * e.motion.xrel * 0.1) + (GL::gCameraDesc[2][0] - GL::gCamera[0]) * (-sin(rad * e.motion.xrel * 0.1))))
                            };
                            //std::cout << "sin: " <<  sin(rad * e.motion.xrel * 0.1) << " " << "cos: "<< cos(rad * e.motion.xrel * 0.1) << std::endl;
                            //std::cout << "sin: " <<  sin(rad * e.motion.yrel * 0.1) << " " << "cos: "<< cos(rad * e.motion.yrel * 0.1) << std::endl;
                            //std::cout << "xrot forw: " << GL::gCameraDesc[0][0] << " "<<GL::gCameraDesc[0][1]<< " "<<GL::gCameraDesc[0][2]<< std::endl;
                            //std::cout << "xrot up: " << GL::gCameraDesc[1][0] << " "<<GL::gCameraDesc[1][1]<< " "<<GL::gCameraDesc[1][2]<< std::endl;
                            //std::cout << "xrot side: " << GL::gCameraDesc[2][0] << " "<<GL::gCameraDesc[2][1]<< " "<<GL::gCameraDesc[2][2]<< std::endl;
                            GL::gCameraDesc[0] = { GL::gCameraDesc[0][0],
                                                   GL::gCamera[1] + (static_cast<float>((GL::gCameraDesc[0][1] - GL::gCamera[1]) * cos(rad * e.motion.yrel * 0.1) + (GL::gCameraDesc[0][2] - GL::gCamera[2]) * (-sin(rad * e.motion.yrel * 0.1)))),
                                                   GL::gCamera[2] + (static_cast<float>((GL::gCameraDesc[0][1] - GL::gCamera[1]) * sin(rad * e.motion.yrel * 0.1) + (GL::gCameraDesc[0][2] - GL::gCamera[2]) * cos(rad * e.motion.yrel * 0.1)))
                                                };
                            GL::gCameraDesc[1] = {GL::gCameraDesc[1][0],
                                                  GL::gCamera[1] + (static_cast<float>((GL::gCameraDesc[1][1] - GL::gCamera[1]) * cos(rad * e.motion.yrel * 0.1) + (GL::gCameraDesc[1][2] - GL::gCamera[2]) * (-sin(rad * e.motion.yrel * 0.1)))),
                                                  GL::gCamera[2] + (static_cast<float>((GL::gCameraDesc[1][1] - GL::gCamera[1]) * sin(rad * e.motion.yrel * 0.1) + (GL::gCameraDesc[1][2] - GL::gCamera[2]) * cos(rad * e.motion.yrel * 0.1)))
                                                };
                            GL::gCameraDesc[2] = {GL::gCameraDesc[2][0],
                                                  GL::gCamera[1] + (static_cast<float>((GL::gCameraDesc[2][1] - GL::gCamera[1]) * cos(rad * e.motion.yrel * 0.1) + (GL::gCameraDesc[2][2] - GL::gCamera[2]) * (-sin(rad * e.motion.yrel * 0.1)))),
                                                  GL::gCamera[2] + (static_cast<float>((GL::gCameraDesc[2][1] - GL::gCamera[1]) * sin(rad * e.motion.yrel * 0.1) + (GL::gCameraDesc[2][2] - GL::gCamera[2]) * cos(rad * e.motion.yrel * 0.1)))
                                                };
                            //std::cout << "yrot up: " << GL::gCameraDesc[0][0] << " " << GL::gCameraDesc[0][1]<< " " << GL::gCameraDesc[0][2]<< std::endl;
                            //std::cout << "yrot forw: " << GL::gCameraDesc[1][0] << " " << GL::gCameraDesc[1][1]<< " " << GL::gCameraDesc[1][2]<< std::endl;
                            //std::cout << "yrot side: " << GL::gCameraDesc[2][0] << " " << GL::gCameraDesc[2][1]<< " " << GL::gCameraDesc[2][2]<< std::endl;
                            
                            float translation[16] = {
                                GL::gCameraDesc[0][0]-GL::gCamera[0], GL::gCameraDesc[1][0]-GL::gCamera[0], -(GL::gCameraDesc[2][0]-GL::gCamera[0]), 0.0f,
                                GL::gCameraDesc[0][1]-GL::gCamera[1], GL::gCameraDesc[1][1]-GL::gCamera[1], -(GL::gCameraDesc[2][1]-GL::gCamera[1]), 0.0f,
                                GL::gCameraDesc[0][2]-GL::gCamera[2], GL::gCameraDesc[1][2]-GL::gCamera[2], -(GL::gCameraDesc[2][2]-GL::gCamera[2]), 0.0f,
                                -(GL::gCamera[0]*(GL::gCameraDesc[0][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[0][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[0][2]-GL::gCamera[2])),
                                -(GL::gCamera[0]*(GL::gCameraDesc[1][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[1][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[1][2]-GL::gCamera[2])),
                                (GL::gCamera[0]*(GL::gCameraDesc[2][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[2][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[2][2]-GL::gCamera[2])), 1.0f
                            };
                           

                            //std::cout << "forw dot up: " << (GL::gCameraDesc[0][0]-GL::gCamera[0]) * (GL::gCameraDesc[1][0]-GL::gCamera[0]) + (GL::gCameraDesc[0][1]-GL::gCamera[1]) * (GL::gCameraDesc[1][1]-GL::gCamera[1]) + (GL::gCameraDesc[0][2]-GL::gCamera[2]) * (GL::gCameraDesc[1][2]-GL::gCamera[2]) << std::endl;
                            //std::cout << "up dot side: " << (GL::gCameraDesc[2][0]-GL::gCamera[0]) * (GL::gCameraDesc[1][0]-GL::gCamera[0]) + (GL::gCameraDesc[2][1]-GL::gCamera[1]) * (GL::gCameraDesc[1][1]-GL::gCamera[1]) + (GL::gCameraDesc[2][2]-GL::gCamera[2]) * (GL::gCameraDesc[1][2]-GL::gCamera[2]) << std::endl;
                            //std::cout << "forw dot side: " << (GL::gCameraDesc[0][0]-GL::gCamera[0]) * (GL::gCameraDesc[2][0]-GL::gCamera[0]) + (GL::gCameraDesc[0][1]-GL::gCamera[1]) * (GL::gCameraDesc[2][1]-GL::gCamera[1]) + (GL::gCameraDesc[0][2]-GL::gCamera[2]) * (GL::gCameraDesc[2][2]-GL::gCamera[2]) << std::endl;

                                   
    
                            
                            GLint cameraloc = glGetUniformLocation(GL::shader->program_index, "translation");
                            glUniformMatrix4fv(cameraloc, 1, GL_FALSE, &translation[0]);
                           
                        }

                        else if (e.type == SDL_EVENT_MOUSE_WHEEL)
                        {
                            if (keyboard[SDL_SCANCODE_LSHIFT]){
                                fovdeg += e.wheel.y;
                                float perspective[16] = {
                                    static_cast<float>(1 / (FOV * GL::gWindowWidth / GL::gWindowHeight)), 0.0f, 0.0f, 0.0f,
                                    0.0f, 1 / FOV, 0.0f, 0.0f,
                                    0.0f, 0.0f, -1.002002f, -1.0f,
                                    0.0f, 0.0f, -0.2002002f, 0.0f};
                                GLint perspectiveloc = glGetUniformLocation(GL::shader->program_index, "perspective");
                                glUniformMatrix4fv(perspectiveloc, 1, GL_FALSE, &perspective[0]);
                            } else {
                                
                            const float diffx = (GL::gCameraDesc[0][0]-GL::gCamera[0]) * e.wheel.y;
                            const float diffz = (GL::gCameraDesc[0][1]-GL::gCamera[1]) * e.wheel.y;
                            const float diffy = (GL::gCameraDesc[0][2]-GL::gCamera[2]) * e.wheel.y;
                                std::cout << " forw vec: " << (GL::gCameraDesc[0][0]-GL::gCamera[0]) << " " << GL::gCameraDesc[0][1]-GL::gCamera[1]<< " " << GL::gCameraDesc[0][2]-GL::gCamera[2]<< std::endl;

                                //std::cout << "horizontal: " << e.wheel.x << " " << "vertical: " << e.wheel.y << std::endl;
                                GL::gCamera[0] += diffx;
                                GL::gCamera[1] += diffy;
                                GL::gCamera[2] += diffz;
                                GL::gCameraDesc[0][0] += diffx;
                                GL::gCameraDesc[0][1] += diffy;
                                GL::gCameraDesc[0][2] += diffz;
                                GL::gCameraDesc[1][0] += diffx;
                                GL::gCameraDesc[1][1] += diffy;
                                GL::gCameraDesc[1][2] += diffz;
                                GL::gCameraDesc[2][0] += diffx;
                                GL::gCameraDesc[2][1] += diffy;
                                GL::gCameraDesc[2][2] += diffz;
                                //std::cout << "move side: " << GL::gCameraDesc[2][0] << " " << GL::gCameraDesc[2][1]<< " " << GL::gCameraDesc[2][2]<< std::endl;
                                //std::cout << "move up: " << GL::gCameraDesc[0][0] << " " << GL::gCameraDesc[0][1]<< " " << GL::gCameraDesc[0][2]<< std::endl;
                                //std::cout << "move forw: " << GL::gCameraDesc[1][0] << " " << GL::gCameraDesc[1][1]<< " " << GL::gCameraDesc[1][2]<< std::endl;
                                
                                float translation[16] = {
                                GL::gCameraDesc[0][0]-GL::gCamera[0], GL::gCameraDesc[1][0]-GL::gCamera[0], -(GL::gCameraDesc[2][0]-GL::gCamera[0]), 0.0f,
                                GL::gCameraDesc[0][1]-GL::gCamera[1], GL::gCameraDesc[1][1]-GL::gCamera[1], -(GL::gCameraDesc[2][1]-GL::gCamera[1]), 0.0f,
                                GL::gCameraDesc[0][2]-GL::gCamera[2], GL::gCameraDesc[1][2]-GL::gCamera[2], -(GL::gCameraDesc[2][2]-GL::gCamera[2]), 0.0f,
                                -(GL::gCamera[0]*(GL::gCameraDesc[0][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[0][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[0][2]-GL::gCamera[2])),
                                -(GL::gCamera[0]*(GL::gCameraDesc[1][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[1][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[1][2]-GL::gCamera[2])),
                                (GL::gCamera[0]*(GL::gCameraDesc[2][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[2][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[2][2]-GL::gCamera[2])), 1.0f
                                };
                                GLint cameraloc = glGetUniformLocation(GL::shader->program_index, "translation");
                                glUniformMatrix4fv(cameraloc, 1, GL_FALSE, &translation[0]);
                        }
                        }
                    }
                    
                    if (keyboard[SDL_SCANCODE_W])
                    {
                            GL::gCameraDesc[0] = { GL::gCameraDesc[0][0],
                                                   static_cast<float>(GL::gCameraDesc[0][1] * cos(rad) + GL::gCameraDesc[0][2] * (-sin(rad))),
                                                   static_cast<float>(GL::gCameraDesc[0][1] * sin(rad) + GL::gCameraDesc[0][2] * cos(rad))
                                                };
                            //std::cout << "yrot forw: " << GL::gCameraDesc[0][0] << " " << GL::gCameraDesc[0][1]<< " " << GL::gCameraDesc[0][2]<< std::endl;
                            GL::gCameraDesc[1] = {GL::gCameraDesc[1][0],
                                                  static_cast<float>(GL::gCameraDesc[1][1] * cos(rad) + GL::gCameraDesc[1][2] * (-sin(rad))),
                                                  static_cast<float>(GL::gCameraDesc[1][1] * sin(rad) + GL::gCameraDesc[1][2] * cos(rad))
                                                };
                            //std::cout << "yrot up: " << GL::gCameraDesc[1][0] << " " << GL::gCameraDesc[1][1]<< " " << GL::gCameraDesc[1][2]<< std::endl;
                            GL::gCameraDesc[2] = {GL::gCameraDesc[2][0],
                                                  static_cast<float>(GL::gCameraDesc[2][1] * cos(rad) + GL::gCameraDesc[2][2] * (-sin(rad))),
                                                  static_cast<float>(GL::gCameraDesc[2][1] * sin(rad) + GL::gCameraDesc[2][2] * cos(rad))
                                                };
                            GL::gCamera = {
                                GL::gCamera[0],
                                static_cast<float>(GL::gCamera[1] * cos(rad) + GL::gCamera[2] * (-sin(rad))),
                                static_cast<float>(GL::gCamera[1] * sin(rad) + GL::gCamera[2] * cos(rad))

                            };
                            // std::cout << "yrot side: " << GL::gCameraDesc[2][0] << " " << GL::gCameraDesc[2][1]<< " " << GL::gCameraDesc[2][2]<< std::endl;
                            float translation[16] = {
                                GL::gCameraDesc[0][0]-GL::gCamera[0], GL::gCameraDesc[1][0]-GL::gCamera[0], -(GL::gCameraDesc[2][0]-GL::gCamera[0]), 0.0f,
                                GL::gCameraDesc[0][1]-GL::gCamera[1], GL::gCameraDesc[1][1]-GL::gCamera[1], -(GL::gCameraDesc[2][1]-GL::gCamera[1]), 0.0f,
                                GL::gCameraDesc[0][2]-GL::gCamera[2], GL::gCameraDesc[1][2]-GL::gCamera[2], -(GL::gCameraDesc[2][2]-GL::gCamera[2]), 0.0f,
                                -(GL::gCamera[0]*(GL::gCameraDesc[0][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[0][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[0][2]-GL::gCamera[2])),
                                -(GL::gCamera[0]*(GL::gCameraDesc[1][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[1][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[1][2]-GL::gCamera[2])),
                                (GL::gCamera[0]*(GL::gCameraDesc[2][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[2][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[2][2]-GL::gCamera[2])), 1.0f
                            };
                            GLint cameraloc = glGetUniformLocation(GL::shader->program_index, "translation");
                            glUniformMatrix4fv(cameraloc, 1, GL_FALSE, &translation[0]);
                    }
                    if (keyboard[SDL_SCANCODE_S])
                    {
                            GL::gCameraDesc[0] = { GL::gCameraDesc[0][0],
                                                   static_cast<float>(GL::gCameraDesc[0][1] * cos(-rad) + GL::gCameraDesc[0][2] * (-sin(-rad))),
                                                   static_cast<float>(GL::gCameraDesc[0][1] * sin(-rad) + GL::gCameraDesc[0][2] * cos(-rad))
                                                };
                            //std::cout << "yrot forw: " << GL::gCameraDesc[0][0] << " " << GL::gCameraDesc[0][1]<< " " << GL::gCameraDesc[0][2]<< std::endl;
                            GL::gCameraDesc[1] = {GL::gCameraDesc[1][0],
                                                  static_cast<float>(GL::gCameraDesc[1][1] * cos(-rad) + GL::gCameraDesc[1][2] * (-sin(-rad))),
                                                  static_cast<float>(GL::gCameraDesc[1][1] * sin(-rad) + GL::gCameraDesc[1][2] * cos(-rad))
                                                };
                            //std::cout << "yrot up: " << GL::gCameraDesc[1][0] << " " << GL::gCameraDesc[1][1]<< " " << GL::gCameraDesc[1][2]<< std::endl;
                            GL::gCameraDesc[2] = {GL::gCameraDesc[2][0],
                                                  static_cast<float>(GL::gCameraDesc[2][1] * cos(-rad) + GL::gCameraDesc[2][2]  * (-sin(-rad))),
                                                  static_cast<float>(GL::gCameraDesc[2][1] * sin(-rad) + GL::gCameraDesc[2][2] * cos(-rad))
                                                };
                            GL::gCamera = {
                                GL::gCamera[0],
                                static_cast<float>(GL::gCamera[1]  * cos(-rad) + GL::gCamera[2] * (-sin(-rad))),
                                static_cast<float>(GL::gCamera[1]  * sin(-rad) + GL::gCamera[2]  * cos(-rad))

                            };
                            //std::cout << "yrot side: " << GL::gCameraDesc[2][0] << " " << GL::gCameraDesc[2][1]<< " " << GL::gCameraDesc[2][2]<< std::endl;
                            float translation[16] = {
                                GL::gCameraDesc[0][0]-GL::gCamera[0], GL::gCameraDesc[1][0]-GL::gCamera[0], -(GL::gCameraDesc[2][0]-GL::gCamera[0]), 0.0f,
                                GL::gCameraDesc[0][1]-GL::gCamera[1], GL::gCameraDesc[1][1]-GL::gCamera[1], -(GL::gCameraDesc[2][1]-GL::gCamera[1]), 0.0f,
                                GL::gCameraDesc[0][2]-GL::gCamera[2], GL::gCameraDesc[1][2]-GL::gCamera[2], -(GL::gCameraDesc[2][2]-GL::gCamera[2]), 0.0f,
                                -(GL::gCamera[0]*(GL::gCameraDesc[0][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[0][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[0][2]-GL::gCamera[2])),
                                -(GL::gCamera[0]*(GL::gCameraDesc[1][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[1][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[1][2]-GL::gCamera[2])),
                                (GL::gCamera[0]*(GL::gCameraDesc[2][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[2][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[2][2]-GL::gCamera[2])), 1.0f
                            };
                 
                        GLint cameraloc = glGetUniformLocation(GL::shader->program_index, "translation");
                            glUniformMatrix4fv(cameraloc, 1, GL_FALSE, &translation[0]);
                    }
                    //std::cout << "before A\n";
                    if (keyboard[SDL_SCANCODE_A])
                    {
                            GL::gCameraDesc[0] = {
                                static_cast<float>(GL::gCameraDesc[0][2] * sin(rad) + GL::gCameraDesc[0][0] * cos(rad)),
                                GL::gCameraDesc[0][1],
                                static_cast<float>(GL::gCameraDesc[0][2] * cos(rad) + GL::gCameraDesc[0][0] * (-sin(rad)))
                                };
                                //std::cout << "xrot forw: " << GL::gCameraDesc[0][0] << " "<<GL::gCameraDesc[0][1]<< " "<<GL::gCameraDesc[0][2]<< std::endl;
                            GL::gCameraDesc[1] = {
                                static_cast<float>(GL::gCameraDesc[1][2] * sin(rad) + GL::gCameraDesc[1][0] * cos(rad)),
                                GL::gCameraDesc[1][1],
                                static_cast<float>(GL::gCameraDesc[1][2] * cos(rad) + GL::gCameraDesc[1][0] * (-sin(rad)))
                                };
                                //std::cout << "xrot up: " << GL::gCameraDesc[1][0] << " "<<GL::gCameraDesc[1][1]<< " "<<GL::gCameraDesc[1][2]<< std::endl;
                            GL::gCameraDesc[2] = {
                                static_cast<float>(GL::gCameraDesc[2][2] * sin(rad) + GL::gCameraDesc[2][0] * cos(rad)),
                                GL::gCameraDesc[2][1],
                                static_cast<float>(GL::gCameraDesc[2][2] * cos(rad) + GL::gCameraDesc[2][0] * (-sin(rad)))
                                };
                                //std::cout << "xrot side: " << GL::gCameraDesc[2][0] << " "<<GL::gCameraDesc[2][1]<< " "<<GL::gCameraDesc[2][2]<< std::endl;
                        GL::gCamera = {
                                static_cast<float>(GL::gCamera[2] * sin(rad) + GL::gCamera[0] * cos(rad)),
                                GL::gCamera[1],
                                static_cast<float>(GL::gCamera[2] * cos(rad) + GL::gCamera[0] * (-sin(rad)))
                                };
                            //std::cout << " after calculating coords" << std::endl;
                            float translation[16] = {
                                GL::gCameraDesc[0][0]-GL::gCamera[0], GL::gCameraDesc[1][0]-GL::gCamera[0], -(GL::gCameraDesc[2][0]-GL::gCamera[0]), 0.0f,
                                GL::gCameraDesc[0][1]-GL::gCamera[1], GL::gCameraDesc[1][1]-GL::gCamera[1], -(GL::gCameraDesc[2][1]-GL::gCamera[1]), 0.0f,
                                GL::gCameraDesc[0][2]-GL::gCamera[2], GL::gCameraDesc[1][2]-GL::gCamera[2], -(GL::gCameraDesc[2][2]-GL::gCamera[2]), 0.0f,
                                -(GL::gCamera[0]*(GL::gCameraDesc[0][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[0][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[0][2]-GL::gCamera[2])),
                                -(GL::gCamera[0]*(GL::gCameraDesc[1][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[1][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[1][2]-GL::gCamera[2])),
                                (GL::gCamera[0]*(GL::gCameraDesc[2][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[2][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[2][2]-GL::gCamera[2])), 1.0f
                            };
                            //std::cout << " after trans" << std::endl;
                            GLint cameraloc = glGetUniformLocation(GL::shader->program_index, "translation");
                            //std::cout << "got loc" << std::endl;
                            glUniformMatrix4fv(cameraloc, 1, GL_FALSE, &translation[0]);
                            //std::cout << "submitted to loc" << std::endl;
                   
                    }
                    if (keyboard[SDL_SCANCODE_D])
                    {
                            GL::gCameraDesc[0] = {
                                static_cast<float>(GL::gCameraDesc[0][2] * sin(-rad) + GL::gCameraDesc[0][0] * cos(-rad)),
                                GL::gCameraDesc[0][1],
                                static_cast<float>(GL::gCameraDesc[0][2] * cos(-rad) + GL::gCameraDesc[0][0] * (-sin(-rad)))
                                };
                                //std::cout << "xrot forw: " << GL::gCameraDesc[0][0] << " "<<GL::gCameraDesc[0][1]<< " "<<GL::gCameraDesc[0][2]<< std::endl;
                            GL::gCameraDesc[1] = {
                                static_cast<float>(GL::gCameraDesc[1][2] * sin(-rad) + GL::gCameraDesc[1][0] * cos(-rad)),
                                GL::gCameraDesc[1][1],
                                static_cast<float>(GL::gCameraDesc[1][2] * cos(-rad) + GL::gCameraDesc[1][0] * (-sin(-rad)))
                                };
                                //std::cout << "xrot up: " << GL::gCameraDesc[1][0] << " "<<GL::gCameraDesc[1][1]<< " "<<GL::gCameraDesc[1][2]<< std::endl;
                            GL::gCameraDesc[2] = {
                                static_cast<float>(GL::gCameraDesc[2][2] * sin(-rad) + GL::gCameraDesc[2][0] * cos(-rad)),
                                GL::gCameraDesc[2][1],
                                static_cast<float>(GL::gCameraDesc[2][2] * cos(-rad) + GL::gCameraDesc[2][0] * (-sin(-rad)))
                                };
                                //std::cout << "xrot side: " << GL::gCameraDesc[2][0] << " "<<GL::gCameraDesc[2][1]<< " "<<GL::gCameraDesc[2][2]<< std::endl;
                            GL::gCamera = {
                                static_cast<float>(GL::gCamera[2] * sin(-rad) + GL::gCamera[0] * cos(-rad)),
                                GL::gCamera[1],
                                static_cast<float>(GL::gCamera[2] * cos(-rad) + GL::gCamera[0] * (-sin(-rad)))
                            };

                            float translation[16] = {
                                GL::gCameraDesc[0][0]-GL::gCamera[0], GL::gCameraDesc[1][0]-GL::gCamera[0], -(GL::gCameraDesc[2][0]-GL::gCamera[0]), 0.0f,
                                GL::gCameraDesc[0][1]-GL::gCamera[1], GL::gCameraDesc[1][1]-GL::gCamera[1], -(GL::gCameraDesc[2][1]-GL::gCamera[1]), 0.0f,
                                GL::gCameraDesc[0][2]-GL::gCamera[2], GL::gCameraDesc[1][2]-GL::gCamera[2], -(GL::gCameraDesc[2][2]-GL::gCamera[2]), 0.0f,
                                -(GL::gCamera[0]*(GL::gCameraDesc[0][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[0][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[0][2]-GL::gCamera[2])),
                                -(GL::gCamera[0]*(GL::gCameraDesc[1][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[1][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[1][2]-GL::gCamera[2])),
                                (GL::gCamera[0]*(GL::gCameraDesc[2][0]-GL::gCamera[0])+GL::gCamera[1]*(GL::gCameraDesc[2][1]-GL::gCamera[1])+GL::gCamera[2]*(GL::gCameraDesc[2][2]-GL::gCamera[2])), 1.0f
                            };
                                //GLint currentProgram;
                                //glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
                                //std::cout << currentProgram << std::endl;
                        GLint cameraloc = glGetUniformLocation(GL::shader->program_index, "translation");
                            glUniformMatrix4fv(cameraloc, 1, GL_FALSE, &translation[0]);
                    }
                    //std::cout << "A done\n";
                    //continue;
                    ImGui_ImplOpenGL3_NewFrame();
                    ImGui_ImplSDL3_NewFrame();
                    ImGui::NewFrame();
                    GUI::MainMenu();
                    static std::vector<std::string> paths;
                    if (ImGuiFileDialog::Instance()->Display("OpenMeshes"))
                    {
                        
                        if (ImGuiFileDialog::Instance()->IsOk())
                        {
                            auto files =
                                ImGuiFileDialog::Instance()->GetSelection();



                            for (auto& [name, path] : files)
                                paths.push_back(path);

                        }
                        ImGuiFileDialog::Instance()->Close();
                        showImportPopup = true;
                    }
                    if(showImportPopup) {
                        ImGui::OpenPopup("Import meshes");
                        showImportPopup = false;
                    }

                    int selectedval = GUI::SelectProjectPopup();
                    //std::cout << selectedval << std::endl;
                    if (selectedval == -1)
                    {
                        std::cout << "reached error popup" << std::endl;
                        ImGui::OpenPopup("Error");
                        selectedval = 0;
                    }
                    else if (selectedval >= 0)
                    {
                        
                        objIdent obj;
                        Physics::projectQueue[selectedval].simPointer->action.reserve(Physics::projectQueue[selectedval].simPointer->action.size() + paths.size());
                        for (size_t i{}; i < paths.size(); i++)
                        {
                            Physics::AddElement(*Physics::projectQueue[selectedval].simPointer, obj.objReader(paths[i]));
                            GL::Compile(Physics::projectQueue[selectedval].simPointer->action.back());
                            paths.clear();
                        }
                        selectedval = 0;
                    }
                    ImGui::SetNextWindowSize(ImVec2(250, 300));
                        ImGui::SetNextWindowPos(ImVec2((GL::gWindowWidth/2)-125, GL::gWindowHeight/2 -150));
                        if (ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
                        {
                            std::cout << "entered" << std::endl;
                            ImGui::Text("You haven't selected any projects!");

                            if(ImGui::Button("Understood")) {
                                ImGui::CloseCurrentPopup();
                                showImportPopup = true;
                            }
                            ImGui::EndPopup();
                        }
                    // std::cout << "Draw\n";
                    GUI::SimDetailWindow();
                    glClearColor(0.2f, 0.1f, 0.2f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);
                    if (drawgrid)
                    {
                        GL::DrawGrid(prevx, prevy, gridVAO, gridVBO);
                    }

                    //std::cout << "ImGui Render\n";
                    ImGui::Render();

                    //std::cout << "ImGui Backend\n";
                    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                    //std::cout << "Swap\n";
                    SDL_GL_SwapWindow(window);

                    //std::cout << "Poll\n";
                }
        };
        void Window::Clean()
        {
            for (size_t i{}; i < Physics::projectQueue.size(); i++) {
                for (size_t j{}; j < Physics::projectQueue[i].simPointer->action.size(); j++) {
                    glDeleteVertexArrays(1, &Physics::projectQueue[i].simPointer->action[j].VAO);
                    glDeleteBuffers(1, &Physics::projectQueue[i].simPointer->action[j].EBO);
                    glDeleteBuffers(1, &Physics::projectQueue[i].simPointer->action[j].lEBO);
                    glDeleteBuffers(1, &Physics::projectQueue[i].simPointer->action[j].VBO);
                }
            }
                ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
            SDL_GL_DestroyContext(glContext);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }


    

