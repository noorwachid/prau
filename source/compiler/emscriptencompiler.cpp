#include "emscriptencompiler.h"
#include <exception>

string EmscriptenCompiler::getProgram() {
	return "em++";
}

string EmscriptenCompiler::composeStandard(const string& standard) {
	return "-std=c++" + standard;
}

string EmscriptenCompiler::composeHeaderDirectory(const string& directory) {
	return "-I" + directory;
}

string EmscriptenCompiler::composeObject(const string& objectFile) {
	return "-o " + objectFile + " -c";
}

string EmscriptenCompiler::composeObjectFile(const string& sourceFile) {
	return sourceFile + ".o";
}

string EmscriptenCompiler::composeLibrary(const string& libraryFile) {
	return "-o " + libraryFile + " -dynamiclib";
}

string EmscriptenCompiler::composeLibraryFile(const string& name) {
	throw std::runtime_error("cannot create a library using emscripten");
}

string EmscriptenCompiler::composeExecutable(const string& executableFile) {
	return "-o " + executableFile + ".html";
}

string EmscriptenCompiler::composeExecutableFile(const string& name) {
	return name;
}

string EmscriptenCompiler::composeMode(const string& mode) {
	if (mode == "release") {
		return "-O2 -Wall";
	}
	
	if (mode == "debug") {
		return "-O0 -g -Wall";
	}

	return "";
}


