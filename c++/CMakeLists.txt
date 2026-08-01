cmake_minimum_required(VERSION 3.16)
project(Radon VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_executable(Radon
	#backend_src/main.cpp
	#backend_src/cl/opencl.c
	#frontend_src/volk.c
	core/frontend_src/main.cpp
	core/frontend_src/window.cpp
	core/frontend_src/glmain.cpp
	core/frontend_src/glad.c
	core/frontend_src/myimgui.cpp
	core/backend_src/physics.cpp
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
	${CMAKE_SOURCE_DIR}/core/include/SDL3/include
	${CMAKE_SOURCE_DIR}/core/include/ImGUI/
    ${CMAKE_SOURCE_DIR}/core/include/ImGUI/backends
	${CMAKE_SOURCE_DIR}/settings

)
find_package(SDL3 REQUIRED)

#if(NOT SDL3_FOUND)
#	include(FetchContent)
#	FetchContent_Declare(
#		SDL3
#		GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
#		GIT_TAG release-3.2.18
#	)

#	FetchContent_MakeAvailable(SDL3)
#	endif()

#find_package(OpenCL REQUIRED)
find_package(OpenGL REQUIRED)
#find_package(Vulkan QUIET)
#link_directories(${CMAKE_SOURCE_DIR}/include/SDL3/build/libSDL3.so)
#target_link_libraries(Radon PRIVATE ${CMAKE_SOURCE_DIR}/include/SDL3/build/libSDL3.so)

target_link_libraries(Radon PRIVATE 
					  SDL3::SDL3
					  #OpenCL::OpenCL
					  OpenGL::GL
					  #Vulkan::Vulkan
					  )
target_compile_options(Radon PRIVATE -fsanitize=address -g -O0)
target_link_options(Radon PRIVATE -fsanitize=address)



