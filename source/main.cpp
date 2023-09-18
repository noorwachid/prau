#include "builder.h"
#include "platform.h"
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

void build(const string& mode, bool verbose) {
	string projectPath = getProjectPath();
	if (projectPath.empty()) {
		return;
	}

	Compiler* compiler = new ClangCompiler();
	Project project = ProjectLoader::load(projectPath);
	Builder builder(*compiler, project);
	builder.setMode(mode);
	builder.setVerbose(verbose);
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

void run(const string& mode, bool verbose, const vector<string>& arguments) {
	string projectPath = getProjectPath();
	if (projectPath.empty()) {
		return;
	}

	Compiler* compiler = new ClangCompiler();
	Project project = ProjectLoader::load(projectPath);
	Builder builder(*compiler, project);
	builder.setMode(mode);
	builder.setVerbose(verbose);
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
	string mode = "release";
	string platform = Platform::get();
	bool verbose = false;

	vector<string> forwardedArguments;

	for (int i = 2; i < argc; ++i) {
		string argument = argv[i];
		if (argument.size() > 1 && argument[0] == '-' && argument[1] == '-') {
			if (argument.size() == 2) {
				for (int j = i + 1; j < argc; ++j) {
					forwardedArguments.push_back(argv[j]);
				}
				break;
			}

			if (argument == "--mode" && i + 1 < argc) {
				mode = argv[i + 1];
				++i;
			}

			if (argument == "--platform" && i + 1 < argc) {
				platform = argv[i + 1];
				++i;
			}

			if (argument == "--verbose") {
				verbose = true;
			}
		}
	}

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
			build(mode, verbose);
			return 0;
		}

		if (command == "run") {
			run(mode, verbose, forwardedArguments);
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
