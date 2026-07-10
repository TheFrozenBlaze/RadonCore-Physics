cmake_minimum_required(VERSION 3.16)
project(Radon VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(Radon
	#backend_src/main.cpp
	#backend_src/cl/opencl.c
	#frontend_src/volk.c
	frontend_src/main.cpp
	frontend_src/window.cpp
	frontend_src/glmain.cpp
	frontend_src/glad.c
	include/ImGUI/imgui.cpp
    include/ImGUI/imgui_draw.cpp
    include/ImGUI/imgui_tables.cpp
    include/ImGUI/imgui_widgets.cpp
    include/ImGUI/backends/imgui_impl_sdl3.cpp
    include/ImGUI/backends/imgui_impl_opengl3.cpp
)

target_include_directories(Radon PRIVATE
	${CMAKE_SOURCE_DIR}/lib
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/ImGUI/
    ${CMAKE_SOURCE_DIR}/include/ImGUI/backends

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

find_package(OpenCL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Vulkan QUIET)


target_link_libraries(Radon PRIVATE 
					  SDL3::SDL3
					  OpenCL::OpenCL
					  OpenGL::GL
					  Vulkan::Vulkan
					  )



