#pragma once

#include "Compiler.h"
#include "ProjectGraph.h"
#include "Term.h"

class BuilderCache
{
public:
	BuilderCache(Project& project, ProjectSource& projectSource);

	void Invalidate();

	void Validate();

	bool IsValid() const;

	vector<string> GetInvalidSources() const;

private:
	bool IsSourceFile(const string& source) const;

	string GetFile() const;

	void AddInvalidSource(const string& source);

private:
	bool _valid = false;
	Project& _project;
	ProjectSource& _projectSource;
	unordered_set<string> _invalidSources;
};

class Builder
{
public:
	bool verbose = true;

	Builder(Compiler& compiler, Project& project);

	void Build();

	void Run();

	void Clean();

	string ComposeStandard() const;

	string ComposeHeaderDirectory(const string& directory) const;

	string ComposeLibraryDirectory(const string& directory) const;

	string ComposeObjectFile(const string& file) const;

	string ComposeLibraryFile(const string& file) const;

	string ComposeExecutableFile(const string& file) const;

	string ComposeObjectBuilder(const string& file) const;

	string ComposeLibraryBuilder(const string& file) const;

	string ComposeExecutableBuilder(const string& file) const;

private:
	bool Compile(const string& flags, const vector<string>& sources);

	bool Link(const string& flags);

private:
	Compiler& _compiler;
	Project& _project;
	ProjectSource _projectSource;
	BuilderCache _cache;
};
