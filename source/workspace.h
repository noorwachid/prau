#pragma once

#include "common.h"
#include "project.h"
#include "projectgraph.h"
#include "builder.h"

class Workspace {
public:
	Workspace(Compiler& compiler);

	void show();

	void showGraph();

	void build();

	void run();

private:
	bool loadProject(const string& path);

private:
	unordered_map<string, Project> _projects;

	// build stages 
	// - [leaf, leaf, leaf]
	// - [branch, branch]
	// - [root]
	vector<vector<string>> _stages;
	
	vector<string> _parsingOrderPaths;

	Compiler& _compiler;
};
