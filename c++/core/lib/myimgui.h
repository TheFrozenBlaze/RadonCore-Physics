#ifndef MYIMGUI_H
#define MYIMGUI_H

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include "3dvfunc.h"
#include "misc.h"
#include "glmain.h"
#include "ImGuiFileDialog.h"
#include <SDL3/SDL.h>
#include "psettings.h"
#include "physics.h"


namespace GUI {
    int SelectProjectPopup();
    void GUIInit(SDL_Window *context);
    void MainMenu();
    void ProjectCast(ProjectDef& def);
    void SimDetailWindow();
}
#endif