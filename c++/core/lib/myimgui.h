#ifndef MYIMGUI_H
#define MYIMGUI_H

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <imconfig.h>
#include <imstb_rectpack.h>
#include <imstb_textedit.h>
#include <imstb_truetype.h>
#include <SDL3/SDL.h>
#include <ctime>

#include "misc.h"
#include "glmain.h"
#include "ImGuiFileDialog.h"
#include "psettings.h"
using json = nlohmann::json;

namespace GUI {
	int SelectProjectPopup(bool& close);
	void GUIInit(SDL_Window *context);
	void MainMenu();
	int SimDetailWindow();
	void SimulationResults(std::unique_ptr<MyCL::Basic>& demand);
	extern bool blockBasicWindowInput;
	extern bool showSimulationResults;
	extern size_t lastSim;
	extern json spec;
}

#endif