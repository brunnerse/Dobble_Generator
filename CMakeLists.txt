cmake_minimum_required(VERSION 3.12)


set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wconversion)

add_compile_definitions("DEBUG")

set(CMAKE_VERBOSE_MAKEFILE false)

project(DobbleGenerator CXX)

add_executable(generator
        src/main.cpp
)

target_link_libraries(generator
	dobble
)


add_library(dobble 
	src/Dobble.cpp  
	src/Generator/IterativeGenerator.cpp
	src/Generator/FillupGenerator.cpp
	src/Generator/PermutationGenerator.cpp
)

target_include_directories(dobble PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/Generator
)


