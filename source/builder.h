#pragma once

#include "compiler.h"
#include "projectgraph.h"
#include "term.h"

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
	Builder(Compiler& compiler, Project& project);

	void setVerbose(bool verbose = true);

	void setMode(const string& mode);

	void build();

	void run(const vector<string>& arguments);

	void clean();

private:
	bool compile(const string& flags, const vector<string>& sources);

	bool link(const string& flags);

private:
	Compiler& _compiler;
	Project& _project;
	ProjectGraph _projectGraph;
	BuilderCache _cache;

	string _mode;
	bool _verbose = false;
};
