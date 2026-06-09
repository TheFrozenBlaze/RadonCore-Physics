cmake_minimum_required(VERSION 3.16)
project(Radon VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Radon
	src/main.cpp
	src/3dvfunc.cpp
	src/cl/opencl.c

)

target_include_directories(Radon PRIVATE
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/lib
	${CMAKE_SOURCE_DIR}/include/CL
)


