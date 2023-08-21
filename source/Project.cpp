#include "Project.h"
#include "Platform.h"

string ProjectPath::Generate(const string& directory)
{
	string file = "project.prau";

	return directory.empty() || directory == "." ? file : directory + "/" + file;
}

Project ProjectLoader::Load(const string& path)
{
	YAML::Node node = YAML::LoadFile(path);
	Project project = node.as<Project>();
	return project;
}
