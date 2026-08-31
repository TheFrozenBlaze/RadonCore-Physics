#include "myimgui.h"
using json = nlohmann::json;


bool showCustomizationWindow {false};
bool drawbodskel {true};
bool drawgrid {true};
bool showSetupWindow {false};
int selected{-1};
bool GUI::blockBasicWindowInput {false};
bool GUI::showSimulationResults{false};
json GUI::spec;
size_t GUI::lastSim{};
void GUI::GUIInit(SDL_Window *context)
{
	ImGuiContext* ctx = ImGui::CreateContext();
	ImGui::SetCurrentContext(ctx);
	ImGui_ImplSDL3_InitForOpenGL(context, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGuiIO& IO = ImGui::GetIO();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
};
void GUI::MainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if(ImGui::BeginMenu("Project")) {
			if(ImGui::MenuItem("New project")) {
				std::srand(std::time(0));
				std::unique_ptr<SimDef> newElement =  std::make_unique<SimDef>();
				time_t dtime = std::time(NULL);
				std::tm* local = std::localtime(&dtime);
				std::stringstream tss;
				tss << std::put_time(local, "%Y%m%d");
				std::filesystem::path newPath = RADON_SOURCE_DIR / std::filesystem::path("cached/Project" + std::to_string(Physics::projectQueue.size()+1) + "_" + tss.str() + "_" + std::to_string(std::rand() % 1000001));
				std::filesystem::create_directory(newPath);
				std::filesystem::create_directories(newPath / std::filesystem::path("simulations"));
				Physics::projectQueue.emplace_back(ProjectDef{newPath, std::move(newElement), "Project" + std::to_string(Physics::projectQueue.size()+1)});
				//std::cout << "current time: " <<  tss.str() << std::endl;
				std::ofstream file(newPath / "project.json");
				tss.clear();
				tss << std::put_time(local, "%Y-%m-%d %H:%M:%S");
				json pj = 
				{
					{"creation", tss.str()},
					{"last_edit", tss.str()},
					{"participants", {""}}
				};
				file << pj;
				file.close();
			}
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
				showSetupWindow = true;
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
	if(showSetupWindow) 
	{
		
		
		GUI::blockBasicWindowInput = true;
		static double mslength{};
		static int steps{};
		static float expressed {};
		static int selectedProj {-1};
		static float timeframe[2];
		static float vectorstp[3];
		static float vector[3];
		static bool single_tick {false};
		static uint tick {0};
		static std::unique_ptr<Rays> ray = std::make_unique<Rays>();
		ImGui::SetNextWindowPos(ImVec2(0, 20));
		ImGui::SetNextWindowSize(ImVec2(GL::gWindowWidth, GL::gWindowHeight-20));
		ImGui::Begin("Setup simulation", &showSetupWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImGui::SetNextWindowSize(ImVec2(250, 300));
		ImGui::SetNextWindowPos(ImVec2((GL::gWindowWidth/2)-125, GL::gWindowHeight/2 -150));
		if (ImGui::BeginPopupModal("ErrorAtProjects", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
		{

			//std::cout << "entered" << std::endl;
			ImGui::Text("You haven't selected any projects!");
			if(ImGui::Button("Understood")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::SetNextWindowSize(ImVec2(250, 300));
		ImGui::SetNextWindowPos(ImVec2((GL::gWindowWidth/2)-125, GL::gWindowHeight/2 -150));
		if (ImGui::BeginPopupModal("ErrorAtMs", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
		{
			//std::cout << "entered" << std::endl;
			ImGui::Text("Time length invalid!");
			if(ImGui::Button("Understood")) 
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SetNextWindowPos(ImVec2(20, 40));
		ImGui::SetNextWindowSize(ImVec2(160, 220));
		if(ImGui::BeginCombo("Projects", (selectedProj >= 0 ? Physics::projectQueue[selectedProj].name.c_str() : "-- Select project --"))) 
		{
			for (size_t i {}; i < Physics::projectQueue.size(); i++)
			{
				bool selectedNow = selectedProj == static_cast<int>(i);
				if (ImGui::Selectable(
						Physics::projectQueue[i].name.c_str(),
						selectedNow))
				{
					selectedProj = static_cast<int>(i);
				}
				if (selectedNow) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		

		if(ImGui::InputDouble("Simulation length", &mslength));
		ImGui::SameLine();
		ImGui::Text("ms (max 5000)");
		if(ImGui::InputInt("Steps (throughout the entire sim)", &steps))
		ImGui::SameLine();
		ImGui::Text("optimal: 500-1000 per sec");
		
		if(mslength != 0.f && steps != 0) {
			if(ImGui::Checkbox("Single tick action", &single_tick));
			if(ImGui::InputFloat3("Add starting point", vectorstp));
			if(ImGui::InputFloat3("Add direction", vector));
			if(ImGui::InputFloat("Expressed force", &expressed));
			if(!single_tick) 
			{
			if(ImGui::InputFloat2("Timeframe of activeness (ms-ms)", timeframe));
			tick = 0;
			}
			else
			{
			if(ImGui::InputScalar("Active tick(1 - end), 0 = disable", ImGuiDataType_U64, &tick));
			timeframe[0] = 0.f;
			timeframe[1] = 0.f;
			}
			auto now = std::chrono::system_clock::now();
			auto in_time_t = std::chrono::system_clock::to_time_t(now);

			std::stringstream ss;
			ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
			if(ImGui::Button("Add to current sim")) 
			{
				std::cout << "selected proj " << selectedProj << std::endl;

				//std::cout << "stp" << ray->stpxcoords.back() << " " << ray->stpycoords.back() << " " << ray->stpzcoords.back() << std::endl; 
				if(mslength / 1000 * steps < tick) 
				{
					std::cout << "Activation step out of bounds" << std::endl;
					tick = 0;
				}
				else
				{
					std::cout << "tick " << tick << std::endl;
					ray->xcoords.push_back(vector[0]);
					ray->ycoords.push_back(vector[1]);
					ray->zcoords.push_back(vector[2]);
					std::cout << "pushed back vector" << std::endl;
					ray->stpxcoords.push_back(vectorstp[0]);
					ray->stpycoords.push_back(vectorstp[1]);
					ray->stpzcoords.push_back(vectorstp[2]);
					std::cout << "pushed back vectorstp" << std::endl;
					ray->expressed_force.push_back(expressed);
					std::cout << "pushed back expressed" << std::endl;
					ray->tfoa.emplace_back(std::pair<float, float>{timeframe[0], timeframe[1]});
					ray->single_tick.push_back(tick);
					std::cout << "pushed back timing" << std::endl;
					vector[0] = 0.f;
					vector[1] = 0.f;
					vector[2] = 0.f;
					vectorstp[0] = 0.f;
					vectorstp[1] = 0.f;
					vectorstp[2] = 0.f;
					expressed = 0.f;
					timeframe[0] = 0.f;
					timeframe[1] = 0.f;
					tick = 0;
				}
			}
		}
		if(ImGui::Button("Cast simulation")) {	
			if ((mslength >= 5001) || (mslength == 0.f)) {
				ImGui::OpenPopup("ErrorAtMs");
				ImGui::End();
				return;

			} 
			else if (selectedProj < 0 || selectedProj >= static_cast<int>(Physics::projectQueue.size())) {
				ImGui::OpenPopup("ErrorAtProjects");
				std::cout << "No projects executed" << std::endl;
				ImGui::End();
				GUI::blockBasicWindowInput = false;
				return;
			} 
			else if (!Physics::projectQueue[selectedProj].simPointer) {
				std::cerr << "Error: simPointer is uninitialized/null!" << std::endl;
			} 
			else {
				std::cout << "queue size " << Physics::projectQueue.size() << std::endl;
				Physics::projectQueue[selectedProj].simPointer->tiles = steps;
				Physics::projectQueue[selectedProj].simPointer->simdur = 
				static_cast<std::chrono::duration<double, std::milli>>(mslength);
				time_t dtime = std::time(NULL);
				std::tm* local = std::localtime(&dtime);
				std::stringstream tss;
				tss << std::put_time(local, "%Y-%m-%d %X");
				std::ofstream sim(Physics::projectQueue[selectedProj].projPath / std::filesystem::path("simulations") / "sim.json");
				nlohmann::ordered_json sj;
				sj["creation_date"] = tss.str();
				sj["length"] = mslength;
				sj["steps"] = steps;
				std::cout << "past basics" << std::endl;
				for (size_t i{}; i < ray->stpxcoords.size(); i++)
				{
					sj["ray" + std::to_string(i)]["start"] = {ray->stpxcoords[i], ray->stpycoords[i], ray->stpzcoords[i]};
					sj["ray" + std::to_string(i)]["end"] = {ray->xcoords[i], ray->ycoords[i], ray->zcoords[i]};
					sj["ray" + std::to_string(i)]["frame"] = {ray->tfoa[i].first, ray->tfoa[i].second};
					sj["ray" + std::to_string(i)]["tick"] = ray->single_tick[i];
					sj["ray" + std::to_string(i)]["expressed"] = ray->expressed_force[i];
					//std::cout << "entered and made through the cycle" << std::endl;
				}
				std::cout << "past ray" << std::endl;
					for (size_t i{}; i < Physics::projectQueue[selectedProj].simPointer->action.size(); i++)
					{
						Physics::projectQueue[selectedProj].simPointer->action[i].start = 0;
						Physics::projectQueue[selectedProj].simPointer->action[i].finish = steps - 1;
					}
					std::cout << "added active time" << std::endl;
					sim << sj.dump(4);
					std::cout << "dumped json" << std::endl;
					steps = 0;
					mslength = 0.f;
					selectedProj = -1;
					ray = std::make_unique<Rays>();
					GUI::blockBasicWindowInput = false;
			}
			
		}
	ImGui::End();
	}

};

int GUI::SimDetailWindow() {
	static bool openMaterial {false};
	static bool loaded {false};
	static json jl;
	static int selectedProject {-1};
	GUI::blockBasicWindowInput = false;
	ImGui::SetNextWindowPos(ImVec2(0,20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(GL::gWindowWidth/6, GL::gWindowHeight-20), ImGuiCond_FirstUseEver);
	
	ImGui::Begin("Simulations", nullptr, ImGuiWindowFlags_NoCollapse);
	//std::cout << "projectq size " << Physics::projectQueue.size() << std::endl;
	//std::cout << "selected: " << selectedProject << std::endl;
	for (size_t i{}; i < Physics::projectQueue.size(); i++)
	{
		bool isSelected = selectedProject == static_cast<int>(i);
		
		if (ImGui::TreeNodeEx(Physics::projectQueue[i].name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (isSelected ? ImGuiTreeNodeFlags_Selected : 0)))
		{
			if (ImGui::IsItemClicked())
			{
				selectedProject = static_cast<int>(i);
				//std::cout << "current selected project " << selectedProject << std::endl;
				// std::cout << "entered click" << std::endl;
			}
			//std::cout << "drawing tree" << std::endl;/ 1000.0f
			if (Physics::projectQueue[i].simPointer != nullptr)
			{
				for (size_t j{}; j < Physics::projectQueue[i].simPointer->action.size(); j++)
				{
					if (ImGui::TreeNode(Physics::projectQueue[i].simPointer->action[j].cs->name.c_str()))
					{
						if (ImGui::Button("Material"))
						{
							openMaterial = true;
							ImGui::OpenPopup("Select material");
						}
							ImGui::SetNextWindowSize(ImVec2(160, 180));
							ImGui::SetNextWindowPos(ImVec2((GL::gWindowWidth/2)-80, GL::gWindowHeight/2 -90));
							ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
							if(ImGui::BeginPopupModal("Select material", &openMaterial, flags)) 
							{
								GUI::blockBasicWindowInput = true;

								char search[128] = "";
								ImGui::InputTextWithHint(
									"##MaterialSearch",
									"Search materials...",
									search,
									sizeof(search)
								);
								std::filesystem::path path = RADON_SOURCE_DIR / std::filesystem::path("mats/json-style/list.json");

								if(!loaded) {
									//std::cout << "path: " << path << std::endl;
									std::ifstream l(path);

									if (!l)
									{
										std::cerr << "Failed to open material list: " << std::filesystem::absolute(path) << '\n';
									}
									jl = json::parse(l);
									loaded = true;
								}
								
										
								ImGui::BeginChild("MaterialList", ImVec2(130, 140), true);
									
									size_t jsoncr{};
									while(true) {
										if(!jl.contains(std::to_string(jsoncr))) break;
										std::string value = jl.at(std::to_string(jsoncr)).get<std::string>();
										if(ImGui::Selectable(value.c_str(), selected == jsoncr, ImGuiSelectableFlags_AllowDoubleClick))
										{
											selected = jsoncr;
											if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
											{
												path = RADON_SOURCE_DIR / std::filesystem::path("mats/json-style") / std::filesystem::path(jl.at(std::to_string(jsoncr)).get<std::string>() + ".json");
												std::ifstream mj(path);
												json m = json::parse(mj);
												std::unique_ptr<Material> mat = std::make_unique<Material>();
												mat->density = m.at("density").get<uint>();
												mat->tconduct = m.at("thermal_cond").get<uint>();
												Physics::projectQueue[i].simPointer->action[j].bodmat = std::move(mat);
												GUI::blockBasicWindowInput = false;
												ImGui::EndChild();
												ImGui::CloseCurrentPopup();
												ImGui::EndPopup();
												selected = -1;
												loaded = false;
											}
										}
										jsoncr++;
										
									}
								ImGui::EndChild();
								if(ImGui::Button("Select")) 
								{
									path = RADON_SOURCE_DIR / std::filesystem::path("mats/json-style") / std::filesystem::path(jl.at(std::to_string(jsoncr)).get<std::string>() + ".json");
									std::ifstream mj(path);
									json m = json::parse(mj);
									std::unique_ptr<Material> mat = std::make_unique<Material>();
									mat->density = m.at("density").get<uint>();
									mat->tconduct = m.at("thermal_cond").get<uint>();
									Physics::projectQueue[i].simPointer->action[j].bodmat = std::move(mat);
									GUI::blockBasicWindowInput = false;
									ImGui::CloseCurrentPopup();
									ImGui::EndPopup();
									selected = -1;
									loaded = false;
								}

								ImGui::EndPopup();
							}
						ImGui::TreePop();
					}
				}
			}
			if (ImGui::TreeNode("Sim. specs"))
			{
				if (ImGui::Selectable("Project json"))
				{
				}
				if(std::filesystem::exists(Physics::projectQueue[i].projPath / std::filesystem::path("simulations/sim.json")))
				{
					if(ImGui::Button("Run simulation"))
					{

						Physics::SimulationCollect(selectedProject);
						//std::cout << "finished sim" << std::endl;
						GUI::showSimulationResults = true;
						std::ifstream sim(Physics::projectQueue[i].projPath / std::filesystem::path("simulations/sim.json"));
						GUI::spec = json::parse(sim);
						//std::cout << "parsed sim " << std::endl;
						GUI::lastSim = selectedProject;
						//std::cout << "last sim " << GUI::lastSim << std::endl;
					}
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		
	}

	ImGui::End();
	return selectedProject;
	
};


int GUI::SelectProjectPopup(bool& close)
{

	ImGui::SetNextWindowSize(ImVec2(250, 300));
	ImGui::SetNextWindowPos(ImVec2((GL::gWindowWidth/2)-125, GL::gWindowHeight/2 -150));
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	if (ImGui::BeginPopupModal("Import meshes",&close, flags))
	{
		ImGui::Text("Choose target project");
		ImGui::BeginChild("ProjectList", ImVec2(250, 300), true);
		for (size_t i{}; i < Physics::projectQueue.size(); i++)
		{
			if (ImGui::Selectable(Physics::projectQueue[i].name.c_str(), selected == i, ImGuiSelectableFlags_AllowDoubleClick))
			{
				selected = i;
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
				{
					GUI::blockBasicWindowInput = false;
					ImGui::CloseCurrentPopup();
					ImGui::EndChild();
					ImGui::EndPopup();
					const int retval = selected;
					selected = -1;
					return retval;
				}
			}
		}
		ImGui::EndChild();
		if (ImGui::Button("Select"))
		{
			GUI::blockBasicWindowInput = false;
			ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
			const int retval = selected;
			selected = -1;
			//std::cout << retval << std::endl;
			return retval;
		}
		ImGui::EndPopup();
	}
		return -2;
};

void GUI::SimulationResults(std::unique_ptr<MyCL::Basic>& demand)
{
	static int tileCount {1};
	static size_t currentTile{1};
	static size_t previousTile{1};
	ImGui::SetNextWindowPos(ImVec2(GL::gWindowWidth / 2 - 60,20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(120, 80), ImGuiCond_FirstUseEver);
	if(ImGui::Begin("Simulation results", &GUI::showSimulationResults, ImGuiWindowFlags_NoCollapse))
	{
		//std::cout << "window has begun" << std::endl;
		ImGui::SliderInt("Iteration", &tileCount, 1, Physics::projectQueue[GUI::lastSim].simPointer->tiles);
		currentTile = static_cast<float>(tileCount);
		static size_t j{};

		if(currentTile != previousTile)
		{
			bool running{true};
			while(running)
			{
				//std::cout << "entered running phase" << std::endl;
				if (GUI::spec["Initial"].contains("Body" + std::to_string(j)))
				{
					//std::cout << "I have found body" << j << std::endl;
					//std::cout << "Tile: " << currentTile - 1 << std::endl;
					std::vector<std::array<float, 3>> displace;
					displace.emplace_back(std::array<float, 3>{GUI::spec["tick" + std::to_string(currentTile - 1)]["Body" + std::to_string(j)]["COM"][0].get<float>() - GUI::spec["tick" + std::to_string(previousTile - 1)]["Body" + std::to_string(j)]["COM"][0].get<float>(),
					GUI::spec["tick" + std::to_string(currentTile - 1)]["Body" + std::to_string(j)]["COM"][1].get<float>() - GUI::spec["tick" + std::to_string(previousTile - 1)]["Body" + std::to_string(j)]["COM"][1].get<float>(),
					GUI::spec["tick" + std::to_string(currentTile - 1)]["Body" + std::to_string(j)]["COM"][2].get<float>() - GUI::spec["tick" + std::to_string(previousTile - 1)]["Body" + std::to_string(j)]["COM"][2].get<float>()});
					//std::cout << "displace done" << std::endl;
					MyCL::ForceApply(Physics::projectQueue[GUI::lastSim].simPointer->action[j].cs, displace, demand);

					glBindBuffer(GL_ARRAY_BUFFER, Physics::projectQueue[GUI::lastSim].simPointer->action[j].VBO);
					glBufferSubData(GL_ARRAY_BUFFER, 0, Physics::projectQueue[GUI::lastSim].simPointer->action[j].cs->vpc.size() * sizeof(float), Physics::projectQueue[GUI::lastSim].simPointer->action[j].cs->vpc.data());
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					/*for (size_t i{}; i < Physics::projectQueue[GUI::lastSim].simPointer->action[j].cs->vpc.size() / 3; i++)
					{
						std::cout << "coords" << Physics::projectQueue[GUI::lastSim].simPointer->action[j].cs->vpc[i * 3] << " " << Physics::projectQueue[GUI::lastSim].simPointer->action[j].cs->vpc[i * 3 + 1] << " " << Physics::projectQueue[GUI::lastSim].simPointer->action[j].cs->vpc[i * 3 + 2] << " " << std::endl;
					}*/
					j++;
				}
				else
				{
					j = 0;
					running = false;
					//std::cout << "new tile" << std::endl;
				}
			}
			
		}
		previousTile = currentTile;
		ImGui::End();
	}
	
};