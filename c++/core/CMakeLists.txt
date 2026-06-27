cmake_minimum_required(VERSION 3.16)
project(Radon VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(Radon
	#ackend_src/main.cpp
	#backend_src/cl/opencl.c
	#frontend_src/volk.c
	frontend_src/main.cpp
	frontend_src/window.cpp
	frontend_src/glmain.cpp
	frontend_src/glad.c
)

target_include_directories(Radon PRIVATE
	${CMAKE_SOURCE_DIR}/lib
	${CMAKE_SOURCE_DIR}/include

)
find_package(SDL3 QUIET)

if(NOT SDL3_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		SDL3
		GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
		GIT_TAG release-3.2.18
	)

	FetchContent_MakeAvaileable(SDL3)
	endif()
target_link_libraries(Radon PRIVATE SDL3::SDL3)


