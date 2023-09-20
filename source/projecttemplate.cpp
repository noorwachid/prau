#include "projecttemplate.h"

void ProjectTemplate::generate() {
	string sourceDirectory = "source";
	string projectPath = ProjectPath::generate(".");
	string mainPath = sourceDirectory + "/main.cpp";

	Project project;
	project.name = "helloworld";
	project.description = "a hello world project";
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
