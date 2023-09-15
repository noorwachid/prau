#pragma once

#include "Compiler.h"
#include "ProjectGraph.h"
#include "Term.h"

class BuilderCache {
public:
	BuilderCache(Project& project, ProjectGraph& projectSource);

	void invalidate();

	void validate();

	bool isValid() const;

	vector<string> getInvalidSources() const;

private:
	bool isSourceFile(const string& source) const;

	string getFile() const;

	void addInvalidSource(const string& source);

private:
	bool _valid = false;
	Project& _project;
	ProjectGraph& _projectSource;
	unordered_set<string> _invalidSources;
};

class Builder {
public:
	bool verbose = true;

	Builder(Compiler& compiler, Project& project);

	void build();

	void run();

	void clean();

private:
	bool compile(const string& flags, const vector<string>& sources);

	bool link(const string& flags);

private:
	Compiler& _compiler;
	Project& _project;
	ProjectGraph _projectSource;
	BuilderCache _cache;
};
