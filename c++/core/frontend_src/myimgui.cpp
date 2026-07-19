#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include "myimgui.h"
#include "3dvfunc.h"
#include "misc.h"
#include "glmain.h"
#include "physics.h"
#include "tinyfiledialogs.h"

void GUIInit() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
};
void MainMenu()
{
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                const char *filters[] = {"*.obj", "*.stl", "*.*"};
                const char *file = tinyfd_openFileDialog("Open File", "", 2, filters, NULL, 0);
                objIdent obj;
                if(!obj.objReader(file)) {
                    tinyfd_messageBox("Error", "Wrong file format or couldn't open file(supported formats: wavefront obj, stl)", "Ok", "Error", 1);
                }
            }
            if (ImGui::MenuItem("Export", "Ctrl+E"))
            {
                
            }
        }
        if(ImGui::BeginMenu("Settings")) {
            if(ImGui::MenuItem("Graphics")) {

            }
            if(ImGui::MenuItem("Memory")) {

            }
            if(ImGui::MenuItem("Hardware misc")) {

            }
            if(ImGui::MenuItem("Customization")) {

            }
        }
        if(ImGui::BeginMenu("Simulate")) {
            if(ImGui::MenuItem("Run")) {
                Physics::StartSim();
            }
            if(ImGui::MenuItem("Simulation depth")) {
                
            }
            if(ImGui::MenuItem("Layers")) {

            }
            if(ImGui::MenuItem("Stop")) {

            }
        }
        if(ImGui::BeginMenu("Mesh")) {
            if(ImGui::MenuItem("Mesh abstractor")) {

            }
            if(ImGui::MenuItem("Mesh tree")) {}
        }
        if(ImGui::BeginMenu("Materials")) {
            if(ImGui::MenuItem("Add material")) {

            }
            if(ImGui::MenuItem("Set material"))
            {

            }
            if(ImGui::MenuItem("View material properties")) {

            }
        }
        ImGui::EndMainMenuBar();
        ImGui::SetNextWindowPos(ImVec2(0, 20));
        ImGui::SetNextWindowSize(ImVec2(GL::gWindowWidth, GL::gWindowHeight-20));
    }
};
void imguiIO() {
    ImGuiIO& IO = ImGui::GetIO();
    bool ctrl = IO.KeyCtrl;
    if(ctrl && ImGui::IsKeyPressed(ImGuiKey_O)) {
        const char *filters[] = {"*.obj", "*.stl", "*.*"};
        const char *file = tinyfd_openFileDialog("Open File", "", 0, nullptr, NULL, 0);
        if (file)
        {
            objIdent obj;
            if(!obj.objReader(file)) {
                tinyfd_messageBox("Error", "Wrong file format or couldn't open file(supported formats: Wavefront obj, stl)", "Ok", "Error", 1);
            }
        }
    }
}
