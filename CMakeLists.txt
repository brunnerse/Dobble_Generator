cmake_minimum_required(VERSION 3.12)


set(CMAKE_CXX_STANDARD 17)
add_compile_options("-Wall")

add_compile_options(-DDEBUG)

project(DobbleGenerator CXX)

add_executable(generator
        src/main.cpp
)

target_link_libraries(generator
	dobble
)


add_library(dobble 
	src/Dobble.cpp  
	src/DobbleGenerator.cpp
)

target_include_directories(dobble PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_compile_options(dobble PRIVATE "-DDEBUG")
