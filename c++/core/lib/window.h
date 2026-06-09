#ifndef WINDOW_H
#define WINDOW_H
#include <SDL3/SDL.h>
class Window{
    private:
    SDL_Window *window;
    SDL_GLContext glContext;
    public:
        void Init();
        void Mainloop();
        void Clean();
};
#endif