#pragma once

#include "../compiler.h"

class GCCCompiler : public Compiler {
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
