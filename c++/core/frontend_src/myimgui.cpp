#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include "myimgui.h"

void GUIInit() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
};
void MainMenu() {
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open...", "Ctrl+O")) {

            }
            if(ImGui::MenuItem("Export")) {

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
        if(ImGui::)
        ImGui::EndMainMenuBar();
    }
};
