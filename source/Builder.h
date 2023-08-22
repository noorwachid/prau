#pragma once

#include "Compiler.h"
#include "ProjectGraph.h"
#include "Term.h"

class BuilderCache
{
public:
	BuilderCache(Project& project, ProjectGraph& projectSource);

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
	ProjectGraph& _projectSource;
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

private:
	bool Compile(const string& flags, const vector<string>& sources);

	bool Link(const string& flags);

private:
	Compiler& _compiler;
	Project& _project;
	ProjectGraph _projectSource;
	BuilderCache _cache;
};
