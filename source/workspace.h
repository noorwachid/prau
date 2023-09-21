#pragma once

#include "common.h"
#include "project.h"
#include "projectgraph.h"
#include "builder.h"

class Workspace {
public:
	Workspace(Compiler& compiler, const unordered_map<string, string>& options);
	
	void bootstrap();

	void show();

	void showGraph();

	void build();

	void clean();

	void run(const vector<string>& arguments);

	void init();
	
	void generate(const string& target);

private:
	bool loadProject(const string& path);

private:
	Compiler& _compiler;

	unordered_map<string, string> _options;
	unordered_map<string, Project> _projects;

	// build stages 
	// - [leaf, leaf, leaf]
	// - [branch, branch]
	// - [root]
	vector<vector<string>> _stages;
	
	vector<string> _parsingOrderPaths;

	friend class Generator;
};

