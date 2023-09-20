#include "workspace.h"
#include "project.h"

Workspace::Workspace(Compiler& compiler): _compiler(compiler) {
	loadProject("");

	for (const string& path: _parsingOrderPaths) {
		Project& parentProject = _projects[path];
		for (const string& dependency: parentProject.dependencies) {
			Project& childProject = _projects[dependency];
			for (const string& headerPath: childProject.headerPaths) {
				parentProject.headerPaths.push_back(headerPath);
			}
		}
	}
}

bool Workspace::loadProject(const string& path) {
	string projectPath = (fs::path(path)).lexically_normal().string();
	string projectFile = (fs::path(path) / fs::path("project.prau")).lexically_normal().string();

	if (!fs::exists(projectFile)) {
		cout << projectFile << " were not found\n";
		exit(1);
	}

	if (_projects.count(projectPath)) {
		return false;
	}

	_parsingOrderPaths.push_back(projectPath);
	_projects[projectPath] = ProjectLoader::load(projectFile);

	for (string& dependency: _projects[projectPath].dependencies) {
		string subProjectPath = (fs::path(path) / fs::path(dependency)).lexically_normal().string();
		string subProjectFile = (fs::path(path) / fs::path(dependency) / fs::path("project.prau")).lexically_normal().string();

		dependency = subProjectPath;

		if (!fs::exists(subProjectFile)) {
			cout << subProjectFile << " were not found\n";
			exit(1);
		}

		if (!loadProject(subProjectPath)) {
			continue;
		}

		Project& subProject = _projects[subProjectPath];
		Project& project = _projects[projectPath];

		for (string& source: subProject.sources) {
			source = (fs::path(subProjectPath) / fs::path(source)).lexically_normal().string();
		}

		for (string& headerPath: subProject.headerPaths) {
			headerPath = (fs::path(subProjectPath) / fs::path(headerPath)).lexically_normal().string();
		}
	}

	return true;
}

void Workspace::show() {
	bool first = true;

	for (auto& [path, project]: _projects) {
		if (!first) {
			cout << "---\n";
		}

		YAML::Node node;
		node = project;
		cout << node << "\n";

		first = false;
	}
}

void Workspace::showGraph() {
}

void Workspace::build() {
	unordered_set<string> recompiling;

	for (size_t i = _parsingOrderPaths.size(); i > 0; --i) {
		Project& project = _projects[_parsingOrderPaths[i - 1]];

		vector<BuildDependency> buildDependencies;

		for (const string& dependency: project.dependencies) {
			BuildDependency buildDependency;
			buildDependency.project = _projects[dependency].name;
			buildDependency.recompiling = recompiling.count(buildDependency.project);
			buildDependency.headerOnly = _projects[dependency].sources.empty();
			buildDependencies.push_back(buildDependency);
		}

		Builder builder(_compiler, project, buildDependencies);
		builder.setVerbose();
		BuildResult result = builder.build();

		if (result.recompiling) {
			recompiling.insert(project.name);
		}
	}
}

void Workspace::run() {
}
