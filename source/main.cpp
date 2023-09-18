#include "builder.h"
#include "starter.h"

string getProjectPath() {
	string projectPath = ProjectPath::generate(".");

	if (!fs::exists(projectPath)) {
		cout << "project file were not found\n";
		return "";
	}

	return projectPath;
}

void infoProject() {
	string projectPath = getProjectPath();
	if (projectPath.empty()) {
		return;
	}

	Project project = ProjectLoader::load(projectPath);
	YAML::Node projectNode;
	projectNode = project;
	cout << projectNode << "\n";
}

void infoProjectGraph() {
	string projectPath = getProjectPath();
	if (projectPath.empty()) {
		return;
	}

	Project project = ProjectLoader::load(projectPath);
	ProjectGraph projectSource = ProjectGraphLoader::load(project);
	YAML::Node projectSourceNode;
	projectSourceNode = projectSource;

	cout << projectSourceNode << "\n";
}

void build() {
	string projectPath = getProjectPath();
	if (projectPath.empty()) {
		return;
	}

	Compiler* compiler = new ClangCompiler();
	Project project = ProjectLoader::load(projectPath);
	Builder builder(*compiler, project);
	builder.build();
}

void clean() {
	string projectPath = getProjectPath();
	if (projectPath.empty()) {
		return;
	}

	Compiler* compiler = new ClangCompiler();
	Project project = ProjectLoader::load(projectPath);
	Builder builder(*compiler, project);
	builder.clean();
}

void run(const vector<string>& arguments) {
	string projectPath = getProjectPath();
	if (projectPath.empty()) {
		return;
	}

	Compiler* compiler = new ClangCompiler();
	Project project = ProjectLoader::load(projectPath);
	Builder builder(*compiler, project);

	builder.run(arguments);
}

void create(const string& directory) {
	if (fs::exists(directory)) {
		cout << directory << " already exists\n";
		return;
	}

	Starter starter;
	starter.generate(directory);
}

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "no command were specified\n";
		return 0;
	}

	string command = argv[1];

	try {
		if (command == "info-project") {
			infoProject();
			return 0;
		}

		if (command == "info-project-graph") {
			infoProjectGraph();
			return 0;
		}

		if (command == "build") {
			build();
			return 0;
		}

		if (command == "run") {
			vector<string> arguments;
			for (int i = 2; i < argc; ++i) {
				arguments.push_back(argv[i]);
			}
			run(arguments);
			return 0;
		}

		if (command == "create") {
			if (argc < 3) {
				cout << "please specify the new directory\n";
				return 0;
			}

			create(argv[2]);
			return 0;
		}

		if (command == "clean") {
			clean();
			return 0;
		}

		cout << "unknown command\n";
		return 0;
	} catch (const std::exception& e) {
		cout << e.what() << "\n";
	}

	return 0;
}
