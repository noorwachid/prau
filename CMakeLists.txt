cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)

project(prau)

add_executable(prau 
	source/text.cpp
	source/platform.cpp
	source/project.cpp
	source/projectgraph.cpp
	source/compiler.cpp
	source/builder.cpp
	source/starter.cpp
	source/main.cpp
)

add_subdirectory(dependency/yaml)

target_link_libraries(prau yaml-cpp)
