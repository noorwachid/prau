#include "compiler.h"
#include "platform.h"

string ClangCompiler::getProgram() {
	return "clang++";
}

string ClangCompiler::composeStandard(const string& standard) {
	return "-std=c++" + standard;
}

string ClangCompiler::composeHeaderDirectory(const string& directory) {
	return "-I" + directory;
}

string ClangCompiler::composeObject(const string& objectFile) {
	return "-o " + objectFile + " -c";
}

string ClangCompiler::composeObjectFile(const string& sourceFile) {
	return sourceFile + ".o";
}

string ClangCompiler::composeLibrary(const string& libraryFile) {
	return "-o " + libraryFile + " -dynamiclib";
}

string ClangCompiler::composeLibraryFile(const string& name) {
	return "lib" + name + ".dylib";
}

string ClangCompiler::composeExecutable(const string& executableFile) {
	return "-o " + executableFile;
}

string ClangCompiler::composeExecutableFile(const string& name) {
	return name;
}

string ClangCompiler::composeMode(const string& mode) {
	if (mode == "release") {
		return "-O2 -Wall";
	}
	
	if (mode == "debug") {
		return "-O0 -g -Wall";
	}

	return "";
}
