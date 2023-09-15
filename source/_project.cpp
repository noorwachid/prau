#include "Project.h"
#include "Platform.h"

string ProjectPath::generate(const string& directory) {
	string file = "project.prau";

	return directory.empty() || directory == "." ? file : directory + "/" + file;
}

Project ProjectLoader::load(const string& path) {
	YAML::Node node = YAML::LoadFile(path);
	Project project = node.as<Project>();
	return project;
}
