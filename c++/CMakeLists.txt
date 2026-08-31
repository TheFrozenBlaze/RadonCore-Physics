cmake_minimum_required(VERSION 3.16)
project(Radon VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
	include(FetchContent)

	set(FETCHCONTENT_QUIET OFF)
	set(SDL_SHARED ON CACHE BOOL "" FORCE)
	set(SDL_STATIC OFF CACHE BOOL "" FORCE)
	set(SDL_TESTS OFF CACHE BOOL "" FORCE)
	set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
	set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SDL_GPU OFF CACHE BOOL "" FORCE)       
    set(SDL_DIALOG OFF CACHE BOOL "" FORCE)    
    set(SDL_CAMERA OFF CACHE BOOL "" FORCE)    
    set(SDL_SENSOR OFF CACHE BOOL "" FORCE)
    set(SDL_VULKAN OFF CACHE BOOL "" FORCE)
    set(SDL_METAL OFF CACHE BOOL "" FORCE)         
    set(SDL_RENDER_METAL OFF CACHE BOOL "" FORCE) 
    set(SDL_DIRECTX OFF CACHE BOOL "" FORCE)
    set(SDL_RENDER_D3D OFF CACHE BOOL "" FORCE)
    set(SDL_RENDER_D3D11 OFF CACHE BOOL "" FORCE)
    set(SDL_RENDER_D3D12 OFF CACHE BOOL "" FORCE)
    set(SDL_OPENGLES OFF CACHE BOOL "" FORCE)      
    set(SDL_OPENGL ON CACHE BOOL "" FORCE)         
        
	FetchContent_Declare (
		SDL3
		GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
		GIT_TAG release-3.2.18
		GIT_SHALLOW TRUE
		GIT_PROGRESS TRUE
	)
	FetchContent_MakeAvailable(SDL3)
endif()

find_package(OpenGL REQUIRED)
find_package(OpenCL REQUIRED)

if(OpenCL_FOUND)
else()
    message(STATUS "OpenCL not found")
endif()

add_executable(Radon
    core/frontend_src/main.cpp
    core/frontend_src/window.cpp
    core/frontend_src/glmain.cpp
    core/frontend_src/glad.c
    core/frontend_src/myimgui.cpp
    core/backend_src/physics.cpp
    core/backend_src/cl/OpenCLInit.cpp
    core/backend_src/cl/BVH.cpp
    core/backend_src/cl/ForceApply.cpp
    core/include/ImGUI/imgui.cpp
    core/include/ImGUI/imgui_draw.cpp
    core/include/ImGUI/imgui_tables.cpp
    core/include/ImGUI/imgui_widgets.cpp
    core/include/ImGUI/backends/imgui_impl_sdl3.cpp
    core/include/ImGUI/backends/imgui_impl_opengl3.cpp
    core/include/ImGuiFileDialog.cpp
)

target_include_directories(Radon PRIVATE
    ${CMAKE_SOURCE_DIR}/core/lib
    ${CMAKE_SOURCE_DIR}/core/include
    ${CMAKE_SOURCE_DIR}/core/include/ImGUI
    ${CMAKE_SOURCE_DIR}/settings
)

target_link_libraries(Radon PRIVATE 
    SDL3::SDL3
    OpenGL::GL
    OpenCL::OpenCL
)

target_compile_definitions(Radon PRIVATE
    RADON_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_compile_options(Radon PRIVATE -g -O0)
