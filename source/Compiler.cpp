#include "Compiler.h"
#include "Platform.h"

string ClangCompiler::GetProgram()
{
	return "clang++";
}

string ClangCompiler::ComposeStandard(const string& standard)
{
	return "-std=c++" + standard;
}

string ClangCompiler::ComposeHeaderDirectory(const string& directory)
{
	return "-I" + directory;
}

string ClangCompiler::ComposeObject(const string& objectFile)
{
	return "-o " + objectFile + " -c";
}

string ClangCompiler::ComposeObjectFile(const string& sourceFile)
{
	return sourceFile + ".o";
}

string ClangCompiler::ComposeLibrary(const string& libraryFile)
{
	return "-o " + libraryFile + " -dynamiclib";
}

string ClangCompiler::ComposeLibraryFile(const string& name)
{
	return "lib" + name + ".dylib";
}

string ClangCompiler::ComposeExecutable(const string& executableFile)
{
	return "-o " + executableFile;
}

string ClangCompiler::ComposeExecutableFile(const string& name)
{
	return name;
}
