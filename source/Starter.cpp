#include "Starter.h"

void Starter::generate(const string& directory) {
	string sourceDirectory = directory + "/source";
	string projectPath = ProjectPath::generate(directory);
	string mainPath = sourceDirectory + "/main.cpp";

	Project project;
	project.name = fs::path(directory).filename();
	project.description = "a starter template";
	project.type = "executable";
	project.language = "cpp";
	project.standard = "17";
	project.sources.push_back("source/main.cpp");

	YAML::Node node;
	node = project;

	if (!fs::exists(sourceDirectory)) {
		fs::create_directories(sourceDirectory);
	}

	ofstream projectStream(projectPath);
	projectStream << node;

	ofstream mainStream(mainPath);
	mainStream << "#include <iostream>\n";
	mainStream << "\n";
	mainStream << "int main() {\n";
	mainStream << "\tstd::cout << \"hello world!\\n\";\n";
	mainStream << "\n";
	mainStream << "\treturn 0;\n";
	mainStream << "}";
}
