#include "gcccompiler.h"

string GCCCompiler::getProgram() {
	return "g++";
}

string GCCCompiler::composeStandard(const string& standard) {
	return "-std=c++" + standard;
}

string GCCCompiler::composeHeaderDirectory(const string& directory) {
	return "-I" + directory;
}

string GCCCompiler::composeObject(const string& objectFile) {
	return "-o " + objectFile + " -c";
}

string GCCCompiler::composeObjectFile(const string& sourceFile) {
	return sourceFile + ".o";
}

string GCCCompiler::composeLibrary(const string& libraryFile) {
	return "-o " + libraryFile + " -dynamiclib";
}

string GCCCompiler::composeLibraryFile(const string& name) {
	return "lib" + name + ".dylib";
}

string GCCCompiler::composeExecutable(const string& executableFile) {
	return "-o " + executableFile;
}

string GCCCompiler::composeExecutableFile(const string& name) {
	return name;
}

string GCCCompiler::composeMode(const string& mode) {
	if (mode == "release") {
		return "-O2 -Wall";
	}
	
	if (mode == "debug") {
		return "-O0 -g -Wall";
	}

	return "";
}


