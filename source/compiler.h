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

class ClangCompiler : public Compiler {
public:
	virtual string getProgram() override;

	virtual string composeStandard(const string& standard) override;

	virtual string composeHeaderDirectory(const string& directory) override;

	virtual string composeObject(const string& objectFile) override;

	virtual string composeObjectFile(const string& sourceFile) override;

	virtual string composeLibrary(const string& libraryFile) override;

	virtual string composeLibraryFile(const string& name) override;

	virtual string composeExecutable(const string& executableFile) override;

	virtual string composeExecutableFile(const string& name) override;

	virtual string composeMode(const string& mode) override;
};
