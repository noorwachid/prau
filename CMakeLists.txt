cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)

project(prau)

add_executable(prau 
	source/Text.cpp
	source/Platform.cpp
	source/Project.cpp
	source/ProjectGraph.cpp
	source/Compiler.cpp
	source/Builder.cpp
	source/Starter.cpp
	source/EntryPoint.cpp
)

add_subdirectory(dependency/yaml)

target_link_libraries(prau yaml-cpp)
