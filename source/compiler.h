#pragma once

#include "common.h"
#include "yaml-cpp/yaml.h"

class Compiler {
public:
	virtual string getProgram() = 0;

	virtual string composeStandard(const string& standard) = 0;

	virtual string composeHeaderDirectory(const string& directory) = 0;

	virtual string composeObject(const string& objectFile) = 0;

	virtual string composeObjectFile(const string& sourceFile) = 0;

	virtual string composeLibrary(const string& libraryFile) = 0;

	virtual string composeLibraryFile(const string& name) = 0;

	virtual string composeExecutable(const string& executableFile) = 0;

	virtual string composeExecutableFile(const string& name) = 0;

	virtual string composeMode(const string& mode) = 0;
};
