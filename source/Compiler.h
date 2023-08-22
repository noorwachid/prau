#pragma once

#include "Common.h"
#include "yaml-cpp/yaml.h"

class Compiler
{
public:
	virtual string GetProgram() = 0;

	virtual string ComposeStandard(const string& standard) = 0;

	virtual string ComposeHeaderDirectory(const string& directory) = 0;

	virtual string ComposeObject(const string& objectFile) = 0;

	virtual string ComposeObjectFile(const string& sourceFile) = 0;

	virtual string ComposeLibrary(const string& libraryFile) = 0;

	virtual string ComposeLibraryFile(const string& name) = 0;

	virtual string ComposeExecutable(const string& executableFile) = 0;

	virtual string ComposeExecutableFile(const string& name) = 0;
};

class ClangCompiler : public Compiler
{
public:
	virtual string GetProgram() override;

	virtual string ComposeStandard(const string& standard) override;

	virtual string ComposeHeaderDirectory(const string& directory) override;

	virtual string ComposeObject(const string& objectFile) override;

	virtual string ComposeObjectFile(const string& sourceFile) override;

	virtual string ComposeLibrary(const string& libraryFile) override;

	virtual string ComposeLibraryFile(const string& name) override;

	virtual string ComposeExecutable(const string& executableFile) override;

	virtual string ComposeExecutableFile(const string& name) override;
};
