#include "myimgui.h"

bool showCustomizationWindow = {false};
bool drawbodskel = {true};
bool drawgrid = {true};
int selected = {-1};
void GUI::GUIInit(SDL_Window *context)
{
    ImGuiContext* ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);
    ImGui_ImplSDL3_InitForOpenGL(context, SDL_GL_GetCurrentContext());
    ImGui_ImplOpenGL3_Init("#version 330");
};
void GUI::MainMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("Project")) {
            if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                IGFD::FileDialogConfig config;
                config.countSelectionMax = 0;

                ImGuiFileDialog::Instance()->OpenDialog(
                    "OpenMeshes",
                    "Choose Meshes",
                    ".obj,.stl",
                    config
                );
            }
            if (ImGui::MenuItem("Export", "Ctrl+E"))
            {
                
            }
            if(ImGui::MenuItem("New project")) {
                
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Settings")) {
            if(ImGui::MenuItem("Graphics")) {

            }
            if(ImGui::MenuItem("Memory")) {

            }
            if(ImGui::MenuItem("Hardware misc")) {

            }
            if(ImGui::MenuItem("Customization")) {
            showCustomizationWindow = true;

            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Simulate")) {
            if(ImGui::MenuItem("Setup")) {

            }
            if(ImGui::MenuItem("Run")) {
            }
            if(ImGui::MenuItem("Stop")) {

            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Mesh")) {
            if(ImGui::MenuItem("Mesh abstractor")) {

            }
            if(ImGui::MenuItem("Mesh tree")) {

            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Materials")) {
            if(ImGui::MenuItem("Add material")) {

            }
            if(ImGui::MenuItem("View material properties")) {

            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
        ImGui::SetNextWindowPos(ImVec2(0, 20));
        ImGui::SetNextWindowSize(ImVec2(GL::gWindowWidth, GL::gWindowHeight-20));
        if (showCustomizationWindow)
        {
            ImGui::Begin("Customization", &showCustomizationWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

            if(ImGui::Button("Dark Theme")) {
                ImGui::StyleColorsDark();
            }
            if(ImGui::Button("Light Theme")) {
                ImGui::StyleColorsLight();
            }

            ImGui::Checkbox("Show Grid", &drawgrid);

            ImGui::End();
        }
        
            
    }
};
/*void imguiIO() {
    ImGuiIO& IO = ImGui::GetIO();
    (void)IO;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}*/
void GUI::ProjectCast(ProjectDef& def) {
    if(ImGui::TreeNode(def.name.c_str())) {
        if(def.simPointer != nullptr) {
            for (size_t i{}; i < def.simPointer->action.size(); i++)
            {
            if (ImGui::TreeNode(def.simPointer->action[i].cs.name.c_str())) {
                if(ImGui::Selectable("Material")) {

                }
            }
            }
        }
        if(ImGui::TreeNode("Sim. specs")) {
            if(ImGui::Selectable("Project json")) {

            }
        }
        ImGui::TreePop();
    }
};
void GUI::SimDetailWindow() {
    ImGui::SetNextWindowPos(ImVec2(0,20));
    ImGui::SetNextWindowSize(ImVec2(GL::gWindowWidth/6, GL::gWindowHeight-20));
    ImGui::Begin("Simulations");
    for (size_t i{}; i < Physics::projectQueue.size(); i++) {
        GUI::ProjectCast(Physics::projectQueue[i]);
    }
    ImGui::End();
};
int GUI::SelectProjectPopup()
{
    ImGui::SetNextWindowSize(ImVec2(250, 300));
    ImGui::SetNextWindowPos(ImVec2((GL::gWindowWidth/2)-125, GL::gWindowHeight/2 -150));
    if (ImGui::BeginPopupModal("Import meshes", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    {
        ImGui::Text("Choose target project");
        ImGui::BeginChild("ProjectList", ImVec2(250, 300), true);
        for (size_t i{}; i < Physics::projectQueue.size(); i++)
        {
            if (ImGui::Selectable(Physics::projectQueue[i].name.c_str(), selected == i))
            {
                selected = i;
            }
        }
        ImGui::EndChild();
        if (ImGui::Button("Select"))
        {
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            const int retval = selected;
            selected = -1;
            std::cout << retval << std::endl;
            return retval;
        }
        ImGui::EndPopup();
    }
        return -2;
};
