cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)

project(prau)

add_executable(prau 
	source/main.cpp
	source/text.cpp
	source/platform.cpp
	source/project.cpp
	source/projectgraph.cpp
	source/builder.cpp
	source/starter.cpp
	source/workspace.cpp
	source/compiler/gcccompiler.cpp
	source/compiler/clangcompiler.cpp
	source/compiler/emscriptencompiler.cpp
)

add_subdirectory(dependency/yaml)

target_link_libraries(prau yaml-cpp)
