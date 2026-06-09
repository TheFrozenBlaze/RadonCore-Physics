#include <iostream>
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include "opencllink.h"
#include "glmain.h"
#include "window.h"
#include "shader.h"

    
        
        void Window::Init() {
            if (SDL_Init(SDL_INIT_VIDEO) == false)
            {
                std::cout << SDL_GetError() << std::endl;
                exit(1);
            }
            else
            {
                std::cout << "Initialized" << std::endl;
            }

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            window = SDL_CreateWindow("Engine",
            1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

            if (window == nullptr)
            {
                std::cout << SDL_GetError() << std::endl;
                return;
            }
            glContext = SDL_GL_CreateContext(window);

            if(SDL_GL_MakeCurrent(window, glContext) != true) {
                std::cout << SDL_GetError() << std::endl;
            }
            if (glContext == nullptr)
            {
                std::cout << SDL_GetError() << std::endl;
            }
            if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
                std::cout << "glad couldn't initialize" << std::endl;
            }
        };
        void Window::Mainloop()
        {
            Shader shader("core/lib/shader/", "core/lib/shader/");
            bool running = true;
            SDL_Event e;
            //LoadOBJ("/home/matyas/Downloads/humanoid_quad.obj", 0);

            int w, h;
            //Data d = GetVector(0, 2, 0);

            while (running)
            {
                while (SDL_PollEvent(&e) == true)
                {
                    if (e.type == SDL_EVENT_QUIT)
                        running = false;
                }
                SDL_GetWindowSize(window,&w, &h);
                GL::Context(w, h);
                SDL_GL_SwapWindow(window);
            }
        };
        void Window::Clean()
        {
            SDL_GL_DestroyContext(glContext);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }


    

