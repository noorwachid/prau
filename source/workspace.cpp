#include "generator.h"
#include "workspace.h"
#include "project.h"
#include "projecttemplate.h"

Workspace::Workspace(Compiler& compiler, const unordered_map<string, string>& options): _compiler(compiler), _options(options) {
}

void Workspace::bootstrap() {
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

void Workspace::clean() {
	if (!fs::exists(ProjectPath::generate("."))) {
		cout << "whoa there's no project file in this directory, careful dude\n";
		return;
	}

	if (!fs::exists("build")) {
		cout << "already clean\n";
		return;
	}

	fs::remove_all("build");
	cout << "cleaned up\n";
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
		builder.setMode(_options["mode"]);

		if (_options.count("verbose")) {
			builder.setVerbose();
		}

		BuildResult result = builder.build();

		if (result.recompiling) {
			recompiling.insert(project.name);
		}
	}
}

void Workspace::run(const vector<string>& arguments) {
	for (auto& [path, project]: _projects) {
		Builder builder(_compiler, project, {});
		if (builder.requireRebuilding()) {
			build();
			break;
		}
	}

	Project& project = _projects[""];
	string command = "./build/executable/" + _compiler.composeExecutableFile(project.name);

	for (const string& argument: arguments) {
		command += " " + argument;
	}

	system(command.c_str());
}

void Workspace::init() {
	ProjectTemplate projectTemplate;
	projectTemplate.generate();
}

void Workspace::generate(const string& target) {
	Generator generator(*this);
	generator.generate(target);
}
